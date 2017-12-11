
#ifndef IBC_HPP
#     define IBC_HPP

#include <string>
#include "IBC_Packet.hpp"
#include "IBC_Transceiver.hpp"
#include "IBC_Inbox.hpp"

// a static message size value of IBC_SIZE_DYNAMIC in truth means the message has dynamic size
#include "IBC_defines.hpp"
#define IBC_SIZE_DYNAMIC IBC_RULE_DYNAMICSIZE

class IBC
{
	Rule rule;

	Transceiver t;

public:
	/**
	 * @brief Construct Interboard Communication
	 *
	 * @param device		Name of the file which represents the serial port on your OS
	 * @param configfile	name of the IBC_config file which states all Commandsizes and IDs
	 */
	IBC(std::string device, std::string configfile);

	/**
	 * @brief Send a Package of data
	 *
	 * @param p				Packet to send
	 */
	void send(Packet& p);


	/**
	 * @brief Retrieve an Inbox to receive Messages.
	 *
	 * @return Inbox object, which does not listen to any incoming Packets.
	 */
	Inbox getInbox();

	/**
	 * @brief Retrieve an Inbox to receive Messages.
	 *
	 * @param id	A single Message-ID the Inbox will listen to.
	 *
	 * @return Inbox object, which will listen to Messages with ID from parameter.
	 */
	Inbox getInbox(uint8_t id);


	/**
	 * @brief Retrieve an Inbox to receive Messages.
	 *
	 * @param ids	A vector-container of Message-IDs the Inbox will listen to.
	 *
	 * @return Inbox object, which will listen to Message with IDs from parameter
	 */
	Inbox getInbox(std::vector<uint8_t> ids);

	/**
	 * @brief Get the specified size of a request.
	 *	Values will be same as specified in you config file.
	 *
	 * @param id	The id of the request you want the information on.
	 *
	 * @return		The size of a Request specified. If 0 it possibly is not set or indeed 0. If 255 it is specified as a dynamic size.
	 */
	uint8_t requestsize(uint8_t id)const;
	
	/**
	 * @brief Get the specified size of a response.
	 *	Values will be same as specified in you config file.
	 *
	 * @param id	The id of the response you want the information on.
	 *
	 * @return		The size of a response specified. If 0 it possibly is not set or indeed 0. If 255 it is specified as a dynamic size.
	 */
	uint8_t responsesize(uint8_t id)const;
};
#endif /* IBC_HPP */
