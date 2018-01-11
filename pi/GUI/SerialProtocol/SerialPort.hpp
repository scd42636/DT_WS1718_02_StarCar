#ifndef SERIALPORT_HPP
#     define SERIALPORT_HPP
	
#include <string>

class SerialPort
{

private:

	int fd;

public:

    SerialPort(std::string);
    ~SerialPort();
    void config();
    int getFD();
};

#endif /* SERIALPORT */


