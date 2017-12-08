
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
   	fd = open(devicename.c_str(),O_RDWR | O_NOCTTY | O_NDELAY);	/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
																/* O_RDWR Read/Write access to serial port           */
																/* O_NOCTTY - No terminal will control the process   */
																/* O_NDELAY -Non Blocking Mode,Does not care about-  */
																/* -the status of DCD line,Open() returns immediatly */                                        
    if(fd == -1)
	{
		throw std::runtime_error("openSerialPortArduino: Unable to open " + devicename);	
	}

    usleep(200000);             // wait some time ... otherwise it will cause problems with the serial port
    tcflush(fd, TCIOFLUSH);	// clar input / output buffer and discard old data

    config();
}

Serial::~Serial()
{
	close(fd);
}

void Serial::config()
<<<<<<< HEAD
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

    cfsetispeed(&SerialPortSettings,B57600);        // Set Read Speed as 57600
    cfsetospeed(&SerialPortSettings,B57600);        // Set Write Speed as 57600

    SerialPortSettings.c_cflag &= ~PARENB;          // Disables the Parity Enable bit(PARENB),So No Parity
    SerialPortSettings.c_cflag &= ~CSTOPB;          // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit
    SerialPortSettings.c_cflag &= ~CSIZE;           // Clears the mask for setting the data size ... enables set own data bits... see next line
    SerialPortSettings.c_cflag |=  CS8;             // Set the data bits = 8
    SerialPortSettings.c_cflag |= (CREAD | CLOCAL); // Enable receiver,Ignore Modem Control lines
=======
{
	//---------- Setting the Attributes of the serial port using termios structure ---------
>>>>>>> feat_protocoll
		
    cfmakeraw(&SerialPortSettings);                 // Setup Raw-Mode automatically

    if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 	// Set the attributes to the termios structure
	
<<<<<<< HEAD
        std::cout << "Serial.config: Error in settting attributes" << std::endl;
		
    }else{
=======
	SerialPortSettings.c_cflag &= ~CRTSCTS;       // No Hardware flow Control                         
	SerialPortSettings.c_cflag |= CREAD | CLOCAL; // Enable receiver,Ignore Modem Control lines 
		
	SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);          // Disable XON/XOFF flow control both i/p and o/p 
	SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // Non Cannonical mode                           

	SerialPortSettings.c_oflag &= ~OPOST;							// No Output Processing

	if((tcsetattr(fd,TCSANOW,&SerialPortSettings)) != 0){ 			// Set the attributes to the termios structure
		printf("openSerialPortArduino: in Setting attributes \n");
		
	}else{
>>>>>>> feat_protocoll
    
        std::cout << "SerialPort is ready! BaudRate = 57600" << std::endl;
    }
	       
<<<<<<< HEAD
        sleep(3);   // Wait until Arduino restarts !!
=======
	sleep(3);	// Wait until Arduino restarts !!
	//TODO check and test if this sleep is really necessary and if it is really a restart
	//tcflush(fd, TCIFLUSH);   					// Discards old data in the buffer
>>>>>>> feat_protocoll
}

int Serial::send(const void * data , int size) const
{
<<<<<<< HEAD
	
        return write(fd, data, size);	// use write() to send data to port
                                        // "fd"                   - file descriptor pointing to the opened serial port
                                        // "write_buffer"         - address of the buffer containing data
                                        // "sizeof(write_buffer)" - No of bytes to write
                                        // returns the actual bytes written to the port
=======
	return write(fd, data, size);	// use write() to send data to port                                            
																	// "fd"                   - file descriptor pointing to the opened serial port 
																	// "write_buffer"         - address of the buffer containing data	            
																	// "sizeof(write_buffer)" - No of bytes to write                               	
																	// returns the actual bytes written to the port
>>>>>>> feat_protocoll
}

int Serial::recv(void * data , int maxsize) const
{
<<<<<<< HEAD
	
    return read(fd, data , maxsize); 		// Read the data
                                                // Write it to the location at <data>
                                                // Write <maxsize> bytes at maximum to not overrun buffersizes
=======
	int bytes_read = read(fd, data , maxsize); 		// Read the data
											// Write it to the location at <data>
											// Write <maxsize> bytes at maximum to not overrun buffersizes
	tcflush(fd, TCIFLUSH);   					// Discards old data in the buffer

	return bytes_read;
>>>>>>> feat_protocoll
}
