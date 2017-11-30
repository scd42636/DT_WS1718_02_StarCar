#ifndef IBC_TRANSCEIVER_HPP
#     define IBC_TRANSCEIVER_HPP

#define IBC_TRANSCEIVER_IDLE_TIME 15ms //using std::chrono_literals this syntax is possible

#include <thread>
#include <queue>
#include <vector>
#include <set>
#include <functional>

#ifndef IBC_EMULATED
	#include "Serial.hpp"
#else
	#include "Serial_emulated.hpp"
#endif

#include "IBC_Packet.hpp"
#include "IBC_Rule.hpp"

class Inbox;

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
	std::priority_queue	<	Packet ,																		//type
							std::vector<Packet >,															//container
							std::function<bool(Packet&,Packet&)>
						>
						tosend;

	//TODO OPT vectors dynamically on heap and only if needed	 
	std::set<Inbox*> receivers [256];

	//stores the current status the transceiver is in (errors and stuff)
	uint8_t status;

	const Rule& rule;
public:

	Transceiver(std::string device,const Rule & rule);
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
	void send(Packet &);

	void addreceiver(Inbox& i, uint8_t id);
	void removereceiver(Inbox& i, uint8_t id);

private:
	void recv_intern(uint8_t*, uint8_t);
	void send_intern(uint8_t*, uint8_t);

	uint8_t sizehash(uint8_t size) const;
	uint8_t datahash(uint8_t * data, uint8_t length, uint8_t in = 0) const;
	uint8_t headhash_request(uint8_t* headerbegin) const;
	uint8_t headhash_response(uint8_t* headerbegin) const;
	void padd(uint8_t * begin, uint8_t paddinglength);

	void store (std::shared_ptr<const Packet>&);
};

#endif /* IBC_TRANSCEIVER_HPP */
