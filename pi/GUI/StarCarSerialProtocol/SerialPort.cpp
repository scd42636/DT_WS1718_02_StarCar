#include "SerialPort.hpp"


SerialPort::SerialPort(std::string devicename)
    :
        fd(-1)
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

    config();
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

int SerialPort::send(const void * data , int size) const
{

        return write(fd, data, size);	// use write() to send data to port
                                        // "fd"                   - file descriptor pointing to the opened serial port
                                        // "write_buffer"         - address of the buffer containing data
                                        // "sizeof(write_buffer)" - No of bytes to write
                                        // returns the actual bytes written to the port
}

int SerialPort::recv(void * data , int maxsize) const
{

    return read(fd, data , maxsize); 		// Read the data
                                                // Write it to the location at <data>
                                                // Write <maxsize> bytes at maximum to not overrun buffersizes
}

void SerialPort::emptyRecvBuffer() const
{
	char buff [20];

	int i=0;
	while (i < 10000) {
		read(fd, buff, 20);
	}
}
