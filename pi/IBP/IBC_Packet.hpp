#ifndef IBC_PACKET_HPP
#     define IBC_PACKET_HPP

#define IBC_PACKET_SIZE_DYNAMIC 255

namespace IBC
{
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

		//move
		Packet(Packet &&):

		uint8_t id() const;
		uint8_t contentsize() const;
		uint8_t * content() const;

		private:

	};
}	//namespace IBC
#endif /* IBC_PACKET_HPP */
