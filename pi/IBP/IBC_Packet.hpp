#ifndef IBC_PACKET_HPP
#     define IBC_PACKET_HPP

#define IBC_PACKET_SIZE_DEFAULT 0

namespace IBC
{

class Packet
{
	uint8_t size;
	uint8_t * data;

public:
	Packet(uint8_t id, uint8_t * data, uint8_t contentsize = IBC_PACKET_NOSIZE);
	Packet(uint8_t * data);
	~Packet();

	//Copy constructor and assignment
	Packet(const Packet&);
	Packet& operator= (const Packet&);

	/**
	 * @brief Get access to the content data of the packet.
	 *	Note : Watch the size of the packet contents you can get with size()
	 *	Note : Also watch if the owner of the Packages data does exist or the data might be corrupted
	 *
	 * @return Pointer to the beginning of content-data 
	 */
	uint8_t* content() const;

	uint8_t id() const;

	/**
	 * @return 16Bit integer with number of data in bytes
	 */
	uint8_t size() const;

	uint8_t contentsize() const;

	/**
	 * @brief Computes a 8Bit long hash value from the contents of the packet.
	 *
	 * @return hash value
	 */
	uint8_t hash() const;

	/**
	 * @brief Set the hash value in the packet.
	 *
	 * @param nhash The new hash value (per default the value hash() returns)
	 */
	void sethash(uint8_t nhash = hash());

	/**
	 * @brief Checks the correctness of the hash.
	 *	Can be used to detect if a received package has been damaged in transmission.
	 * @return true/false whether hash is correct or not
	 */
	bool verify() const;
};

}

#endif /* IBC_PACKET_HPP */
