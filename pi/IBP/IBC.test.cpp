#include "IBC.hpp"

#include "IBC_Packet.hpp"

#include <cstdint>
#include <exception>
#include <iostream>

#include <stdexcept>
#include <chrono>

using std::cout;

int run(IBC*);

int main (int argc , char** argv)
{
	IBC* ibc = new IBC("/dev/ttyUSB0", "IBC_config.cfg");
	try{
		run(ibc);
	}
	catch(std::runtime_error e)
	{
		std::cout << e.what()<<'\n';
	}
	return 0;
}

int run (IBC* ibc)
{
	

	Inbox* i = new Inbox(ibc->getInbox(180));

	Packet p (180, 0);

	ibc->send(p);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	i->fetch();

	if(i->size())
    {
		std::cout << *(i->front()) << '\n';
    }

	delete i;

	return 0;
}
