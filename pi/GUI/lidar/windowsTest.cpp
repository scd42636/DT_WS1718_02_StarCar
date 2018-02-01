// StarCarUrg.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>  

#include "urg_sensor.h"
#include "urg_utils.h"


int main()
{
	urg_t urg;
	int ret;
	long *length_data;
	int length_data_size;


	// \~english "COM1" is, in this case, the device name detected for the sensor
	const char connect_device[] = "COM6";
	const long connect_baudrate = 115200;


	// \~english Connects to the sensor
	ret = urg_open(&urg, URG_SERIAL, connect_device, connect_baudrate);
	// \todo check error code

	// \~english Allocates memory to hold received measurement data
	length_data = (long *)malloc(sizeof(long)* urg_max_data_size(&urg));
	// \todo check length_data is not NULL


	// \~english Starts range data measurement
	ret = urg_start_measurement(&urg, URG_DISTANCE, 1, 0);
	// \todo check error code


	// \~english Receives the measurement data
	length_data_size = urg_get_distance(&urg, length_data, NULL);
	// \todo process length_data array




	long *xVal, *yVal;

	xVal = (long*)malloc(length_data_size * sizeof(long));  //memory allocated using malloc
	yVal = (long*)malloc(length_data_size * sizeof(long));

	for (int i = 0; i < length_data_size; ++i) {
		// \~english Gets the angle in radians for range data, and convert to X-Y coordinates
		double radian;
		long length;
		long x;
		long y;

		radian = urg_index2rad(&urg, i);
		length = length_data[i];
		// \todo check length is valid

		*(xVal + i) = (long)(length * sin(radian));
		*(yVal + i) = (long)(length * cos(radian));

		printf("%ld,", *(xVal + i));
	}

	printf("\n");

	for (int i = 0; i < length_data_size; i++)
	{
		printf("%ld,", *(yVal + i));
	}

	
	// \~english Disconnects from the sensor
	urg_close(&urg);
	free(xVal);
	free(yVal);

	return 0;
}

