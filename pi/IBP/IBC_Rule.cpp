
#include "IBC_Rule.hpp"

#include <fstream>
#include <iostream>
#include <regex>



Rule::Rule(std::string configfile)
{
	for(int i = 0; i < 255; i++)
	{
		properties[i].reqsize = 0;
		properties[i].anssize = 0;
	}
	reloadcfg(configfile);
}

Rule::~Rule()
{}

void Rule::reloadcfg(std::string configfile)
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

		id = -1 ;
		
		if(!(iss >> id >> req >> answ)) 
		{
			std::cerr << "Rule_config critical read error in line " << linecounter << " !\n";
			break; // error
		}

		//out of bound fehlererkennung
		if( id < 0 || id > 255 || req < 0 || req > 255 || answ < 0 || answ > 255 )
		{
			std::cerr << "Rule_config [Line " << linecounter << "]: Number out of bound !\n";
		}

		if(properties[id].reqsize != 0 || properties[id].anssize != 0)
		{
			std::cerr << "Rule_config [Line " << linecounter << "]: Possible multiple instantiation of id usage !\n";
		}

		//write values
		properties[id].reqsize = req;
		properties[id]. anssize = answ;

	}

	ifs.close();
}

uint8_t Rule::requestsize(uint8_t id) const
{
	return properties[id].reqsize;
}

uint8_t Rule::answersize(uint8_t id) const
{
	return properties[id].anssize;
}
