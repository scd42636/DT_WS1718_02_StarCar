
#include "IBC.hpp"

#include <iostream>
int main (int argc , char** argv)
{
	IBC ibc("IBC_config.cfg");

	for(int i = 0; i < 256 ; i++)
	{
		std::cout << i << " : " << (int) ibc.requestsize(i) << " | " << (int) ibc.answersize(i) << '\n';
	}

	return 0;
}
