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

	Inbox i = ibc.getInbox(180);

	Packet p (180, 0);

	ibc.send(p);

	std::this_thread::sleep_for(std::crhono::seconds(1));

	i.fetch();

	if(i.size())
    {
        std;;cout << i.front() << '\n';
    }

	return 0;
}
