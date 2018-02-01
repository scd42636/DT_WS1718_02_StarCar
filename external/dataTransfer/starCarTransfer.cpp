#include <stdlib.h>

#include <stdio.h>

#include <iostream>


#include <string>


#include <errno.h>
#include <cerrno>
#include <cstring>
int main(int argc, char ** argv)

{


int counter=1;

for(counter=1; counter<100; counter++)
{

system("'C:/Program Files/PuTTY/psftp.exe' pi@172.16.24.254 -pw starcar -b 'C:/Program Files/PuTTY/test_commands.sh'");
std::string str="BeschleunigungX.txt";

str.insert(15, "Cp");

rename("D:\\DT\\FinalRun\\BeschleunigungX.txt", ("D:\\DT\\FinalRun\\" + str).c_str());

str="BeschleunigungY.txt";

str.insert(15, "Cp");
rename("D:\\DT\\FinalRun\\BeschleunigungY.txt", ("D:\\DT\\FinalRun\\" + str).c_str());

str="Compass.txt";

str.insert(7, "Cp");
rename("D:\\DT\\FinalRun\\Compass.txt", ("D:\\DT\\FinalRun\\" + str).c_str());

str="Lidar.txt";

str.insert(5, "Cp");
rename("D:\\DT\\FinalRun\\Lidar.txt", ("D:\\DT\\FinalRun\\" + str).c_str());

str="UltraHinten.txt";

str.insert(11, "Cp");
rename("D:\\DT\\FinalRun\\UltraHinten.txt", ("D:\\DT\\FinalRun\\" + str).c_str());

str="UltraVorne.txt";

str.insert(10, "Cp");
rename("D:\\DT\\FinalRun\\UltraVorne.txt", ("D:\\DT\\FinalRun\\" + str).c_str());
}
return 0;
}