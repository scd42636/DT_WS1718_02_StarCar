#include "Serial.hpp"

#include <iostream>
#include <thread>
#include <csignal>
#include <sys/time.h>

Serial s ("/dev/ttyUSB0");

void testSerial(){
	
	int k = 0;
	int bytesSend = 0;
	int bytesRecv = 0;
	char buffer2[9] = {'1','2','3','4','5','6','7','8'};
	
    struct timeval tvalBefore, tvalAfter;

    gettimeofday(&tvalBefore,NULL);
    tvalAfter.tv_sec = tvalBefore.tv_sec + 5;

    while (tvalBefore.tv_sec < tvalAfter.tv_sec)
	{
			
        bytesSend += s.send(buffer2,8);
		bytesRecv += s.recv(buffer2,8);
		buffer2[9] = '\0';
        //std::cout << "Received: " << buffer2 << std::endl;
        gettimeofday(&tvalBefore,NULL);
	}

    //gettimeofday(&tvalAfter,NULL);

	std::cout << "Bytes send: " << bytesSend << "Bytes received: " << bytesRecv << std::endl;
    //std::cout << "Zeit: " << tvalAfter.tv_usec - tvalBefore.tv_usec << "µS" << tvalBefore.tv_sec - tvalAfter.tv_sec << "s";
}

void signalHandler (int signum) {
	
	if (signum == SIGINT){
	
		
	}
}


int main (int argc , char** argv)
{
	//signal(SIGINT,signalHandler);
	
	//Serial s ("/dev/ttyUSB0");	 
	std::thread serial(testSerial);
	serial.join();	 
	
	
	/*std::cout << "Sending 1235 to Arduino" << std::endl ;

	std::cout << "Bytes sent : " << s.send("2222", 4) << '\n';

	char buffer[5] = {};
	
	s.recv(buffer, 4);

	buffer[4] = '\0';
	
	for(int i = 0; i < 5; i++)
	{
		std::cout << buffer[i] ;
		}
	
	std::cout << std::endl << buffer << '\n';
	
	
	
	int k = 0;
	char buffer2[4];
	
	while (k < 5)
	{ 
		std::cin >> buffer2;
		s.send(buffer2, 4);
		s.recv(buffer2, 4);
		std::cout << buffer2 << std::endl;
		
		k++;
	}
	*/
	s.~Serial();
	
	return 0;
}
