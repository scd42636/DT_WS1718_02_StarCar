// -------------------------------------------------------------------------------------------------
// <copyright file="Accelerator.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "Accelerator.h"

// ---------- Private fields ----------

bool acceleratorIsReady = false;


// ---------- Private methods (private predefines) ----------

u8 cma_as_read_register(u8 bAddress);
u8 cma_as_write_register(u8 bAddress, u8 bData);
void cma_as_get_data(u8 * data);


// ---------- Public methods (public defines) ----------

void Accelerator_GetData(u8* data)
{
    // Reset current acceleration value
    SystemAccelerator.Data = 0;

    // Get data from sensor
    cma_as_get_data(SystemAccelerator.xyz);
}

bool Accelerator_IsActive()
{
    return ((SystemAccelerator.Mode == ACCEL_MODE_ON) && (SystemAccelerator.Timeout > 0));
}

void Accelerator_Reset()
{
    // Clear timeout counter
    SystemAccelerator.Timeout = 0;

    // Default mode is off
    SystemAccelerator.Mode = ACCEL_MODE_OFF;
}

void Accelerator_Setup()
{
    // Init acceleration sensor
    // Deactivate connection to acceleration sensor
    AS_PWR_OUT &= ~AS_PWR_PIN;         // Power off
    AS_INT_OUT &= ~AS_INT_PIN;         // Pin to low to avoid floating pins
    AS_SPI_OUT &= ~(AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN); // Pin to low to avoid floating pins
    AS_CSN_OUT &= ~AS_CSN_PIN;         // Pin to low to avoid floating pins
    AS_INT_DIR |= AS_INT_PIN;          // Pin to output to avoid floating pins
    AS_SPI_DIR |= AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN; // Pin to output to avoid floating pins
    AS_CSN_DIR |= AS_CSN_PIN;          // Pin to output to avoid floating pins
    AS_PWR_DIR |= AS_PWR_PIN;          // Power pin to output direction

    acceleratorIsReady = true;
}

void Accelerator_Start()
{
    volatile u16 Counter_u16;
    u8 bConfig;

    // Initialize SPI interface to acceleration sensor
    AS_SPI_CTL0 |= UCSYNC | UCMST | UCMSB        // SPI master, 8 data bits,  MSB first,
                   | UCCKPH;                     //  clock idle low, data output on falling
                                                 // edge
    AS_SPI_CTL1 |= UCSSEL1;                      // SMCLK as clock source
    AS_SPI_BR0 = CMA_AS_BR_DIVIDER;              // Low byte of division factor for baud rate
    AS_SPI_BR1 = 0x00;                           // High byte of division factor for baud
                                                 // rate
    AS_SPI_CTL1 &= ~UCSWRST;                     // Start SPI hardware

    // Configure interface pins
    // Initialize interrupt pin for data read out from acceleration sensor
    AS_INT_IES &= ~AS_INT_PIN;                   // Interrupt on rising edge

    // Enable interrupt
    AS_INT_DIR &= ~AS_INT_PIN;                   // Switch INT pin to input
    AS_SPI_DIR &= ~AS_SDI_PIN;                   // Switch SDI pin to input
    AS_SPI_REN |= AS_SDI_PIN;                    // Pulldown on SDI pin
    AS_SPI_SEL |= AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN; // Port pins to SDO, SDI and SCK function
    AS_CSN_OUT |= AS_CSN_PIN;                    // Deselect acceleration sensor
    AS_PWR_OUT |= AS_PWR_PIN;                    // Power on active high

    // Delay of >5ms required between switching on power and configuring sensor
    Timer_Delay(ConvertMillisecondsToTicks(10));

    // Initialize interrupt pin for data read out from acceleration sensor
    AS_INT_IFG &= ~AS_INT_PIN;                   // Reset flag
    AS_INT_IE |= AS_INT_PIN;                     // Enable interrupt

    // Configure sensor and start to sample data
#if (CMA_AS_RANGE == 2)
    bConfig = 0x80;
#    if (CMA_AS_SAMPLE_RATE == 100)
    bConfig |= 0x02;
#    elif (CMA_AS_SAMPLE_RATE == 400)
    bConfig |= 0x04;
#    else
#        error "Sample rate not supported"
#    endif
#elif (CMA_AS_RANGE == 8)
    bConfig = 0x00;
#    if (CMA_AS_SAMPLE_RATE == 40)
    bConfig |= 0x06;
#    elif (CMA_AS_SAMPLE_RATE == 100)
    bConfig |= 0x02;
#    elif (CMA_AS_SAMPLE_RATE == 400)
    bConfig |= 0x04;
#    else
#        error "Sample rate not supported"
#    endif
#else
#    error "Measurement range not supported"
#endif

    // Reset sensor
    cma_as_write_register(0x04, 0x02);
    cma_as_write_register(0x04, 0x0A);
    cma_as_write_register(0x04, 0x04);

    // Wait 5 ms before starting sensor output
    Timer_Delay(ConvertMillisecondsToTicks(5));

    // Set 2g measurement range, start to output data with 100Hz rate
    cma_as_write_register(0x02, bConfig);
}

void Accelerator_Stop()
{
    // Disable interrupt
    AS_INT_IE &= ~AS_INT_PIN;                    // Disable interrupt

    // Power-down sensor
    AS_PWR_OUT &= ~AS_PWR_PIN;                   // Power off
    AS_INT_OUT &= ~AS_INT_PIN;                   // Pin to low to avoid floating pins
    AS_SPI_OUT &= ~(AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN); // Pins to low to avoid floating pins
    AS_SPI_SEL &= ~(AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN); // Port pins to I/O function
    AS_CSN_OUT &= ~AS_CSN_PIN;                   // Pin to low to avoid floating pins
    AS_INT_DIR |= AS_INT_PIN;                    // Pin to output to avoid floating pins
    AS_SPI_DIR |= AS_SDO_PIN + AS_SDI_PIN + AS_SCK_PIN; // Pins to output to avoid floating pins
    AS_CSN_DIR |= AS_CSN_PIN;                    // Pin to output to avoid floating pins
}

// ---------- Private methods (private defines) ----------

// *************************************************************************************************
// @fn          cma_as_read_register
// @brief       Read a byte from the acceleration sensor
// @param       u8 bAddress             Register address
// @return      u8 0 or bResult         Register content
//                                      If the returned value is 0 there was an error.
// *************************************************************************************************
u8 cma_as_read_register(u8 bAddress)
{
    bAddress <<= 2;                              // Address to be shifted left by 2 and
                                                 // RW bit to be reset

    u8 bResult;
    u16 timeout;

    // Exit function if an error was detected previously
    if (!acceleratorIsReady)
        return (0);

    AS_SPI_REN &= ~AS_SDI_PIN;                   // Pulldown on SDI pin not required
    AS_CSN_OUT &= ~AS_CSN_PIN;                   // Select acceleration sensor

    bResult = AS_RX_BUFFER;                      // Read RX buffer just to clear
                                                 // interrupt flag

    AS_TX_BUFFER = bAddress;                     // Write address to TX buffer

    timeout = AS_SPI_TIMEOUT;
    while (!(AS_IRQ_REG & AS_RX_IFG) && (--timeout > 0)); // Wait until new data was written into
                                                 // RX buffer
    if (timeout == 0)
    {
        acceleratorIsReady = false;
        return (0);
    }
    bResult = AS_RX_BUFFER;                      // Read RX buffer just to clear
                                                 // interrupt flag

    AS_TX_BUFFER = 0;                            // Write dummy data to TX buffer

    timeout = AS_SPI_TIMEOUT;
    while (!(AS_IRQ_REG & AS_RX_IFG) && (--timeout > 0)); // Wait until new data was written into
                                                 // RX buffer
    if (timeout == 0)
    {
        acceleratorIsReady = false;
        return (0);
    }
    bResult = AS_RX_BUFFER;                      // Read RX buffer

    AS_CSN_OUT |= AS_CSN_PIN;                    // Deselect acceleration sensor
    AS_SPI_REN |= AS_SDI_PIN;                    // Pulldown on SDI pin required again

    // Return new data from RX buffer
    return bResult;
}

// *************************************************************************************************
// @fn          cma_as_write_register
// @brief       Write a byte to the acceleration sensor
// @param       u8 bAddress             Register address
//              u8 bData                Data to write
// @return      u8 0 or bResult         Register content
//                                      If the returned value is 0 there was an error.
// *************************************************************************************************
u8 cma_as_write_register(u8 bAddress, u8 bData)
{
    bAddress <<= 2;                              // Address to be shifted left by 1
    bAddress |= BIT1;                            // RW bit to be set

    u8 bResult;
    u16 timeout;

    // Exit function if an error was detected previously
    if (!acceleratorIsReady)
        return (0);

    AS_SPI_REN &= ~AS_SDI_PIN;                   // Pulldown on SDI pin not required
    AS_CSN_OUT &= ~AS_CSN_PIN;                   // Select acceleration sensor

    bResult = AS_RX_BUFFER;                      // Read RX buffer just to clear
                                                 // interrupt flag

    AS_TX_BUFFER = bAddress;                     // Write address to TX buffer

    timeout = AS_SPI_TIMEOUT;
    while (!(AS_IRQ_REG & AS_RX_IFG) && (--timeout > 0)); // Wait until new data was written into
                                                 // RX buffer
    if (timeout == 0)
    {
        acceleratorIsReady = false;
        return (0);
    }
    bResult = AS_RX_BUFFER;                      // Read RX buffer just to clear
                                                 // interrupt flag

    AS_TX_BUFFER = bData;                        // Write data to TX buffer

    timeout = AS_SPI_TIMEOUT;
    while (!(AS_IRQ_REG & AS_RX_IFG) && (--timeout > 0)); // Wait until new data was written into
                                                 // RX buffer
    if (timeout == 0)
    {
        acceleratorIsReady = false;
        return (0);
    }
    bResult = AS_RX_BUFFER;                      // Read RX buffer

    AS_CSN_OUT |= AS_CSN_PIN;                    // Deselect acceleration sensor
    AS_SPI_REN |= AS_SDI_PIN;                    // Pulldown on SDI pin required again

    return bResult;
}

// *************************************************************************************************
// @fn          cma_as_get_data
// @brief       Service routine to read acceleration values.
// @param       none
// @return      none
// *************************************************************************************************
void cma_as_get_data(u8 * data)
{
    // Exit if sensor is not powered up
    if ((AS_PWR_OUT & AS_PWR_PIN) != AS_PWR_PIN)
        return;

    // Store X/Y/Z acceleration data in buffer
    *(data + 0) = cma_as_read_register(0x06);
    *(data + 1) = cma_as_read_register(0x07);
    *(data + 2) = cma_as_read_register(0x08);
}
