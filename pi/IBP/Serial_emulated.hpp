#ifndef SERIAL_HPP
#     define SERIAL_HPP
	
#include <string>

#include <vector>

class Serial
{
	std::vector<char> sendbuff;
	std::vector<char> recvbuff;
	

public:
	/**
	 * @brief Constructs a Serial object.
	 *	The object manages a Serial connection file descriptor for handling I/O
	 *
	 * @param devicename the filename of the serial port device
	 *		example :
	 *		Serial s("/dev/ttyUSB0");
	 */
	Serial(std::string devicename);


	/**
	 * @brief closes file descriptor
	 */
	~Serial();

	/**
	 * @brief Sends data.
	 *	Sends data using the specified serial device.
	 * @param data	points to memory location where data is to be sent
	 * @param size	the size of bytes beginning from <data> to send
	 *
	 * @return the number of bytes actually sent by the function (non blocking)
	 */
	int send (char * data, int size);


	/**
	 * @brief Receives data.
	 *	Receives data using the specified serial device.
	 * @param data	Memory location where the received message can be written to
	 * @param maxsize	Maximum size of bytes to read. This can protect your memory around your buffer.
	 *
	 * @return the number of bytes actually received
	 */
	int recv (char * data, int maxsize);
private:
	/**
	 * @brief configures the Serial port for data transmission
	 */
	void config();
};

#endif /* SERIAL_HPP */


