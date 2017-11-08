
#include "Serial.hpp"

#include <stdexcept>
#include <fcntl.h>   	// File Control Definitions 
#include <termios.h> 	// POSIX Terminal Control Definitions
#include <unistd.h>  	// UNIX Standard Definitions
#include <errno.h>   	// ERROR Number Definitions


Serial::Serial(std::string devicename)
	:
		fd(-1)
{

   	fd = open(devicename.c_str(),O_RDWR | O_NOCTTY | O_NDELAY);		/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
																/* O_RDWR Read/Write access to serial port           */
																/* O_NOCTTY - No terminal will control the process   */
																/* O_NDELAY -Non Blocking Mode,Does not care about-  */
																/* -the status of DCD line,Open() returns immediatly */                                        
									
    if(fd == -1)
	{
		throw std::runtime_error("openSerialPortArduino: Unable to open " + devicename);	
	}

	//existence of file descriptor is assured
	config();
}

Serial::~Serial()
{
	
}

void Serial::config()
{
	
   	
	//---------- Setting the Attributes of the serial port using termios structure ---------
		
	struct termios SerialPortSettings;

	tcgetattr(fd, &SerialPortSettings);			// Get the current attributes of the Serial port 

	cfsetispeed(&SerialPortSettings,B9600); 	// Set Read Speed as 9600                       
	cfsetospeed(&SerialPortSettings,B9600);		// Set Write Speed as 9600

	SerialPortSettings.c_cflag &= ~PARENB;   	// Disables the Parity Enable bit(PARENB),So No Parity
	SerialPortSettings.c_cflag &= ~CSTOPB;   	// CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit
	SerialPortSettings.c_cflag &= ~CSIZE;	 	// Clears the mask for setting the data size            
	SerialPortSettings.c_cflag |=  CS8;      	// Set the data bits = 8                                 
	
	SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control                         
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines 
		
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both i/p and o/p 
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode                           

	SerialPortSettings.c_oflag &= ~OPOST;							// No Output Processing

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 			// Set the attributes to the termios structure
	
		printf("openSerialPortArduino: in Setting attributes \n");
		
	}else{
    
		printf("BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none \n");
	}	
	       
	sleep(3);	// Wait until Arduino restarts !!
	//TODO check and test if this sleep is really necessary and if it is really a restart
	//tcflush(fd, TCIFLUSH);   					// Discards old data in the buffer
	
}

int Serial::send(const void * data , int size) const
{
	
	return write(fd, data, size);	// use write() to send data to port                                            
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
	
	tcflush(fd, TCIFLUSH);   					// Discards old data in the buffer

	return bytes_read;
}
