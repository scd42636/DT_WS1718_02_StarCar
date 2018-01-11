#ifndef STARCAR
#define STARCAR

enum StarCarMode
{
    CarMode_None = 0,
    CarMode_Controller = 1,
    CarMode_Watch = 2
};

enum StarCarSensorRequest
{
    CarSensorRequest_None = 0,
    CarSensorRequest_Sonic = 1,
    CarSensorRequest_Magnet = 2,
    CarSensorRequest_Accelerator = 4,
    CarSensorRequest_All = 7
};

typedef struct payload
{
    StarCarMode Mode;
    StarCarSensorRequest Request;

    uint16_t DistanceFront;
    uint16_t DistanceBack;

    uint8_t DirectionParity;
    uint16_t DirectionValue;

    uint8_t AccelerationXParity;
    uint16_t AccelerationXValue;

    uint8_t AccelerationYParity;
    uint16_t AccelerationYValue;
} __attribute__((packed)) message;

#endif // STARCAR

