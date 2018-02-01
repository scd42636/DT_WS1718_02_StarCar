#include "Serial.hpp"

#include <iostream>

int main (int argc , char** argv)
{
	Serial s ("/dev/ttyUSB0");

	std::cout << "Bytes sent : " << s.send("Hello World!", 12);

	char buffer[13];

	s.recv(buffer, 12);

	buffer[12] = '\0';

	std::cout << buffer << '\n';

	return 0;
}
