
#include "Serial_emulated.hpp"

#include <cstring>
#include <iostream>

Serial::Serial(std::string devicename)
{
}

Serial::~Serial()
{
}

void Serial::config()
{
}

int Serial::send(char * data , int size)
{
	//write to buffer
	for ( int i = 0; i < size; ++i )
	{
		sendbuff.push_back(data[i]);
	}

	//process frames here manually
	bool stop = false;
	while(!stop)
	{
		if(sendbuff.empty())
		{
			stop = true;
			break;
		}
		switch ((uint8_t) sendbuff.front() )
		{
			case 252:
				//normal static request
				if(sendbuff.size() < 7)
				{
					std::cerr << "You did crap 1 !\n";
					stop = true;
					break;
				}
				//status
				recvbuff.push_back(0x00);
				//data
				recvbuff.push_back(5);
				recvbuff.push_back(6);
				recvbuff.push_back(7);
				recvbuff.push_back(8);
				recvbuff.push_back(5);
				recvbuff.push_back(6);
				recvbuff.push_back(7);
				recvbuff.push_back(8);
	
				//hash
				recvbuff.push_back(0);
	
								//we sent this stuff and received a response at this point so away with it
				sendbuff.erase(sendbuff.begin(), sendbuff.begin()+7);

			break;
			case 253:
				//construct response from dynamic request
				if(sendbuff.size() < 3)
				{
					std::cerr << "You did crap 2 !\n";
					stop = true;
					break;
				}
				if(sendbuff.size() < sendbuff[2]+4)
				{
					std::cerr << "You did crap 3 !\n";
					break;
				}
				//status
				recvbuff.push_back(0);
				//send back the dynamic size 2 times
				recvbuff.push_back(sendbuff[2]);
				recvbuff.push_back(sendbuff[2]);
				//hash	
				recvbuff.push_back(sendbuff[2]^sendbuff[2]);
	
				sendbuff.erase(sendbuff.begin(), sendbuff.begin()+ sendbuff[2]+4);
			break;
			case 254:
				//dynamic response from static request
				if(sendbuff.size()< 5)
				{
					std::cerr << "You did crap 4 !\n";
					stop = true;
					break;
				}
				//status
				recvbuff.push_back(0);
				//size
				recvbuff.push_back(1);
				//data
				recvbuff.push_back(42);
				//hash
				recvbuff.push_back(1);
	
				sendbuff.erase(sendbuff.begin(), sendbuff.begin()+5);

			break;
		}
	}	
	return size;
}

int Serial::recv(char * data , int maxsize)
{
	if(recvbuff.empty()) return 0;
	unsigned int extract = (recvbuff.size() > maxsize) ? maxsize : recvbuff.size();
	std::memcpy(data, recvbuff.data(), extract);
	recvbuff.erase(recvbuff.begin(), recvbuff.begin()+extract);
	return extract;
}
