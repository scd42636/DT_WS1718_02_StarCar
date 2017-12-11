#ifndef IBC_PACKET_HPP
#     define IBC_PACKET_HPP

#include "IBC_defines.hpp"

#define IBC_PACKET_SIZE_DYNAMIC IBC_RULE_DYNAMICSIZE

#include <cstdint>

	class Packet
	{
		uint8_t m_id;
		uint8_t m_size;
		uint8_t * m_content;
		public:
		Packet(uint8_t id, uint8_t contentsize);
		Packet(uint8_t id, uint8_t contentsize, uint8_t * content);
		~Packet();

		//copy
		Packet(const Packet&);
		Packet& operator=(const Packet&);

		//move
		Packet(Packet &&);
		Packet& operator=(Packet&&);

		uint8_t id() const;
		uint8_t contentsize() const;
		uint8_t * content() const;
	};
#endif /* IBC_PACKET_HPP */
