#ifndef IBC_TRANSCEIVER_HPP
#     define IBC_TRANSCEIVER_HPP

#define IBC_TRANSCEIVER_IDLE_TIME 15 //in ms

#include <thread>
#include <queue>
#include <vector>
#include <functional>

#include "Serial.hpp"

namespace IBC
{
	
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
	 * 2 Template arg : std::vector<packet> , the container which stores the heaps objects
	 * 3 Template arg : a comparison function which orders the heap (here in form of a lamba expression)
	 */
	std::priority_queue	<	Packet ,																		//type
							std::vector<Packet>,															//container
							std::function<bool(const Packet&,const packet&)>								//comparison
						>	tosend																			//queue name
							([](const Packet & lhs , const Packet & rhs){return lhs.id() > rhs.id();}		//comparison code as parameter lambda

	

public:
	Transceiver();
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



private:

	void store (std::shared_ptr<Packet>&);

};

}//namespace IBC
#endif /* IBC_TRANSCEIVER_HPP */
