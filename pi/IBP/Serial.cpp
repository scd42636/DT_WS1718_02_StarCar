
#include "Serial.hpp"

#include <stdio.h>
#include <iostream>
#include <stdexcept>
#include <fcntl.h>   	// File Control Definitions 
#include <termios.h> 	// POSIX Terminal Control Definitions
#include <unistd.h>  	// UNIX Standard Definitions
#include <errno.h>   	// ERROR Number Definitions
#include <string.h> 	// ERROR 


Serial::Serial(std::string devicename)
	:
		fd(-1)
{

   	fd = open(devicename.c_str(),O_RDWR | O_NOCTTY /*| O_NDELAY*/); // ttyUSB0 is the connected arduino
																// O_RDWR Read/Write access to serial port
																// O_NOCTTY - Serial port should not kill the process is a ctrl-c is received
																// O_NDELAY - Non Blocking Mode                                       
									
    if(fd == -1)
	{
		throw std::runtime_error("openSerialPortArduino: Unable to open " + devicename);	
	}

	usleep(200000);												// wait some time ... otherwise it will cause problems with the serial port
	tcflush(fd, TCIOFLUSH);										// clar input / output buffer and discard old data
																// existence of file descriptor is assured
	config();
	
	send("HelloMSG",8);											// send hello message to arduino
	char buffer[9];
	buffer[8] = '\0';
	
	std::cout << "|       Sending hello message to arduino...          |" << std::endl;
	
	if (recv(buffer, 8) == 8){
	
		std::cout << "|        hello message received from arduino         |" << std::endl;
		std::cout << "|              Serial Port is ready !!               |" << std::endl;
		std::cout << "+----------------------------------------------------+" << std::endl;	
	}else{
	
		std::cout << "Serial.recv: Error receiving data with error number: " << errno << " Error message: "<< strerror(errno) << std::endl;
		std::cout << "+----------------------------------------------------+" << std::endl;
	}
}

Serial::~Serial()
{
	close(fd);
}

void Serial::config()
{
	
   	
	//---------- Setting the Attributes of the serial port using termios structure ---------
	
	std::cout << "+----------------------------------------------------+" << std::endl;
	std::cout << "|              Open Serial Port Arduino              |" << std::endl;
	std::cout << "+----------------------------------------------------+" << std::endl;
		
	struct termios SerialPortSettings;

	tcgetattr(fd, &SerialPortSettings);			// Get the current attributes of the Serial port 

	cfsetispeed(&SerialPortSettings,B57600); 	// Set Read Speed as 57600                    
	cfsetospeed(&SerialPortSettings,B57600);	// Set Write Speed as 57600

	SerialPortSettings.c_cflag &= ~PARENB;   	// Disables the Parity Enable bit(PARENB),So No Parity
	SerialPortSettings.c_cflag &= ~CSTOPB;   	// CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit
	SerialPortSettings.c_cflag &= ~CSIZE;	 	// Clears the mask for setting the data size ... enables set own data bits... see next line           
	SerialPortSettings.c_cflag |=  CS8;      	// Set the data bits = 8                                 
	
	SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control                         
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines 
		
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both i/p and o/p
	SerialPortSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); 
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHONL | ISIG | IEXTEN);  // Non Cannonical mode
	                           

	SerialPortSettings.c_oflag &= ~OPOST;							// No Output Processing

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 			// Set the attributes to the termios structure
	
		std::cout << "|    Serial.config: Error in settting attributes     |" << std::endl;
		
	}else{
    
		std::cout << "| BaudRate = 57600    StopBits = 1   Parity = none   |" << std::endl;
	}	
	       
	sleep(3);														// Wait until Arduino restarts !!	
}

int Serial::send(const void * data , int size) const
{
	
	return write(fd, data, size);									// use write() to send data to port                                            
																	// "fd"                   - file descriptor pointing to the opened serial port 
																	// "write_buffer"         - address of the buffer containing data	            
																	// "sizeof(write_buffer)" - No of bytes to write                               	
																	// returns the actual bytes written to the port
}

int Serial::recv(void * data , int maxsize) const
{
	
	int bytes_read = read(fd, data , maxsize); 		// Read the data
													// Write it to the location at <data>
													// Write <maxsize> bytes at maximum to not overrun buffersizes
	
	if(bytes_read < 0)
	{
		std::cout << "Serial.recv: Error receiving data with error number: " << errno << " Error message: "<< strerror(errno) << std::endl;
		}
	
	return bytes_read;
}
