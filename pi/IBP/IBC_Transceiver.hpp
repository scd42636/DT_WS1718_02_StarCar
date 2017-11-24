#ifndef IBC_TRANSCEIVER_HPP
#     define IBC_TRANSCEIVER_HPP

#define IBC_TRANSCEIVER_IDLE_TIME 15 //in ms

#include <thread>
#include <queue>
#include <vector>
#include <set>
#include <functional>

#include "Serial.hpp"
#include "IBC_Inbox.hpp"
#include "IBC_Packet.hpp"
#include "IBC_Rule.hpp"
	

class Transceiver
{
	Serial s;

	bool running;
	void body();
	std::thread worker;

	/**
	 * Heap for storing Packages to send
	 * Some complicated stuff so bear with me :
	 * priority_queue = heap
	 * 1 Template arg : Packet , the type the heap stores
	 * 2 Template arg : std::vector<Packet> , the container which stores the heaps objects
	 * 3 Template arg : a comparison function which orders the heap (here in form of a lamba expression)
	 */
	std::function<bool(const Packet& , const Packet&)> more_important_packet_comparator = 
		[](const Packet & lhs , const Packet & rhs)
		{ return lhs.id() > rhs.id(); };

	typedef std::priority_queue	<	const Packet ,																		//type
							std::vector<const Packet >,															//container
							decltype(more_important_packet_comparator)
						>								//comparison
						Packetqueue;
	
	Packetqueue tosend (more_important_packet_comparator);	


	//TODO OPT vectors dynamically on heap and only if needed	 
	std::set<Inbox*> receivers [256];

	//stores the current status the transceiver is in (errors and stuff)
	uint8_t status;

	const Rule& rule;
public:

	Transceiver(std::string device,const IBC::Rule & rule);
	~Transceiver();

	/**
	 * @brief Starts the worker thread for sending and receiving
	 *
	 * @param running You can set this value and the thread will start/stop according to it . true = thread is truly running
	 */
	void runworker(bool running = true);

	/**
	 * @brief Queues you package for sending.
	 *	The actual sending will be done by a worker thread.
	 *	Queue orders your Packet for priority (id)
	 * @param A reference to the Packet you want to send. Will be copied, so you can safely destroy your version of it.
	 */
	void send(IBC_Packet &) const;

	void addreceiver(const Inbox& i, uint8_t id);
	void removereceiver(const Inbox& i, uint8_t id);

private:


	void store (std::shared_ptr<const Packet>&);

};

#endif /* IBC_TRANSCEIVER_HPP */
