#include "IBC.hpp"

#include "IBC_Packet.hpp"

#include <cstdint>
#include <exception>
#include <iostream>

#include <stdexcept>
#include <chrono>

using std::cout;

int run();

int main (int argc , char** argv)
{
	try{
	run();
	}
	catch(std::runtime_error e)
	{
		std::cout << e.what()<<'\n';
	}
	return 0;
}

int run ()
{
	IBC ibc("/dev/ttyUSB0", "IBC_config.cfg");

	//construction inbox
	Inbox i1 = ibc.getInbox(254);

	uint8_t buffer [4] = {'f','u','c','k'};

	Packet p (254, 4, buffer);

	ibc.send(p);

	std::this_thread::sleep_for(std::chrono::seconds(10));

	i1.fetch();

	if(i1.empty())
	{
		std::cout << "FOOOL !";
	}
	else std::cout << *i1.front();

	return 0;
}
