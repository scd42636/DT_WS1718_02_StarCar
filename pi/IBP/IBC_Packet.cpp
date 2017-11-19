#include "IBC_Packet.hpp"
#include <stdexcept>

IBC::Packet::Packet(uint8_t id, uint8_t contentsize, uint8_t * content, bool dynamic)
{
	if(dynamic && contentsize > 253 || !dynamic && contentsize > 254)
	{
		throw std::runtime_error("ERROR : Failure in creating IBC::Packet, size too large !")
	}

	m_contentsize = contentsize;
	m_size = m_contentsize + dynamic ? 2 : 1 ;
	
	m_data = new uint8_t [m_size];

	data[0] = id;
	if(dynamic) data[1] = contentsize;
	memcpy(m_data + (dynamic?2:1) , content, contentsize);
}

IBC::Packet::~Packet()
{
	delete[] m_data;
}

uint8_t IBC::Packet::size() const
{
	return m_size;
}

uint8_t* IBC::Packet::data() const
{
	return m_data;
}

uint8_t IBC::Packet::id()const
{
	return m_data[0];
}

uint8_t* IBC::Packet::content()const
{
	return m_data + (m_size - m_contentsize);
}

uint8_t IBC::Packet::contentsize() const
{
	return m_contentsize;
}

bool IBC::Packet::dynamic() const
{
	return (m_size-m_contentsize) == 2;
}
