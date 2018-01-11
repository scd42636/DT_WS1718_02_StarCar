#ifndef SERIALPORT_HPP
#     define SERIALPORT_HPP
	
#include <string>

class SerialPort
{
public:
	int fd;

public:

    SerialPort(std::string);
    ~SerialPort();
    void config();
};

#endif /* SERIALPORT */


