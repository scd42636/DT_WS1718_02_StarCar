#ifndef LIDAR
#define LIDAR

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <string>

#include "include/urg_sensor.h"
#include "include/urg_utils.h"


class lidar {

public:
    int get_measurement(char *);
};


#endif
