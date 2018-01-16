
#ifndef RFSIMPLICITI_H_
#define RFSIMPLICITI_H_

#include "Defines.h"
#include "simpliciti.h"

// *************************************************************************************************
// Prototypes section
extern void sx_rf(u8 line);
extern void display_control(u8 line, u8 update);
extern void send_smpl_data(u16 data);
extern u8 is_rf(void);
extern void start_simpliciti_tx_only();

// Stop SimpliciTI transmission after 60 minutes to save power
#define SIMPLICITI_TIMEOUT                          (60 * 60u)

extern unsigned char simpliciti_flag;
extern unsigned char simpliciti_data[SIMPLICITI_MAX_PAYLOAD_LENGTH];

#endif                          /*RFSIMPLICITI_H_ */
