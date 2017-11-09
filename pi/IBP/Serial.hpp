#ifndef SERIAL_HPP
#     define SERIAL_HPP
	
#include <string>

class Serial
{
	int fd;

public:
	Serial(std::string devicename);
	~Serial();

	int send (const void * data, int size) const;
	int recv (void * data, int maxsize) const;
private:
	void config();
};

#endif /* SERIAL_HPP */


