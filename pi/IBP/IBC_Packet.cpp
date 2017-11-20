#include "IBC_Packet.hpp"

IBC::Packet::Packet(uint8_t id, uint8_t contentsize)
	:
	m_id(id),
	m_size(contentsize)
{
	m_content = new uint8_t [m_size];
}
	

IBC::Packet::Packet(uint8_t id, uint8_t contentsize, uint8_t * content)
	:
	IBC::Packet::Packet(id, contentsize)
{
	memcpy(m_content , content, contentsize);
}

IBC::Packet::~Packet()
{
	if(m_content) delete[] m_content;
}

IBC::Packet::Packet(const Packet& p)
	:
	IBC::Packet::Packet(p.m_id, p.m_size)
{
	memcpy(m_content, p.m_content, m_size);
}

IBC::Packet::Packet(Packet&& p)
	:
	IBC::Packet::Packet(p.m_id, p.m_size)
{
	m_content = p.m_content;

	p.m_id = 0;
	p.size = 0;
	p.m_content = nullptr;
}

uint8_t IBC::Packet::contentsize() const
{
	return m_size;
}

uint8_t IBC::Packet::id()const
{
	return m_id;
}

uint8_t* IBC::Packet::content()const
{
	return m_content;
}

