#include "IBC.test.h"

#include <iostream>

int main (int argc , char** argv)
{
	IBC ibc;

	ibc.m_INMID = 20;
	ibc.m_INSTAT = 0x08;
	ibc.m_INSIZE_DYN = 2;
	ibc.m_INDH = 0;

	ibc.m_STAT = 0;


	std::cout << "MID " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.inMID() << " = " <<  (unsigned int) ibc.m_INMID << " | " << (ibc.inMID()==ibc.m_INMID) << '\n';
	
	std::cout << "Statusbyte " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.inStatbyte() << " = " <<  (unsigned int) ibc.m_INSTAT << " | " <<  (ibc.inStatbyte() == ibc.m_INSTAT) << '\n';
	
	std::cout << "Dynamic Size " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.inSIZE_DYN() << " = " <<  (unsigned int) ibc.m_INSIZE_DYN << " | " << (ibc.inSIZE_DYN() == ibc.m_INSIZE_DYN) << '\n';
	
	std::cout << "Status IN" << '\n';
	std::cout << std::hex << (unsigned int) ibc.inSTAT() << " = " << (unsigned int) (ibc.m_INSTAT>>4) << " | " << (ibc.inSTAT() == (ibc.m_INSTAT>>4)) << '\n';

	std::cout << "Sizehash IN " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.inSH() << " = " <<  (unsigned int) ((ibc.m_INSTAT >> 2) & 0x03) << " | " << (ibc.inSH() == ((ibc.m_INSTAT >> 2) & 0x03)) << '\n';

	std::cout << "Headhash IN " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.inHH() << " = " <<  (unsigned int) (ibc.m_INSTAT & 0x03)  << " | " <<  (ibc.inHH() == (ibc.m_INSTAT & 0x03)) << '\n';

	std::cout << "Datahash IN " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.inDH() << " = " <<  (unsigned int) (ibc.m_INDH)  << " | " <<  (ibc.inDH() == (ibc.m_INSTAT)) << '\n';

	std::cout << "Status THIS " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.STAT() << " = " <<  0  << " | " <<  (ibc.STAT() == 0) << '\n';

	std::cout << "Requestheadhash " << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.HH_request() << " = " << (unsigned int) (((ibc.m_INMID ^ (ibc.m_INMID<<2)^ (ibc.m_INMID<<4)^ (ibc.m_INMID<<6)^ (ibc.m_INSTAT)^ (ibc.m_INSTAT<<2))>>6)&0x03) << " | " <<  (  ibc.HH_request() ==(((ibc.m_INMID ^ (ibc.m_INMID<<2)^ (ibc.m_INMID<<4)^ (ibc.m_INMID<<6)^ (ibc.m_INSTAT)^ (ibc.m_INSTAT<<2))>>6)&0x03) ) << '\n';

	std::cout << "Datahash " << '\n';
	
		byte buff[4] = {0,1,2,3};
		std::cout << std::hex << (unsigned int) ibc.createDH(buff, 4) << " = " << 0 << " | " << (ibc.createDH(buff, 4) == 0) << '\n';	
	

	std::cout << "Statbyte" << '\n';
	std::cout << std::hex <<  (unsigned int) ibc.Statbyte() << " = " <<  0x0  << " | " <<  (ibc.Statbyte(0)  == 0x0) << '\n';
	
	std::cout << "Hashchecks : inHH , inSH, inDH " << '\n';
	std::cout << ibc.checkinHH() << " : " << ibc.checkinSH() << " : " << ibc.checkinDH(0) << '\n';

	std::cout << "DH Setter and getter " << '\n';
	ibc.setDH(2);
	std::cout << (ibc.m_DH == 2) << " : " << (ibc.DH() == 2) << '\n';


	return 0;
}

	
