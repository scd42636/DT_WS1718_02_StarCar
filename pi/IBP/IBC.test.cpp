#include "IBC.hpp"

#include "IBC_Packet.hpp"

#include <cstdint>
#include <exception>
#include <iostream>

#include <chrono>

using std::cout;

int run();

int main (int argc , char** argv)
{
	try{
	run();
	}
	catch(std::exception e)
	{
		std::cout << e.what()<<'\n';
	}
	return 0;
}

int run ()
{
	IBC ibc("/dev/ttyUSB0", "IBC_config.cfg");

	//construction inbox
	Inbox i1 = ibc.getInbox(252);
	Inbox i2 = ibc.getInbox(std::vector<uint8_t>({253,254}));

//	//copy inbox
//	Inbox i3 (i1);

	uint8_t ms1 [4] = {1,2,3,4};
	Packet p1(252, ibc.requestsize(252), (uint8_t*) ms1 );
	ibc.send(p1);

	uint8_t ms2[2] = {42,42};
	Packet p2 (253, 2, ms2);
	ibc.send(p2);


//	uint8_t ms3[2] = {1,1};
//	Packet p3(254, ibc.requestsize(254), (uint8_t*) ms3);
//	ibc.send(p3);

	std::this_thread::sleep_for(std::chrono::seconds(10));

	i1.fetch();
	i2.fetch();
//	i3.fetch();

	if(!i1.size() || ! i2.size()) std::cerr << "FOOOOOOOOOOOOL !";

	std::cout << "i1:\n";
	for(auto e : i1)
	{
		std::cout << (unsigned int)e->id() << '|' << (unsigned int) e->contentsize() << '|';
		for ( int i = 0; i < e->contentsize(); ++i )
		{
			std::cout << std::hex <<(unsigned int)  e->content()[i] << ':';
		}
		std::cout << '\n';
	}
	std::cout << "i2:\n";
	for(auto e : i2)
	{
		std::cout << (unsigned int)e->id() << '|' << (unsigned int)e->contentsize() << '|';
		for ( int i = 0; i < e->contentsize(); ++i )
		{
			std::cout << std::hex << (unsigned int) e->content()[i] << ':';
		}
		std::cout << '\n';
	}
/*	for(auto e : i3)
	{
		std::cout << (unsigned int)e->id() << '|' << (unsigned int)e->contentsize() << '|';
		for ( int i = 0; i < e->contentsize(); ++i )
		{
			std::cout << std::hex << (unsigned int) e->content()[i] << ':';
		}
		std::cout << '\n';
	}*/

	return 0;
}
