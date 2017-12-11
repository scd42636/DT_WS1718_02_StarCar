
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
	uint8_t req_buffer [256] = {0};

	uint8_t req_size;
	bool req_dynamic;
	uint8_t req_contentsize;
	uint8_t* req_content;

	uint8_t res_buffer [256] = {0};

	uint8_t res_size;
	bool res_dynamic;
	uint8_t res_contentsize;
	uint8_t* res_content;

	int failcount = 0;

	while (running)
	{
		transfer_sendbuffer();

		if (tosend.empty())
		{
			std::this_thread::sleep_for(IBC_TRANSCEIVER_IDLE_TIME);								//idle if there is nothing to do
			continue;																		//start anew and check again first
		}
		Packet* next = tosend.top().get();		//retrieve the next packet to send

		req_size = 0;
		req_dynamic = false;
		req_contentsize = 0;
		req_content = nullptr;

		res_size = 0;
		res_dynamic = false;
		res_contentsize = 0;
		res_content = nullptr;

		req_buffer[0] = next->id();

		req_buffer[1] |= this->status << 4;
	
		req_contentsize = rule.requestsize(next->id());
		if(req_contentsize == IBC_RULE_SIZE_DYNAMIC)
		{
			req_dynamic = true;
			req_contentsize = next->contentsize();
			req_size = req_contentsize + 4;
		}
		else
		{
			req_dynamic = false;
			req_size = req_contentsize + 3;
		}
		uint8_t paddinglength = 0;

		if(req_contentsize < next->contentsize())
		{
			std::cerr << "WARNING [IBC TRANSCEIVER] : Packet larger than specified ! Sending only specified payload ! <" << next->id() << '|' << next->contentsize() + ">\n";
		}
		if(req_contentsize > next->contentsize())
		{
			std::cerr << "WARNING [IBC TRANSCEIVER] : Packet smaller than specified ! Sending additional padding ! Please rework for efficiency !<" << next->id() << "|" << next->contentsize()+">\n";
			paddinglength = req_contentsize - next->contentsize();
		}

		req_buffer[1] |= (this->status << 4);

		req_buffer[1] |= (sizehash(req_contentsize) << 2);
		req_buffer[1] |= headhash_request(req_buffer);
	
		if(req_dynamic)
		{
			req_buffer[2] = req_contentsize;
			req_content = req_buffer + 3;
		}
		else
		{
			req_content = req_buffer + 2;
		}
		std::memcpy(req_content, next->content(), next->contentsize());
		
		//padding if neccessary
		if(!paddinglength)	padd(req_content + next->contentsize() + 1, paddinglength);

		//footer
		*(req_content + req_contentsize + 1) = datahash(req_content, req_contentsize);

		send_intern(req_buffer, req_size);

		//now recv header first
		recv_intern(res_buffer, 1); //response header 1 byte long
		
		//check header hash
		if(headhash_response(res_buffer) != (res_buffer[0] & 0x03))
		{
			failcount++;
			std::cerr << "[IBC TRANSCEIVER] Respone Headhash failed ! " << std::hex << (unsigned int)req_buffer[0] << ":" << (unsigned int)req_buffer[1] << "(Failcount: "<< failcount << " )\n" << std::dec;
			if(failcount > IBP_FAIL_MAX)
			{
				failcount = 0;
				//discard this packet because it failed to often
				tosend.pop();
				s.emptyRecvBuffer();
				continue;
			}
		}

		if(rule.answersize(next->id()) == IBC_RULE_SIZE_DYNAMIC)
		{
			res_dynamic = true;
			recv_intern(res_buffer+1, 1);		//recv size
			//check se size hash
			if(sizehash(res_buffer[1]) != (res_buffer[0] << 4 >> 4))
			{
				failcount++;
				std::cerr << "[IBC TRANSCEIVER] Response Sizehash failed ! " << std::hex << (unsigned int)req_buffer[0] << ":" << (unsigned int)req_buffer[1]<< ":" << (unsigned int)req_buffer[2] << "(Failcount: " << failcount << ")\n" << std::dec;
				if(failcount > IBP_FAIL_MAX)
				{
					failcount++;
					//discard this packet because it failed to often
					tosend.pop();
					s.emptyRecvBuffer();
					continue;	
				}
			}
			res_content = res_buffer + 2;
			res_contentsize = res_buffer[1];
			res_size = res_contentsize + 3;
		}
		else
		{
			res_dynamic = false;
			res_content = res_buffer +1;
			res_contentsize = rule.answersize(next->id());
			res_size = res_contentsize + 2;
		}

		recv_intern(res_content, res_contentsize + 1); //recv content and its hash

		uint8_t res_datahash = res_buffer[res_size-1];
		if(datahash(res_content, res_contentsize) != res_datahash)
		{
			failcount++;
			std::cerr << "[IBC TRANSCEIVER] Response Datahash failed ! " << std::hex << (unsigned int)req_buffer[0] << ":" << (unsigned int)req_buffer[1]<< ":" << (unsigned int)req_buffer[2] << ":";
			
			for(int i = 0 ; i < req_size+1; i++){std::cerr << std::hex << (unsigned int)req_buffer[i+3] << ":";}

			std::cerr << "(Failcount: " << failcount << ")\n" << std::dec;
			if(failcount > IBP_FAIL_MAX)
			{
				failcount++;
				//discard this packet because it failed to often
				tosend.pop();
				s.emptyRecvBuffer();
				continue;
			}
		}

		//construct and store the answer packet
		std::shared_ptr<const Packet> answer (new Packet (next->id(), res_contentsize, res_content));

		store(answer);

		//this request has been handled, so we erase it from the queue
		tosend.pop();
		failcount = 0;
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
		torecv -= received ;
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
	return	headerbegin[0] ^ (headerbegin[0] >> 2) ^ (headerbegin[0] >> 4) ^ (headerbegin[0] >> 6) ^ (headerbegin[1] >> 4) ^ (headerbegin[1] >> 6) & 0x03;
}

uint8_t Transceiver::headhash_response(uint8_t* headerbegin) const
{
	return	(headerbegin[0] >> 4) ^ (headerbegin[0] >> 6) & 0x03;
}

uint8_t Transceiver::sizehash(uint8_t size) const
{
	return (size ^ (size >> 2) ^ (size >> 4) ^ (size >> 6)) & 0x03;
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
