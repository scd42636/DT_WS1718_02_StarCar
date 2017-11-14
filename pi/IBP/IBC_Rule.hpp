
#ifndef IBC_RULE_HPP
#     define IBC_RULE_HPP

#include <string>


// a static message size value of Rule_SIZE_DYNAMIC in truth means the message has dynamic size
#define IBC_RULE_SIZE_DYNAMIC 255

namespace IBC
{

class Rule
{
	struct message_prop
	{
		uint8_t reqsize, anssize;
	};
	
	//an array of message properties
	message_prop properties [255];


public:
	Rule(std::string configfile);
	~Rule();

	void reloadcfg(std::string configfile);

	uint8_t requestsize(uint8_t id) const;
	uint8_t answersize(uint8_t id) const;

private:

};	

} //namespace IBC
#endif /* Rule_HPP */
