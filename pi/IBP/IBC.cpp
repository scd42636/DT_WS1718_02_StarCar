
#include "IBC.hpp"

#include <fstream>
#include <iostream>
#include <regex>

IBC::IBC(std::string configfile)
:
	properties{0}
{
	reloadcfg(configfile);
}

IBC::~IBC()
{}

void IBC::reloadcfg(std::string configfile)
{
	std::ifstream ifs (configfile);

	std::string line;

	std::regex e ("[[:space:]]*[[:digit:]]+[[:space:]]+[[:digit:]]+[[:space:]]+[[:digit:]]+[[:space:]]*");

	unsigned int linecounter = 0 ; 
	while(std::getline(ifs , line))
	{
		linecounter++;
		//check comment
		if(line[0] == '#') continue;
		//check empty

		if(!regex_match(line, e)) continue;

		std::istringstream iss (line);
		
		int id, req, answ;
		
		if(!(iss >> id >> req >> answ)) 
		{
			std::cerr << "IBC_config critical read error in line " << linecounter << " !\n";
			break; // error
		}

		//out of bound fehlererkennung
		if( id < 0 || id > 255 || req < 0 || req > 255 || answ < 0 || answ > 255 )
		{
			std::cerr << "IBC_config [Line " << linecounter << "]: Number out of bound !\n";
		}

		//write values
		properties[id].reqsize = req;
		properties[id]. anssize = answ;
	}
}

uint8_t IBC::requestsize(uint8_t id) const
{
	return properties[id].reqsize;
}

uint8_t IBC::answersize(uint8_t id) const
{
	return properties[id].anssize;
}
