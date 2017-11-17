
#include "IBC.hpp"

#include <fstream>
#include <iostream>
#include <regex>

IBC::IBC::IBC(std::string device , std::string configfile)
:
	t(device, configfile)
{}

IBC::IBC::~IBC()
{}

IBC::Packet IBC::IBC::makePacket(uint8_t id, void * content, uint8_t dyncontentsize) const
{
	uint8_t reqsize = t.rule.reqsize();
	bool dyn = false;
	if(reqsize == IBC_SIZE_DYNAMIC)
	{
		reqsize = dyncontentsize;
		dyn = true;
	}
	return IBC::Packet (id, reqsize, content, dyn);
}

IBC::Inbox IBC::IBC::getInbox()
{}

IBC::Inbox IBC::IBC::getInbox(uint8_t id)
{
	return IBC::Inbox(t, id);
}

IBC::Inbox IBC::IBC::getInbox(std::vector<uint8_t> ids)
{
	return IBC::Inbox(t, ids);
}

void IBC::IBC::send(IBC::Packet & p)
{
	t.send(p);
}
