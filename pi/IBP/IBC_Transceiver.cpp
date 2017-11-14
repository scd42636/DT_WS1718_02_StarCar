
#include "IBC_Transceiver.hpp"

IBC::Transceiver::Transceiver()
	:
	s("/dev/ttyUSB0"),
	running(false)
{
	
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
			std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME);
			continue;
		}
//TODO ! dynamic SEND !
		auto next = tosend.top();

		unsigned int bytestosend = next->size();

		uint8_t * content = next->content();

		while(bytestosend > 0)
		{
			unsigned int sent = 0;
			sent = s.send(content , bytestosend);
			if(!sent)
			{
				std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME)
			}
			bytestosend -= sent;
			content += sent;
		}

		//package is sent, so get rid of it in the queue
		tosend.pop();

		//	get id of package
		//	determine sizes
		uint8_t requestsize = rule.requestsize(next->id());
		uint8_t answersize = rule.answersize(next.id());

		std::shared_ptr<Packet> answer = new Packet(answersize);
		
		unsigned int torecv = answer.size();
		content = answer->content();

	//TODO dynamic answer !

		while(torecv > 0)
		{
			unsigned int received = 0;
			received = s.recv(content, torecv);
			if(!received)
			{
				std::this_thread::sleep(IBC_TRANSCEIVER_IDLE_TIME)
			}
			torecv -= received ;
			content += received;
		}

		store(answer);
	}

}

void IBC::Transceiver::send(Packet & p)
{
	//TODO mutex
	tosend.emplace(p);
}

void IBC::Transceiver::store(std::shared_ptr<Packet>)
{
	
}
