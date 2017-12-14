#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>

#include "include/urg_sensor.h"
#include "include/urg_utils.h"

// uncomment for debug information
//#define DEBUG	
// uncomment for vebose debug information
//#define DEBUGV


class lidar
{
	public:
  	int get_measurement();
};

//starts measurement once
int lidar::get_measurement(char *file_name)
{
	
	#ifdef  DEBUGV 
	printf("LIDAR: Started\n");
	#endif
	
	//file name for data
	//char file_name[] = "LIDAR_data.txt";

	urg_t urg;
	int ret;
	long *length_data;
	int length_data_size;
	
	const char connect_device[] = "/dev/ttyACM0";
	const long connect_baudrate = 115200;
	
	#ifdef DEBUGV
	printf("LIDAR: Variables initialized\n");
	#endif
	
	ret = urg_open(&urg, URG_SERIAL, connect_device, connect_baudrate);
	#ifdef DEBUG 	
	printf("LIDAR: urg_open: %d\n", ret);
	#endif
	
	length_data = (long *)malloc(sizeof(long)* urg_max_data_size(&urg));
	
	while
	ret += urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
	#ifdef DEBUG
	printf("LIDAR: urg_start_measurement: %d\n", ret);
	#endif
	
	length_data_size = urg_get_distance(&urg, length_data, NULL);

	long *xVal, *yVal;
	xVal = (long*)malloc(length_data_size * sizeof(long));
	yVal = (long*)malloc(length_data_size * sizeof(long));
	
	std::string xValues = "";
	std::string yValues = "";	

	for(int i=0; i < length_data_size; ++i)
	{
		double radian;
		long length, x, y;

		radian = urg_index2rad(&urg, i);
		length = length_data[i];
		*(xVal +i) = (long)(length * sin(radian));
		*(yVal +i) = (long)(length * cos(radian));
		
		//printf("%ld, ", *(xVal+i));
			
		xValues += std::to_string(*(xVal+i)) + ", ";
		yValues += std::to_string(*(yVal+i)) + ", ";
	}

	#ifdef DEBUGV
	printf("LIDAR: Write data to file: %s\n", file_name);
	#endif
	std::ofstream file;
	file.open(file_name);
	file << std::to_string(ret) + "\n";	
	file << xValues + "\n";
	file << yValues + "\n";
	file.close();
	#ifdef DEBUGV
	printf("LIDAR: Write data done\n");
	#endif

	//for(int i=0; i<length_data_size; i++){
	//	printf("%ld, ", *(yVal +i));
	//}

	#ifdef DEBUGV
	printf("LIDAR: Free memory\n");
	#endif

	urg_close(&urg);
	free(xVal);
	free(yVal);

	#ifdef DEBUG 
	printf("LIDAR: End of program\n");
	#endif		
	return(ret);		
}
