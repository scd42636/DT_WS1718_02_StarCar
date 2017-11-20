
#include "IBC_Transceiver.hpp"

IBC::Transceiver::Transceiver(std::string device, std::string configfile)
	:
	s(device),
	rule(configfile)
	running(false)
{}

IBC::Transceiver::~Transceiver()
{
	//stop the worker
	runworker(false);

	//we need to detach all remote inboxes from us to safely destruct
	for (unsigned int i = 0; i < 256 ; i++)
	{
		for(Inbox* ptr : receivers[i])
		{
			//invalidate the pointer inside of all referenced Inboxes
			ptr->t = nullptr;
			// also trigger a mute on the inbox
			ptr->mute(i);
		}
	}
}

void IBC::Transceiver::runworker(bool running)
{
	//set flag accordingly
	this->running = running;
	
	if(running)
	{
		//start thread
		worker = std::thread(&IBC::Transceiver::body(), this);
	}
	else
	{
		//stop thread (flag will cause thread to exit loop)
		//we will then simply let it run out
		worker.detach();
	}

}


void IBC::Transceiver::body()
{
	while (running)
	{
		if (tosend.empty())
		{
			std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME);								//idle if there is nothing to do
			continue;																		//start anew and check again first
		}

		auto next = tosend.top();															//retrieve the next packet to send

		{
			uint8_t id = next.id();
			send_intern(&id, 1);
		}

		uint8_t requestsize = rule.requestsize(next.id());

		if(requestsize == IBC_RULE_DYNAMIC)
		{
			requestsize = next.contentsize();
			send_intern(&requestsize, 1);
		}

		int paddinglenght = 0;
		if(requestsize > next.contentsize())
		{
			paddinglenght = rule.requestsize() - next.contentsize();

			//TODO log warning
		}
		if(rule.requestsize < next.contentsize())
		{
			//TODO log warning
		}


		uint8_t
		send_intern(&next.size();)

		unsigned int bytestosend = next->size();													

		uint8_t * data = next->data();


		uint8_t status_byte_send = status_byte(next);												//calculate status byte for package you just sent

		//send the status byte
		while (! s.send(&status_byte_send , 1)) std::this_thread::sleep/IBC_TRANSCEIVER_IDLE_TIME);	//send this status byte after the package bytes have been sent


		uint8_t answersize = rule.answersize(next.id());											//determine expected contentsize of answer

		std::shared_ptr<Packet> answer = new Packet(answersize);									
		
		unsigned int torecv = answer.size();
		data = answer->data();

	//TODO dynamic answer !

		while(torecv > 0)
		{
			unsigned int received = 0;
			received = s.recv(data, torecv);
			if(!received)
			{
				std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME)
			}
			torecv -= received ;
			data += received;
		}

		uint8_t status_byte_recv;

		while (!s.recv(&status_byte_recv, 1)) std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME);

		store(answer);
		tosend.pop();
	}

}

void IBC::Transceiver::send(Packet & p)
{
	//TODO mutex
	tosend.emplace(p);
}

void IBC::Transceiver::addreceiver(Inbox& i , uint8_t id)
{
	receivers[id].insert(&i);
}

void IBC::Transceiver::removereceiver(Inbox& i, uint8_t id)
{
	receivers[id].erase(&i);
}

void send_intern(uint8_t* data, int size, int paddinglenght) const
{
		//send data
		unsigned int sent = 0;
		while(size  > 0)
		{
			sent = 0;
			sent = s.send(data , send);														//send until everything is through
			if(!sent)
			{
				std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME);
			}
			size -= sent;
			data += sent;
		}

		//send padding
		uint8_t padding = 0xAA;
		while(paddinglength > 0)
		{
			sent = 0;
			sent = s.send(&padding, 1);
			paddinglenght -= sent;
		}

}

uint8_t IBC::Transceiver::status_byte(const Packet& p , uint8_t status) const
{
	uint8_t hash = hash6(p);

	return hash ^ (status) ^ (status<<2) ^ (status<<4);	//squash the status bit into the hash by xor
}

uint8_t IBC::Transceiver::hash6(const Packet& p) const
{
	uint8_t sum = 0 ;
	
	for (int i = 0 ; i < p.size(); i++)
	{
		sum ^= data[i];								//xor all the data in the bytes
		uint8_t up = sum >> 6;						//squash the highest bytes into the 6 lower via another xor
		uint8_t low = up ^ (up << 2) ^ (up << 4)
		sum ^= low;	
	}

	return sum << 2 >> 2;							//return last 6 bits and assure that first two bits are 0
}

void IBC::Transceiver::store(std::shared_ptr<const Packet> answer)
{
	//we will give every receiver a shared ptr to the packet
	//the last actual holder will delete the package
	for (auto r : receivers[answer->id()])
	{
		r.push_back(answer);
	}
}
