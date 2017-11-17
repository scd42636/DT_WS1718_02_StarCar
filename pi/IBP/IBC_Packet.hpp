#ifndef IBC_PACKET_HPP
#     define IBC_PACKET_HPP

#define IBC_PACKET_SIZE_DYNAMIC 255

namespace IBC
{
	class Packet
	{
		friend class IBC;
		uint8_t * data;
		uint8_t size;
		uint8_t contentsize;

		Packet(unsigned int id, uint8_t contentsize, uint8_t * content , bool dynamic = false);
	public:
		~Packet();
		
		uint8_t size() const;
		uint8_t * data () const;

		uint8_t id () const;
		uint8_t * content() const;
		uint8_t contentsize() const;
	};
}	//namespace IBC
#endif /* IBC_PACKET_HPP */
