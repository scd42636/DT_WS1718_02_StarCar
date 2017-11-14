
#include "IBC_Inbox.hpp"

IBC::Inbox::Inbox(Transceiver& t)
	:
	t(t)
{}


IBC::Inbox::Inbox(Transceiver& t, uint8_t id)
	:
	t(t)
{
	listen(id)
}


IBC::Inbox::Inbox(Transceiver& t, std::vector<uint8_t> ids)
	:
	t(t)
{
	listen(ids)
}

IBC::Inbox::~Inbox()
{
	//log out of every listening relationship before destroying the object
	for(auto id : listening)
	{
		mute(id);
	}
}

void IBC::Inbox::listen(uint8_t id) 
{
	//TODO log into t
	
	//add to own track
	listening.insert(id);
}


void IBC::Inbox::listen(std::vector<uint8_t> ids) 
{
	for(auto id : ids)
	{
		listen(id);
	}
}

void IBC::Inbox::mute(uint8_t id)
{
	//TODO log out of t
	
	//remove from own track
	listening.erase(id);
}

void IBC::Inbox::mute(std::vector<uint8_t> ids)
{
	for(auto id : ids)
	{
		mute(id);
	}
}
