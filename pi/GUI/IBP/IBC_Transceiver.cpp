
#include "IBC_Transceiver.hpp"

#include <iostream>		//cerr
#include <cstring>		//std::memcpy

#include <chrono>		//for idle duration

#include "IBC_Inbox.hpp"

#include <QDebug>

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
	uint8_t& mstat = this->status;
	uint8_t sstat = 0;

	int failcount = 0;

	while (running)
	{
		//reset status
		mstat = 0;
		sstat = 0;

		transfer_sendbuffer();

		if (tosend.empty())
		{
			std::this_thread::sleep_for(IBC_TRANSCEIVER_IDLE_TIME);								//idle if there is nothing to do
			continue;																		//start anew and check again first
		}
		Packet* next = tosend.top().get();		//retrieve the next packet to send


		//actual sending and receiving stuff begins here

        /*

		uint8_t message_header[2] = {0};

		message_header[0] = next->id();

		send_intern(message_header, 2);

		recv_intern(&sstat, 1);

		send_intern(&mstat, 1);

        */

        uint8_t id = next->id();


        send_intern(&id, 1);

		//determine a possible padding
		int padding = 0;
		if(rule.requestsize(next->id()) > next->contentsize())
		{
			//TODO WARNING
			padding = rule.requestsize(next->id()) - next->contentsize();
		}
		if(rule.requestsize(next->id()) < next->contentsize())
		{
			//TODO WARNING !
		}

		//send data over
		send_intern(next->content(), next->contentsize());

        /*
        //and the corresponding hash
		{
			uint8_t dh = datahash(next->content(), next->contentsize(), 0);

			//with possible padding
			if(padding)
			{
				uint8_t padd = IBP_PADDING;
				for(int i = 0 ; i < padding; i++)
				{
					send_intern(&padd, 1);
					dh = datahash(&padd, 1, dh);
				}
			}

			send_intern(&dh, 1);
		}

		recv_intern(&sstat, 1);
        */


		uint8_t * res_buffer = new uint8_t [rule.answersize(next->id())];
		recv_intern(res_buffer, rule.answersize(next->id()));

        /*
        uint8_t datahash;
		recv_intern(&datahash, 1);
        */

		//create packet and store it
		std::shared_ptr<const Packet> p (new Packet(next->id(), rule.answersize(next->id()), res_buffer));

		store(p);
		//std::cout << "Stored : " << *p << '\n';
		tosend.pop();
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
//	unsigned int mtorecv = torecv;

	while(torecv > 0)
	{
		unsigned int received = 0;
		received = s.recv(reinterpret_cast<char*>(data),torecv);
		if(!received)
		{
			std::this_thread::sleep_for(IBC_TRANSCEIVER_IDLE_TIME);
		}
        else
        {
            for (int i=0; i<received ; i++)
            {
                qDebug() << 'r' << data[i] << '(' << i << ")\n";
            }
        }
        torecv -= received;
		data += received;
	}


//		data -= mtorecv;
//		std::cout << "\nRECV["<<(unsigned int)mtorecv<<"] ";
//		for(int i = 0; i < mtorecv; i++)
//		{
//			std::cout << std::hex << (unsigned int) data[i] << ":";
//		}
//		std::cout << '\n';

}

void Transceiver::send_intern(uint8_t* data, uint8_t tosend)
{
//		unsigned int mtosend = tosend;

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
            else
            {
                for (int i=0; i<sent ; i++)
                {
                    qDebug() << 's' << data[i] << '(' << i << ")\n";
                }
            }
			tosend -= sent;
			data += sent;
		}

//		data = data-mtosend;
//		std::cout << "\nSENT["<<(unsigned int)mtosend<<"] ";
//		for(int i = 0; i < mtosend; i++)
//		{
//			std::cout << std::hex << (unsigned int) data[i] << ":";
//		}
//		std::cout << '\n';
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
