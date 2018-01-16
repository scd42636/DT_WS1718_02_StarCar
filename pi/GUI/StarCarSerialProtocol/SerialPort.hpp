#ifndef SERIAL_HPP
#     define SERIAL_HPP
	
#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fcntl.h>   	// File Control Definitions
#include <termios.h> 	// POSIX Terminal Control Definitions
#include <unistd.h>  	// UNIX Standard Definitions
#include <errno.h>   	// ERROR Number Definitions
#include <string.h> 	// ERROR

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


