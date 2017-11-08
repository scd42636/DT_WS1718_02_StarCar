/*==========================================================================*/
/* serialConnection.c 														*/
/* StarCar																	*/
/*==========================================================================*/


#include <stdio.h>
#include <fcntl.h>   	// File Control Definitions           
#include <termios.h> 	// POSIX Terminal Control Definitions 
#include <unistd.h>  	// UNIX Standard Definitions 	    
#include <errno.h>   	// ERROR Number Definitions


int openSerialPorttoArduino (){
	
	int fd;		/*File Descriptor*/
		
	printf("\n +-----------------------------------+");
	printf("\n |      Open Serial Port Arduino     |");
	printf("\n +-----------------------------------+\n");

	//------------------------------- Opening the Serial Port -------------------------------

    fd = open("/dev/ttyUSB0",O_RDWR | O_NOCTTY | O_NDELAY);		/* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
																/* O_RDWR Read/Write access to serial port           */
																/* O_NOCTTY - No terminal will control the process   */
																/* O_NDELAY -Non Blocking Mode,Does not care about-  */
																/* -the status of DCD line,Open() returns immediatly */                                        
									
    if(fd == -1){
    
		printf("openSerialPortArduino: Unable to open /dev/ttyUSB0 \n");
		return -1;
		
	}else {
            	  
		printf("openSerialPortArduino: /dev/ttyUSB0 Opened Successfully \n");

	}
	
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
	//tcflush(fd, TCIFLUSH);   					// Discards old data in the buffer
	
	return fd;
}

int sendDatatoArduino(int fd) {
	
	char write_buffer[] = "12345678910";	// Buffer containing characters to write into port
	int  bytes_written  = 0;  	// Value for storing the number of bytes written to the port 

	bytes_written = write(fd,write_buffer,sizeof(write_buffer));	// use write() to send data to port                                            
																	// "fd"                   - file descriptor pointing to the opened serial port 
																	// "write_buffer"         - address of the buffer containing data	            
																	// "sizeof(write_buffer)" - No of bytes to write                               	
	printf("sendDatatoArduino: %s written to Arduino \n", write_buffer);
	printf("sendDatatoArduino %d Bytes written to Arduino \n", bytes_written);
	return 0;
}

void readDatafromArduino(int fd){
	
	char read_buffer[32];   // Buffer to store the data received              
	int  bytes_read = 0;    // Number of bytes read by the read() system call 
 	int i = 0;

	bytes_read = read(fd,&read_buffer,32); 		// Read the data 
	
	printf("Bytes read - %d\n", bytes_read); 		// Print the number of bytes read

	for(i=0;i<bytes_read;i++){					// printing only the received characters
		    
		printf("%c\n",read_buffer[i]);
		
	}
	
	tcflush(fd, TCIFLUSH);   					// Discards old data in the buffer
	
}	
