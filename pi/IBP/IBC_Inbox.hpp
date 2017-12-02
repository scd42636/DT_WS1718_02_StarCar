#ifndef IBC_INBOX_HPP
#     define IBC_INBOX_HPP

#include <set>
#include <list>
#include <vector>
#include <cstdint>
#include <memory>
#include "IBC_Packet.hpp"
#include "IBC_Transceiver.hpp"

	class Inbox : public std::list<std::shared_ptr<const Packet>>
	{
		friend class Transceiver;
		//this will be necessary mainly in case the Transceiver object at *t is destroyed
		Transceiver* t;
		std::set<uint8_t> listening;

	public:
		Inbox(Transceiver& t);
		Inbox(Transceiver& t, uint8_t id);
		Inbox(Transceiver& t, std::vector<uint8_t> ids);
		~Inbox();

		Inbox(const Inbox&);
		Inbox& operator= (const Inbox&);

		//for usage :
		//front()
		//and pop_front()
		//of std::list

		void listen (uint8_t id);
		void listen (std::vector<uint8_t> ids);

		const std::set<uint8_t>& listeningto()const;

		void mute (uint8_t id);
		void mute (std::vector<uint8_t> ids);
	};
#endif /* IBC_INBOX_HPP */
