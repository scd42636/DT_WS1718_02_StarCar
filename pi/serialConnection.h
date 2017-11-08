/*==========================================================================*/
/* serialConnection.h 														*/
/* StarCar																	*/
/*==========================================================================*/

/*#ifndef __cplusplus
extern "C" {
#endif
*/

#ifndef SERIALCONNECTIONHEADER
#define SERIALCONNECTIONHEADER

extern int 	openSerialPorttoArduino 	();
extern int 	sendDatatoArduino			(int fd);
extern void readDatafromArduino			(int fd);

/*#ifdef __cplusplus
}
#endif
*/

#endif
