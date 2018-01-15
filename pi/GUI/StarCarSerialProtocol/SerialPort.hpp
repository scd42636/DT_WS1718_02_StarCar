#ifndef SERIAL_HPP
#     define SERIAL_HPP
	
#include <string>

class SerialPort
{
	int fd;

public:

    SerialPort(std::string devicename);
    ~SerialPort();

    int send (const void * data, int size) const;
	int recv (void * data, int maxsize) const;
    void emptyRecvBuffer() const;

private:

	void config();
};

#endif /* SERIAL_HPP */


