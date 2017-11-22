
#include "IBC_Transceiver.hpp"

#include <iostream>		//cerr
#include <cstring>		//std::memcpy

IBC::Transceiver::Transceiver(std::string device,const IBC::Rule & rule)
	:
	s(device),
	rule(rule),
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
	uint8_t req_buffer [256] = {0};

	uint8_t& req_id = send_buffer[0];
	uint8_t& req_dynamic_size = send_buffer[1];

	uint8_t * req_content;
	uint8_t * req_status_byte;

	uint8_t answ_buffer [256] = {0};

	uint8_t& answ_status_byte = recv_buffer[0];
	uint8_t& answ_dynamic_size = recv_buffer[1];

	uint8_t * answ_content;

	while (running)
	{
		if (tosend.empty())
		{
			std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME);								//idle if there is nothing to do
			continue;																		//start anew and check again first
		}
	
		auto next = tosend.top();															//retrieve the next packet to send


		uint8_t req_contentsize = rule.requestsize(next.id());
		bool req_dynamic = req_contentsize == IBC_RULE_DYNAMIC;

		if(req_dynamic)
		{
			req_contentsize = next.size();
			req_content = req_buffer + 2;
		}
		else
		{
			req_content = req_buffer +1;
		}

		uint8_t requestsize = req_contentsize + (req_dynamic) ? 3:2;

		int paddinglength = 0;
		
		if(req_contentsize > next.contentsize())
		{
			paddinglength = rule.requestsize(next.id()) - next.contentsize();

			//TODO log warning
			std::cerr << "WARNING [IBC] : Size of Packet smaller than specified ! Added padding to compensate !\n";
		}
		if(rule.requestsize < next.contentsize())
		{
			//TODO log warning
			std::cerr << "WARNING [IBC] : Size of Packet larger than specified ! Only the specified length will be sent !\n";
		}


		//fill buffer
		req_id = next.id();
		if(req_dynamic)
		{
			req_dynamic_size = requestsize;
		}

		std::memcpy(req_content, next.content(), next.size());

		if(paddinglength)
		{
			uint8_t * paddingstart = req_content + next.size();
			uint8_t * paddingend = paddingstart + paddinglenght;

			for(uint8_t* paddingstart; paddingstart < paddingend; paddingstart++ )
			{
				*paddingstart = IBP_PADDING;
			}
		}

		send_status_byte = send_content + requestsize;

		//calculate status byte for package you just sent
		*send_status_byte = this->status << 6;	

		*send_status_byte |= hash6(send_buffer, framesize);

		//send buffer
		send_intern(send_buffer, framesize);

		//request has been sent


		//we always expect at least one byte as respond so we can receive this first
		
		answ_writer = answ_buffer;
		recv_intern(answ_writer, 1);
		answ_writer += 1;

		//we check if the status is wrong (good is 0)
		if((answ_status_byte >> 6))
		{
			//TODO ERROR HANDLING
			//we need the second byte for the size
			////TODO
			std::cerr << "ERROR [IBC TRANSCEIVER] : A bad status <" + (answ_status_byte >> 6) + "> was returned in an answer! " + answ_buffer[1] + " bytes supposedly transmitted wrongly!\n";
		}

		uint8_t answ_contentsize = rule.answersize(next.id());
		bool answ_dynamic = answ_contentsize == IBC_RULE_DYNAMIC;

		unsigned int torecv;

		//we need to read out the contentsize if it is dynamic
		if(answ_dynamic)
		{
			recv_intern(answ_writer, 1);		// we need to receive the answers contentsize
			answ_writer+=1;

			answ_contentsize = answ_dynamic_size;
			answ_content = answ_buffer + 2;		//first 2 are status bit and size
			torecv = answ_contentsize;
		}
		else
		{
			answ_content = answ_buffer + 1;		//first one is status bit
			torecv = answ_contentsize;
		}

		recv_intern(answ_writer, torecv);

		//TODO checkhash

		std::shared_ptr<IBC::Packet> answer (new Packet (next.id(), answ_contentsize, answ_content));		

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

void recv_intern(uint8_t * data, uint8_t torecv) const
{
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
}

void send_intern(uint8_t* data, int size) const
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
}

uint8_t IBC::Transceiver::status_byte(const Packet& p, uint8_t paddinglenght, uint8_t status) const
{

	uint8_t hash = hash6(p);

	return hash ^ (status) ^ (status<<2) ^ (status<<4);	//squash the status bit into the hash by xor
}

uint8_t IBC::Transceiver::hash(const Packet& p, bool dynamic, uint8_t status)
{
	auto id = p.id();
	auto sum = hash6(&id ,1 ,0);
	
	sum = hash6(p.content(), )
}

uint8_t IBC::Transceiver::hash6(uint8_t * data, uint8_t length , uint8_t sum) const
{	
	for (int i = 0 ; i < length; i++)
	{
		uint8_t up = data[i] >> 6;								//highest 2 bits of a byte
		uint8_t low = (up | (up << 2) | (up << 4)) ^ data[i];	//xor them with all the other 2 bit pairs of the byte
		sum ^= low;												//xor this with the sum
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
