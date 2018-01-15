
#include "IBC_Rule.hpp"

#include <iostream>
int main (int argc , char** argv)
{
	IBC::Rule ibc("IBC_config.cfg");

	for(int i = 0; i < 256 ; i++)
	{
		std::cout << i << " : " << (int) ibc.requestsize(i) << " | " << (int) ibc.answersize(i) << '\n';
	}

	return 0;
}
