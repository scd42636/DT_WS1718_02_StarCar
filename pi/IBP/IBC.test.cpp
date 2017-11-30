#include "IBC.hpp"

#include "IBC_Packet.hpp"

#include <cstdint>

int main (int argc , char** argv)
{
	IBC ibc("/dev/ttyUSB0", "IBC_config.cfg");

	Inbox i1 = ibc.getInbox(252);
	Inbox i2 = ibc.getInbox(std::vector<uint8_t>({253,254}));


	uint8_t ms1 [4] = {1,2,3,4};
	Packet p1(252, ibc.requestsize(252), (uint8_t*) ms1 );
	ibc.send(p1);

	uint8_t ms2[1] = {42};
	Packet p2 (253, 1, ms2);
	ibc.send(p2);

	uint8_t ms3[2] = {1,1};
	Packet p3(254, ibc.requestsize(254), (uint8_t*) ms3);
	ibc.send(p3);


	return 0;
}
