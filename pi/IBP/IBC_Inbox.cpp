
#include "IBC_Inbox.hpp"

Inbox::Inbox(Transceiver& t)
	:
	t(&t)
{}


Inbox::Inbox(Transceiver& t, uint8_t id)
	:
	Inbox(t)
{
	listen(id);
}


Inbox::Inbox(Transceiver& t, std::vector<uint8_t> ids)
	:
	Inbox(t)
{
	listen(ids);
}

Inbox::~Inbox()
{
	//log out of every listening relationship before destroying the object
	for(auto id : listening)
	{
		mute(id);
	}
}

Inbox::Inbox(const Inbox& other)
	:
	std::list<std::shared_ptr<const Packet>> (other), //strangely necessary to copy the list
	t(other.t),
	listening(other.listening)
{
	for(auto id : listening)
	{
		listen(id);
	}
}

Inbox& Inbox::operator= (const Inbox& other)
{
	if(this == &other) return *this;
	this->t = other.t;
	this->listening = listening;
	for (auto id : listening)
	{
		listen(id);
	}
	return *this;
}

void Inbox::listen(uint8_t id) 
{
	t->addreceiver(*this, id);
	
	//add to own track
	listening.insert(id);
}


void Inbox::listen(std::vector<uint8_t> ids) 
{
	for(auto id : ids)
	{
		listen(id);
	}
}

void Inbox::mute(uint8_t id)
{
	t->removereceiver(*this, id);
	
	//remove from own track
	listening.erase(id);
}

void Inbox::mute(std::vector<uint8_t> ids)
{
	for(auto id : ids)
	{
		mute(id);
	}
}
