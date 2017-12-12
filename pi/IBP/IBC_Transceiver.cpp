
#include "IBC_Transceiver.hpp"

#include <iostream>		//cerr
#include <cstring>		//std::memcpy

#include <chrono>		//for idle duration

#include "IBC_Inbox.hpp"

using namespace std::chrono_literals;

Transceiver::Transceiver(std::string device,const Rule & rule)
	:
	s(device),
	rule(rule),
	running(false),
	tosend([](std::shared_ptr<Packet>& lhs,std::shared_ptr<Packet>& rhs){return lhs->id() > rhs->id();})
{
	sendbuff_lock.unlock();
}

Transceiver::~Transceiver()
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

void Transceiver::runworker(bool running)
{
	if(this->running == running) return; //exactly nothing will happen
	if(!this->running && running)
	{
		//we start the thread
		this->running = true;
		worker = std::thread(&Transceiver::body, this);
		return;
	}
	if(this->running && !running)
	{
		//we forcefully stop the thread
		//by causing its execution to come to an end
		this->running = false;
		//and detaching it from us so it can safely run out
		worker.detach();
		return;
	}
}


void Transceiver::body()
{

	while (running)
	{
		//reset status
		this->status = 0;

		transfer_sendbuffer();

		if (tosend.empty())
		{
			std::this_thread::sleep_for(IBC_TRANSCEIVER_IDLE_TIME);								//idle if there is nothing to do
			continue;																		//start anew and check again first
		}
		Packet* next = tosend.top().get();		//retrieve the next packet to send

		uint8_t req_header[2] = {0};	

		req_header[0] = next->id();
		req_header[1] |= (this->status << 4);
		req_header[1] |= headhash_request(header);

		send_intern(header, 2);


		uint8_t res_header[1] = {0};

		recv_intern(res_header, 1);		

		//TODO check res_header hash and status handle
		

		uint8_t req_data_header = statbyte();
		//send dataheader
		send_intern(req_data_header, 1);
		//send data
		send_intern(next->content(), next.contentsize());

		if(req_size != next.contentsize()){
			//TODO padding
		}


		//send data hash
		send_intern(datahash(next->content(), req_size, 0) , 1);

		//recv dataheader
		uint8_t req_data_header [1] = {0};

		recv_intern(req_data_header, 1);

		//TODO check data header and handle status
		
		
	}
}

void Transceiver::padd(uint8_t * begin, uint8_t paddinglength)
{
	for( ; begin < begin+paddinglength; begin++)
	{
		*begin = IBP_PADDING;
	}
}

void Transceiver::send(Packet & p)
{
	std::lock_guard<std::mutex> guard(sendbuff_lock);
	sendbuff.emplace_back(new Packet (p));
}

void Transceiver::addreceiver(Inbox& i , uint8_t id)
{
	receivers[id].insert(&i);
}

void Transceiver::removereceiver(Inbox& i, uint8_t id)
{
	receivers[id].erase(&i);
}

void Transceiver::recv_intern(uint8_t * data, uint8_t torecv)
{
	while(torecv > 0)
	{
		unsigned int received = 0;
		received = s.recv(reinterpret_cast<char*>(data),torecv);
		if(!received)
		{
			std::this_thread::sleep_for(IBC_TRANSCEIVER_IDLE_TIME);
		}
        torecv -= received;
		data += received;
	}
}

void Transceiver::send_intern(uint8_t* data, uint8_t tosend)
{
		//send data
		unsigned int sent = 0;
		while(tosend  > 0)
		{
			sent = 0;
			sent = s.send(reinterpret_cast<char*>(data) ,tosend);														//send until everything is through
			if(!sent)
			{
				std::this_thread::sleep_for(IBC_TRANSCEIVER_IDLE_TIME);
			}
			tosend -= sent;
			data += sent;
		}
}

uint8_t Transceiver::headhash_request(uint8_t* headerbegin) const
{
	return (headerbegin[0] ^ (headerbegin[0]>>4) ^ (headerbegin[1]>>4)) & 0x0F;
}

uint8_t Transceiver::headhash_response(uint8_t* headerbegin) const
{
	return	(headerbegin[0]>>4) & 0x0F;
}

uint8_t Transceiver::statbyte()
{
	return = (this->status << 4) | (this->status)


}

uint8_t Transceiver::datahash(uint8_t * data, uint8_t length, uint8_t in) const
{
	for ( int i = 0; i < length; ++i )
	{
		in ^= data[i];
	}
	return in;
}

void Transceiver::store(std::shared_ptr<const Packet>& answer)
{
	//we will give every receiver a shared ptr to the packet
	//the last actual holder will delete the package
	for(auto& box : receivers[answer->id()])
	{
		std::lock_guard<std::mutex> guard (box->l);
		box->dump.push_back(answer);
	}
}

void Transceiver::transfer_sendbuffer()
{
	if(sendbuff.empty()) return; // if there is nothing new to transfer we are here for naught

	//in case of an empty tosend buffer we have to wait for the lock	
	if(!tosend.empty())
	{
		std::lock_guard<std::mutex> guard (sendbuff_lock);

		//Black Magic
		//Basically it allows to use the std::back_inserter for the priority queue, since no other iterator interface seems to be implemented to use with the std::move algorithm
		
		for(auto& e : sendbuff)
		{
			tosend.push(e);
		}
		sendbuff.clear();
	}
	else	// to retreive new Packets is not urgent so we only try to lock here
	{
		if(sendbuff_lock.try_lock())		//we might lock here
		{
			for(auto& e : sendbuff)
			{
				tosend.push(e);
			}	
			sendbuff.clear();

			sendbuff_lock.unlock();			//unlock again int this case
		}
		//else lock could not be aquired
	}
}
