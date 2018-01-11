#ifndef STARCAR
#define STARCAR

/*enum StarCarMode
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
*/
typedef struct payload
{
    uint8_t Mode;
    uint8_t Request;

    uint32_t DistanceFront;
    uint32_t DistanceBack;

    uint8_t DirectionParity;
    uint32_t DirectionValue;

    uint8_t AccelerationXParity;
    uint32_t AccelerationXValue;

    uint8_t AccelerationYParity;
    uint32_t AccelerationYValue;

} __attribute__((packed)) message;

#endif // STARCAR

