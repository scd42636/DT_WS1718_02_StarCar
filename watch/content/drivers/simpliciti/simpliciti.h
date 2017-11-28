// -------------------------------------------------------------------------------------------------
// <copyright file="simpliciti.h" company="Sprocket Enterprises">
//     Copyright (c) OTH Regensburg, Team 02 StarCar. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl</autor>
// -------------------------------------------------------------------------------------------------

#ifndef SIMPLICIT_H
#define SIMPLICIT_H

// *************************************************************************************************
//
// SimpliciTI packet size (TX only mode)
// -------------------------------------
//
// 		* packet rate			(100/3) packets/second = 33.3 packets/second
//		* packet length			28 bytes
//		* packet structure		4 bytes 	preamble
//								4 bytes 	sync
//								1 bytes 	length
//								1 bytes 	address
//								16 bytes 	data
//									 12 byte network data
//									 4  byte user data
//								2 bytes		crc
//
// SimpliciTI frequency overview
// -----------------------------
//
// CC430_End_Device_433MHz.lib (433MHz ISM band)
//
//		* base frequency		433.92 MHz
//		* deviation				32 kHz
//		* channel spacing		25 kHz
//		* used channel number	0 (frequency agility/hopping disabled)
//		* data rate				76.8 kBaud
//		* output power			1.4 dBm
//		* duty					9,6% (TX only mode, 32 packets / second)
//
// CC430_End_Device_868MHz.lib (868MHz ISM band)
//
//		* base frequency		869.525 MHz
//		* deviation				32 kHz
//		* channel spacing		25 kHz
//		* used channel number	0 (frequency agility/hopping disabled)
//		* data rate				76.8 kBaud
//		* output power			1.1 dBm
//		* duty					9,6% (TX only mode, 32 packets / second)
//
// CC430_End_Device_915MHz.lib (915MHz ISM band)
//
//		* base frequency		902.000 MHz
//		* deviation				32 kHz
//		* channel spacing		200 kHz
//		* used channel number	20 (frequency agility/hopping disabled)
//		* data rate				76.8 kBaud
//		* output power			1.3 dBm
//		* duty					9.6% (TX only mode, 32 packets / second)
//
// *************************************************************************************************

// ---------------------------------------------------------------
// Generic defines and variables

// Entry point into SimpliciTI library
extern unsigned char simpliciti_link(void);

// 4 byte device address overrides device address set during compile time
extern unsigned char simpliciti_ed_address[4];

// Maximum data length
#define SIMPLICITI_MAX_PAYLOAD_LENGTH       	(32u)

// Data to send / receive
extern unsigned char simpliciti_data[SIMPLICITI_MAX_PAYLOAD_LENGTH];

// Flag contains status information and triggers to send data or to exit SimpliciTI library
// Control is done from outside SimpliciTI library
extern unsigned char simpliciti_flag;
#define SIMPLICITI_STATUS_LINKING		    	(BIT0)
#define SIMPLICITI_STATUS_LINKED		    	(BIT1)
#define SIMPLICITI_STATUS_ERROR		            (BIT2)
#define SIMPLICITI_TRIGGER_SEND_DATA 	        (BIT3)
#define SIMPLICITI_TRIGGER_RECEIVED_DATA 	    (BIT4)
#define SIMPLICITI_TRIGGER_STOP		            (BIT5)

// Radio frequency offset read from calibration memory
// Compensates crystal deviation from 26MHz nominal value
extern unsigned char rf_frequoffset;

// Macros
#define getFlag(val, flag)						((val&flag)==flag)
#define setFlag(val, flag)						(val|=flag)
#define clearFlag(val, flag)					(val&=(~flag))
#define toggleFlag(val, flag)					(val^=flag)


// ---------------------------------------------------------------
// SimpliciTI RX only

// Entry point into SimpliciTI library
extern void simpliciti_main_tx_only(void);

// Callback function to read data from acceleration sensor or buttons and trigger sending
extern void simpliciti_get_ed_data_callback(void);


// ---------------------------------------------------------------
// SimpliciTI Sync

// Sync data length
#define BM_SYNC_DATA_LENGTH                     (19u)

// Device data  (0)TYPE   (1) - (18) DATA
#define SYNC_ED_TYPE_R2R                        (1u)
#define SYNC_ED_TYPE_MEMORY                     (2u)
#define SYNC_ED_TYPE_STATUS                     (3u)

// Host data    (0)CMD    (1) - (18) DATA
#define SYNC_AP_CMD_NOP                         (1u)
#define SYNC_AP_CMD_GET_STATUS					(2u)
#define SYNC_AP_CMD_SET_WATCH                   (3u)
#define SYNC_AP_CMD_GET_MEMORY_BLOCKS_MODE_1   	(4u)
#define SYNC_AP_CMD_GET_MEMORY_BLOCKS_MODE_2   	(5u)
#define SYNC_AP_CMD_ERASE_MEMORY                (6u)
#define SYNC_AP_CMD_EXIT						(7u)


// Entry point into SimpliciTI library
extern void simpliciti_main_sync(void);

// Callback function to decode access point command
extern void simpliciti_sync_decode_ap_cmd_callback(void);

// Callback function to read data from application and trigger sending
extern void simpliciti_sync_get_data_callback(unsigned int index);

// Send reply packets (>0), 0=no need to reply
extern unsigned char simpliciti_reply_count;

#endif
