
#include "Serial.hpp"

int main (int argc , char** argv)
{
	Serial s("/dev/ttyUSB0");

	char b;

	b=252;
	s.send(&b,1);
	b=3;
	s.send(&b,1);
	b = 0;
	s.send(&b,1);
	b = 1;
	s.send(&b,1);
	b=2;	
	s.send(&b,1);
	b=3;
	s.send(&b,1);
	b=0;
	s.send(&b,1);

	return 0;
}
