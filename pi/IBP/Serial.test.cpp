#include "Serial.hpp"

#include <iostream>

int main (int argc , char** argv)
{
		 
	Serial s ("/dev/ttyUSB0");
	
	std::cout << "Sending 1235 to Arduino" << std::endl ;

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
	
	s.~Serial();
	
	return 0;
}
