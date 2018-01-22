
#include "IBC_Packet.hpp"
#include "iostream"

void pr(Packet& p)
{
	std::cout <<(int) p.id() << ":" <<(int) p.contentsize() << ":";
	for ( int i = 0; i < p.contentsize(); ++i )
	{
		std::cout << p.content()[i];
	}
	std::cout << '\n';
}

int main (int argc , char** argv)
{
	

	char data [13] = "Hello World!";
	Packet p (1, 13,(uint8_t*) data);

	pr(p);

	Packet p2 = p;

	pr(p2);

	Packet p3 = std::move(p);

	pr(p3);

	return 0;
}
