
#ifndef IBC_HPP
#     define IBC_HPP

#include <string>


// a static message size value of IBC_SIZE_DYNAMIC in truth means the message has dynamic size
#define IBC_SIZE_DYNAMIC 255

class IBC
{
	struct message_prop
	{
		uint8_t reqsize, anssize;
	};
	
	//an array of message properties
	message_prop properties [255];


public:
	IBC(std::string configfile);
	~IBC();

	void reloadcfg(std::string configfile);

	uint8_t requestsize(uint8_t id) const;
	uint8_t answersize(uint8_t id) const;

private:

};	
#endif /* IBC_HPP */
