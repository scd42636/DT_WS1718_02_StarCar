
#define IBC_PACKET_NOSIZE 0

class IBC_Packet
{
	uint8_t id;
	uint16_t size;
	void * data;

	uint8_t m_hash;	
	bool copy;

public:
	IBC_Packet(uint8_t id, void * data, uint16_t size = IBC_PACKET_NOSIZE, bool copy = true );
	IBC_Packet(void * data);
	~IBC_Packet();

	//Copy constructor and assignment
	IBC_Packet(const IBC_Packet&);
	IBC_Packet& operator= (const IBC_Packet&);

	/**
	 * @brief Get access to the content data of the packet.
	 *	Note : Watch the size of the packet contents you can get with size()
	 *	Note : Also watch if the owner of the Packages data does exist or the data might be corrupted
	 *
	 * @return Pointer to the beginning of content-data 
	 */
	void * content() const;

	/**
	 * @return 16Bit integer with number of data in bytes
	 */
	uint16_t size() const;

	/**
	 * @brief Computes a 8Bit long hash value from the contents of the packet.
	 *
	 * @return hash value
	 */
	uint8_t hash() const;

	/**
	 * @brief Set the hash value in the packet.
	 *
	 * @param nhash The new has value
	 */
	void sethash(uint8_t nhash = hash());

	/**
	 * @brief Checks the correctness of the hash.
	 *	Can be used to detect if a received package has been damaged in transmission.
	 * @return true/false whether hash is correct or not
	 */
	bool verify() const;
};
