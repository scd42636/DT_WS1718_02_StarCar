#include <stdio.h>
#include "serialConnection.h"
#include <fcntl.h> 
#include <unistd.h> 

int main(int argc, char **argv)
{	
	int fd = openSerialPorttoArduino();
	
	sendDatatoArduino(fd);
	readDatafromArduino(fd);
	
	close(fd);
	
	return 0;
}
