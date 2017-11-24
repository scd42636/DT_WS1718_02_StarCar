
#ifndef IBC_HPP
#     define IBC_HPP

#include <string>
#include "IBC_Packet.hpp"
#include "IBC_Transceiver.hpp"
#include "IBC_Inbox.hpp"

// a static message size value of IBC_SIZE_DYNAMIC in truth means the message has dynamic size
#define IBC_SIZE_DYNAMIC 255

class IBC
{
	IBC::Transceiver t;

public:
	IBC(std::string device, std::string configfile);

	Packet makePacket(uint8_t id , void * content, uint8_t dyncontentsize = 0 ) const;

	void send(Packet& p);

	Inbox getInbox();
	Inbox getInbox(uint8_t id);
	Inbox getInbox(std::vector<uint8_t> ids);
};
#endif /* IBC_HPP */
