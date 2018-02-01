#include "IBC.test.h"

#include <iostream>

int main (int argc , char** argv)
{
	IBC ibc;

	ibc.m_INMID = 20;
	ibc.m_INSTAT = 0x08;
	ibc.m_INSIZE_DYN = 2;
	ibc.m_INDH = 0;

	std::cout << ibc.inMID() << " = " << ibc.m_INMID() << "|" << ibc.inMID()==ibc.m_INMID() << '\n';
	std::cout << ibc.inStatbyte() << " = " << ibc.m_INMID << "|" << ibc.inStatbyte() == ibc.m_INSTAT << '\n';
	std::cout << ibc.inSIZE_DYN() << " = " << ibc.m_INSIZE_DYN << "|" << ibc.inSIZE_DYN() == ibc.m_INSIZE_DYN << '\n';
	std::cout << std::hex << (unsigned int) ibc.inSTAT() << "=" << (unsigned in) ibc.m_INSTAT>>4 << "|" << ibc.inSTAT() == ibc.m_INSTAT>>4 << '\n';

	return 0;
}
