#include "Serial_emulated.hpp"

#include <vector>
#include <iostream>

int main (int argc , char** argv)
{
	Serial s("-");

	std::vector<char> m252 = {(char)252 , (char)0x03, 1, 2, 3, 4, 4};

	s.send(m252.data(), 7);

	std::vector<char> m253 = {(char)253 , 0x02, 1, 42, 42};

	s.send(m253.data(), 5);

	std::vector<char> m254 = {(char)254 , 0x01, 1, 1, 0};

	s.send(m254.data(), 5);

	char buffer [256];

	std::cout << s.recv(buffer, 256) << '\n';

	std::cout << std::hex;

	for ( int i = 0; i < 256; ++i )
	{
		std::cout  << "|" <<  (0xFF& ((unsigned int) buffer[i]));
	}

	return 0;
}
