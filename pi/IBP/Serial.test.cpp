#include "Serial.hpp"

#include <iostream>
#include <thread>
#include <csignal>

Serial s ("/dev/ttyUSB0");

void testSerial(){
	
	int k = 0;
	int bytesSend = 0;
	int bytesRecv = 0;
	char buffer2[9] = {'1','2','3','4','5','6','7','8'};
	
	while (k < 1000)
	{
			
		//bytesSend += s.send(buffer2,8);
		bytesRecv += s.recv(buffer2,8);
		buffer2[9] = '\0';
		std::cout << "Received: " << buffer2 << std::endl;
		k++;
	}
	
	std::cout << "Bytes send: " << bytesSend << "Bytes received: " << bytesRecv << std::endl;
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
