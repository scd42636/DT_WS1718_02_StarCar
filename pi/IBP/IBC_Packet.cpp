#include "IBC_Packet.hpp"

#define IBC_PACKET_M_HASH data[size - 2]

IBC_Packet::IBC_Packet(uint8_t id, void * data , uint8_t size)
	:
id(id),
size(size)
{
	
}

IBC_Packet::IBC_Packet(void * data)
{
	char* dataptr = data;
	id = dataptr[0];
	if (dynamic(id)) size = dataptr[1];
	else			 size = truesize(id);
}

uint8_t* IBC_Packet::content() const 
{
	//so if its dynamic the content starts at data[2]
	//and if its static the content starts right after the id at data[1]
	return (char *) data + ( dynamic(id()) ) ? 2 : 1 ;
}

uint8_t IBC_Packet::id() const
{
	return ((uint8_t *)data)[0];
}

uint8_t  IBC_Packet::size() const 
{
	return size;
}

uint8_t IBC_Packet::contentsize() const
{
	return size - 1 - content() - data;
}

uint8_t IBC_Packet::hash() const 
{
	return hash8(data, size - 1 ); // data without the part for hash at the end will be hashed
}

void IBC_Packet::sethash(uint8_t nhash) const 
{
	IBC_PACKET_M_HASH = hash();
}

bool IBC_Packet::verify() const 
{
	return IBC_PACKET_M_HASH == hash();
}
