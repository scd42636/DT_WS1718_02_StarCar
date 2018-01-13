#include "SerialPort.hpp"

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fcntl.h>   	// File Control Definitions
#include <termios.h> 	// POSIX Terminal Control Definitions
#include <unistd.h>  	// UNIX Standard Definitions
#include <errno.h>   	// ERROR Number Definitions
#include <string.h> 	// ERROR


SerialPort::SerialPort(std::string devicename) : fd(-1)
{
    fd = open(devicename.c_str(),O_RDWR | O_NOCTTY); // ttyUSB0 is the connected arduino or ACM0
                                                                        // O_RDWR Read/Write access to serial port
                                                                        // O_NOCTTY - Serial port should not kill the process is a ctrl-c is received
                                                                        // O_NDELAY - Non Blocking Mode

    if(fd == -1){                                                        // existence of file descriptor is assured

        throw std::runtime_error("openSerialPortArduino: Unable to open " + devicename);
    }

    usleep(200000);             // wait some time ... otherwise it will cause problems with the serial port
    tcflush(fd, TCIOFLUSH);	// clar input / output buffer and discard old data
}

SerialPort::~SerialPort()
{
    close(fd);
}

void SerialPort::config()
{

    //---------- Setting the Attributes of the serial port using termios structure ---------

    // Info zu Membern in termios
    /*struct termios
      {
       Bitmaske für die Eingabe-Flags          tcflag_t c_iflag
       Bitmaske für die Ausgabe-Flags          tcflag_t c_oflag
       Bitmaske für die Control-Flags          tcflag_t c_cflag
       Bitmaske für lokale Einstellungen       tcflag_t c_lflag
       line discipline                         char    __c_line
       Array für Sonderzeichen/-funktionen     cc_t c_cc[NCCS]
      }*/

    struct termios SerialPortSettings;

    tcgetattr(fd, &SerialPortSettings);             // Get the current attributes of the Serial port

    cfsetispeed(&SerialPortSettings,B115200);        // Set Read Speed as 115200
    cfsetospeed(&SerialPortSettings,B115200);        // Set Write Speed as 115200

    SerialPortSettings.c_cflag &= ~PARENB;          // Disables the Parity Enable bit(PARENB),So No Parity
    SerialPortSettings.c_cflag &= ~CSTOPB;          // CSTOPB = 2 Stop bits, here it is cleared so 1 Stop bit
    SerialPortSettings.c_cflag &= ~CSIZE;           // Clears the mask for setting the data size ... enables set own data bits... see next line
    SerialPortSettings.c_cflag |=  CS8;             // Set the data bits = 8
    SerialPortSettings.c_cflag |= (CREAD | CLOCAL); // Enable receiver,Ignore Modem Control lines

    cfmakeraw(&SerialPortSettings);                 // Setup Raw-Mode automatically

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 	// Set the attributes to the termios structure

        std::cout << "Serial.config: Error in settting attributes" << std::endl;

    }else{

        std::cout << "SerialPort is ready! BaudRate = 115200" << std::endl;
    }

        sleep(3);   // Wait until Arduino restarts !!
}

int SerialPort::getFD(){

    return this->fd;
}
