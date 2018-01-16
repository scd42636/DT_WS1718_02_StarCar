// -------------------------------------------------------------------------------------------------
// <copyright file="AcceleratorDefines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef ACCELERATORDEFINES_H
#define ACCELERATORDEFINES_H

#include "Defines.h"


// Port and pin resource for power-up of acceleration sensor, VDD=PJ.0
#define AS_PWR_OUT           (PJOUT)
#define AS_PWR_DIR           (PJDIR)
#define AS_PWR_PIN           (BIT0)

// Port, pin and interrupt resource for interrupt from acceleration sensor, CMA_INT=P2.5
#define AS_INT_IN            (P2IN)
#define AS_INT_OUT           (P2OUT)
#define AS_INT_DIR           (P2DIR)
#define AS_INT_IE            (P2IE)
#define AS_INT_IES           (P2IES)
#define AS_INT_IFG           (P2IFG)
#define AS_INT_PIN           (BIT5)

// Disconnect power supply for acceleration sensor when not used
#define AS_DISCONNECT

// Port and pin resource for SPI interface to acceleration sensor
// SDO=MOSI=P1.6, SDI=MISO=P1.5, SCK=P1.7
#define AS_SPI_IN            (P1IN)
#define AS_SPI_OUT           (P1OUT)
#define AS_SPI_DIR           (P1DIR)
#define AS_SPI_SEL           (P1SEL)
#define AS_SPI_REN           (P1REN)
#define AS_SDO_PIN           (BIT6)
#define AS_SDI_PIN           (BIT5)
#define AS_SCK_PIN           (BIT7)

// CSN=PJ.1
#define AS_CSN_OUT           (PJOUT)
#define AS_CSN_DIR           (PJDIR)
#define AS_CSN_PIN           (BIT1)

#define AS_TX_BUFFER         (UCA0TXBUF)
#define AS_RX_BUFFER         (UCA0RXBUF)
#define AS_TX_IFG            (UCTXIFG)
#define AS_RX_IFG            (UCRXIFG)
#define AS_IRQ_REG           (UCA0IFG)
#define AS_SPI_CTL0          (UCA0CTL0)
#define AS_SPI_CTL1          (UCA0CTL1)
#define AS_SPI_BR0           (UCA0BR0)
#define AS_SPI_BR1           (UCA0BR1)

// SPI timeout to detect sensor failure
#define AS_SPI_TIMEOUT       (1000u)

#define ACCEL_MODE_OFF          (0u)
#define ACCEL_MODE_ON           (1u)

// Stop acceleration measurement after 60 minutes to save battery
#define ACCEL_MEASUREMENT_TIMEOUT               (60 * 60u)

typedef struct AcceleratorInfo_t
{
    u8 Mode;                    // ACC_MODE_OFF, ACC_MODE_ON
    u8 xyz[3];                  // Sensor raw data
    u16 Data;                   // Acceleration data in 10 * mgrav
    u16 Timeout;                // Timeout
} AcceleratorInfo;

extern AcceleratorInfo SystemAccelerator;

#endif
