
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

	uint8_t req_size;
	bool req_dynamic = false;
	uint8_t req_contentsize;

	uint8_t res_buffer [256] = {0};

	while (running)
	{
		if (tosend.empty())
		{
			std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME);								//idle if there is nothing to do
			continue;																		//start anew and check again first
		}	
		auto next = tosend.top();		//retrieve the next packet to send

		req_buffer[0] = next->id();

		req_buffer[1] |= this->status << 4;
		
		req_contentsize = rule.req_contentsize();
		if(req_contentsize == IBC_RULE_DYNAMIC)
		{
			req_dynamic = true;
			req_contentsize = next.size();
			req_size = req_contentsize + 4;
		}
		else
		{
			req_dynamic = false;
			req_size = req_contentsize + 3;
		}


		uint8_t paddinglenght = 0;

		if(req_contentsize < next.size())
		{
			std::cerr << "WARNING [IBC TRANSCEIVER] : Packet larger than specified ! Sending only specified payload ! <"+ next.id() +"|"+next.size()+">\n";
		}
		if(req_contentsize > next.size())
		{
			std::cerr << "WARNING [IBC TRANSCEIVER] : Packet smaller than specified ! Sending additional padding ! Please rework for efficiency !<"+ next.id() +"|"+next.size()+">\n";
			paddinglength = req_contentsize - next.size();
		}

		req_buffer[1] |= (sizehash(req_contentsize) << 2);
		req_buffer[1] |= headhash(req_buffer);
		
		if(req_dynamic)
		{
			req_buffer[2] = req_contentsize;
			req_content = req_buffer + 3;
		}
		else
		{
			req_content = req_buffer + 2;
		}
		std::memcpy(req_content, next.content(), next.size());
		
		//padding if necessary
		if(!paddinglength)	padd(req_content + next.size() + 1, paddinglength);

		//footer
		*(req_content + req_contentsize + 1) = datahash(req_content, req_contentsize);

		send_intern(req_buffer, req_size);

		//now recv header first
		recv_intern(res_buffer, 1); //response header 1 byte long
		
		//check header hash
		if(headhash(res_buffer) != (res_buffer[0] & 0x03)) //TODO ERROR HANDLING WITH STATUS;

		if(rule.answersize(res_buffer[0]) == IBC_RULE_DYNAMIC)
		{
			res_dynamic = true;
			recv_intern(res_buffer+1, 1);		//recv size
			//check se size hash
			if(sizehash(res_buffer[1]) != (res_buffer[0] << 4 >> 4)) //TODO ERRORHANDLING WRONG SIZE !
			res_content = res_buffer + 2;
			res_contentsize = res_buffer[1];
			res_size = res_contentsize + 3;
		}
		else
		{
			res_dynamic = false;
			res_content = res_buffer +1;
			res_contentsize = rule.answersize(res_buffer[0]);
			res_size = res_contentsize + 2;
		}
		recv_intern(res_buffer+2, res_contentsize + 1) //recv content and its hash

		uint8_t res_datahash = res_buffer[res_size-1];
		if(datahash(res_content, res_contentsize) != res_datahash) //TODO ERROR HADNLING DATA FAILURE !

		//construct and store the answer packet
		std::shared_ptr<const Packet> answer (new Packet (res_buffer[0], res_content, res_contentsize));
		store(answer);

		//this request has been handled, so we erase it from the queue
		tosend.pop();

	}

}

void IBC::Transceiver::padd(uint8_t * begin, uint8_t paddinglength)
{
	for( ; begin <= begin+paddinglength; begin++)
	{
		*paddingbegin = IBP_PADDING;
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
