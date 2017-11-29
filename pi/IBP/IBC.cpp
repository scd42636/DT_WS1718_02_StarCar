
#include "IBC.hpp"

#include <fstream>
#include <iostream>
#include <regex>

IBC::IBC(std::string device , std::string configfile)
:
	rule(configfile),
	t(device, rule)
{}

void IBC::send(Packet& p)
{
	t.send(p);
}

Inbox IBC::getInbox()
{
	return Inbox(t);
}

Inbox IBC::getInbox(uint8_t id)
{
	return Inbox(t, id);
}

Inbox IBC::getInbox(std::vector<uint8_t> ids)
{
	return Inbox(t, ids);
}

uint8_t IBC::requestsize(uint8_t id) const
{
	rule.requestsize(id);
}

uint8_t IBC::responsesize(uint8_t id) const
{
	rule.answersize(id);
}
