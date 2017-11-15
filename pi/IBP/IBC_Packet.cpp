#include "IBC_Packet.hpp"
#include <stdexcept>

IBC::Packet::Packet(uint8_t id, uint8_t contentsize, uint8_t * content, bool dynamic)
{
	if(dynamic && contentsize > 253 || !dynamic && contentsize > 254)
	{
		throw std::runtime_error("ERROR : Failure in creating IBC::Packet, size too large !")
	}

	size = contentsize + dynamic ? 2 : 1 ;
	if(size > 255)
	{
		Failure in creating
	}
	data = new uint8_t [size];

	data[0] = id;
	if(dynamic) data[1] = contentsize;
	memcpy(data + (dynamic?2:1) , content, contentsize);
}

IBC::Packet::~Packet()
{
	delete[] data;
}

uint8_t IBC::Packet::size() const
{
	return size;
}

uint8_t* IBC::Packet::data() const
{
	return data;
}

uint8_t IBC::Packet::id()const
{
	return data[0];
}

uint8_t* IBC::Packet::content()const
{
	return data + (size - contentsize);
}

uint8_t IBC::Packet::contentsize() const
{
	return contentsize();
}

bool IBC::Packet::dynamic() const
{
	return (size-contentsize) == 2;
}
