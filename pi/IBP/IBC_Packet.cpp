#include "IBC::Packet.hpp"

#define IBC_PACKET_M_ID this->data[0]
#define IBC_PACKET_M_CONTENTSIZE this->data[1]
#define IBC_PACKET_M_HASH this->data[size - 2]

IBC::Packet::Packet(uint8_t id, void * content , uint8_t contentsize)
{
	size = truesize(id, data);
	if(size) this->data = new uint8_t [size];
	IBC_PACKET_M_CONTENTSIZE = contentsize();
	memcpy
}

IBC::Packet::Packet(void * data)
{
	IBC_PACKET_M_ID = data[0];
	if (dynamic(id)) size = dataptr[1];
	else			 size = truesize(id);
}

uint8_t* IBC::Packet::content() const 
{
	//so if its dynamic the content starts at data[2]
	//and if its static the content starts right after the id at data[1]
	return data + ( dynamic(id()) ) ? 2 : 1 ;
}

uint8_t IBC::Packet::id() const
{
	return IBC_PACKET_M_ID;
}

uint8_t  IBC::Packet::size() const 
{
	return size;
}

uint8_t IBC::Packet::contentsize() const
{
	return size - 1 - content() - data;
}

uint8_t IBC::Packet::hash() const 
{
	return hash8( data, size - 1 ); // data without the part for hash at the end will be hashed
	//ID and size parts have effects !
}

void IBC::Packet::sethash(uint8_t nhash) const 
{
	IBC_PACKET_M_HASH = hash();
}

bool IBC::Packet::verify() const 
{
	return IBC_PACKET_M_HASH == hash();
}
