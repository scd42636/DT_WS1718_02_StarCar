#include "IBC_Packet.hpp"

IBC_Packet::IBC_Packet(uint8_t id, void * data , uint16_t size = IBC_PACKET_NOSIZE, bool copy = true)
	:
id(id),
size(size)
{
	if(!size) size = truesize(id);

	if(copy)
	{
		this->data = new char [size];
		memcpy(this->data , data, size);
	}
	else
	{
		this->data = data;
	}

	m_hash = hash();
}

IBC_Packet::IBC_Packet(void * data)
{
	char* dataptr = data;
	id = dataptr[0];
	if (dynamic(id)) size = dataptr[1];
	else			 size = truesize(id);

}

void* IBC_Packet::content() const 
{
	return data;
}

uint16_t  IBC_Packet::size() const 
{
	return size;
}

uint8_t IBC_Packet::hash() const 
{
	uint8_t newhash;
	//TODO hash function
	for(uint16_t i = 0; i < size ; i++)
	{
		
	}
}

void IBC_Packet::sethash(uint8_t nhash) const 
{
	hash = nhash;
}

bool IBC_Packet::verify() const 
{
	return m_hash == hash();
}
