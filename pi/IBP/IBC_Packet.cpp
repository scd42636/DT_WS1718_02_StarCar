#include "IBC_Packet.hpp"

#include <cstring>

Packet::Packet(uint8_t id, uint8_t contentsize)
	:
	m_id(id),
	m_size(contentsize)
{
	m_content = new uint8_t [m_size];
}
	

Packet::Packet(uint8_t id, uint8_t contentsize, uint8_t * content)
	:
	Packet::Packet(id, contentsize)
{
	std::memcpy(m_content , content, contentsize);
}

Packet::~Packet()
{
	if(m_content) delete[] m_content;
}

Packet::Packet(const Packet& p)
	:
	Packet::Packet(p.m_id, p.m_size)
{
	std::memcpy(m_content, p.m_content, m_size);
}

Packet& Packet::operator=(const Packet& p)
{
	this->m_id = p.m_id;
	this->m_size = p.m_size;
	std::memcpy(this->m_content, p.m_content, this->m_size);
}

Packet::Packet(Packet&& p)
	:
	Packet::Packet(p.m_id, p.m_size)
{
	m_content = p.m_content;

	p.m_id = 0;
	p.m_size = 0;
	p.m_content = nullptr;
}

Packet& Packet::operator=(Packet&& p)
{	
	this->m_id = p.m_id;
	this->m_size = p.m_size;
	this->m_content = p.m_content;

	p.m_id = 0;
	p.m_size = 0;
	p.m_content = nullptr;
}

uint8_t Packet::contentsize() const
{
	return m_size;
}

uint8_t Packet::id()const
{
	return m_id;
}

uint8_t* Packet::content()const
{
	return m_content;
}

