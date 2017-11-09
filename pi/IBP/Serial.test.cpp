#include "Serial.hpp"

#include <iostream>

int main (int argc , char** argv)
{
	//Serial d ("/dev/ttyUSB0");
	//d.~Serial();
	 
	Serial s ("/dev/ttyUSB0");
	
	std::cout << "Sending 1235 to Arduino" << std::endl ;

	std::cout << "Bytes sent : " << s.send("1235", 4) << '\n';

	char buffer[5] = {};
	
	s.recv(buffer, 4);

	//buffer[4] = '\0';
	
	for(int i = 0; i < 5; i++)
	{
		std::cout << buffer[i] ;
		}
	
	std::cout << std::endl << buffer << '\n';
	
	s.~Serial();
	
	return 0;
}
