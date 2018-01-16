#include "Timer.h"


// *************************************************************************************************
// ADC12 functions.
// *************************************************************************************************

// *************************************************************************************************
// Include section

// driver
#include "adc12.h"
#include "timer.h"

// *************************************************************************************************
// Prototypes section

// *************************************************************************************************
// Defines section

// *************************************************************************************************
// Global Variable section
u16 adc12_result;
u8 adc12_data_ready;

// *************************************************************************************************
// Extern section

// *************************************************************************************************
// @fn          adc12_single_conversion
// @brief       Init ADC12.      Do single conversion. Turn off ADC12.
// @param       u16 ref                  Select reference
//                              u16 sht          Sample-and-hold time
//                              u16 channel      Channel of the conversion
// @return      u16 adc12_result Return ADC result
// *************************************************************************************************
u16 adc12_single_conversion(u16 ref, u16 sht, u16 channel)
{
    // Initialize the shared reference module
    REFCTL0 |= REFMSTR + ref + REFON;   // Enable internal reference (1.5V or 2.5V)

    // Initialize ADC12_A
    ADC12CTL0 = sht + ADC12ON;          // Set sample time
    ADC12CTL1 = ADC12SHP;               // Enable sample timer
    ADC12MCTL0 = ADC12SREF_1 + channel; // ADC input channel
    ADC12IE = 0x001;                    // ADC_IFG upon conv result-ADCMEMO

    // Wait 2 ticks (66us) to allow internal reference to settle
    Timer_Delay(2);

    // Start ADC12
    ADC12CTL0 |= ADC12ENC;

    // Clear data ready flag
    adc12_data_ready = 0;

    // Sampling and conversion start
    ADC12CTL0 |= ADC12SC;

    // Delay to get next ADC value
    Timer_Delay(5);
    while (!adc12_data_ready) ;

    // Shut down ADC12
    ADC12CTL0 &= ~(ADC12ENC | ADC12SC | sht);
    ADC12CTL0 &= ~ADC12ON;

    // Shut down reference voltage
    REFCTL0 &= ~(REFMSTR + ref + REFON);

    ADC12IE = 0;

    // Return ADC result
    return (adc12_result);
}

// *************************************************************************************************
// @fn          ADC12ISR
// @brief       Store ADC12 conversion result. Set flag to indicate data ready.
// @param       none
// @return      none
// *************************************************************************************************
#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR(void)
{
    switch (__even_in_range(ADC12IV, 34))
    {
        case 0:
            break;                    // Vector  0:  No interrupt
        case 2:
            break;                    // Vector  2:  ADC overflow
        case 4:
            break;                    // Vector  4:  ADC timing overflow
        case 6:                       // Vector  6:  ADC12IFG0
            adc12_result = ADC12MEM0; // Move results, IFG is cleared
            adc12_data_ready = 1;
            _BIC_SR_IRQ(LPM3_bits);   // Exit active CPU
            break;
        case 8:
            break;                    // Vector  8:  ADC12IFG1
        case 10:
            break;                    // Vector 10:  ADC12IFG2
        case 12:
            break;                    // Vector 12:  ADC12IFG3
        case 14:
            break;                    // Vector 14:  ADC12IFG4
        case 16:
            break;                    // Vector 16:  ADC12IFG5
        case 18:
            break;                    // Vector 18:  ADC12IFG6
        case 20:
            break;                    // Vector 20:  ADC12IFG7
        case 22:
            break;                    // Vector 22:  ADC12IFG8
        case 24:
            break;                    // Vector 24:  ADC12IFG9
        case 26:
            break;                    // Vector 26:  ADC12IFG10
        case 28:
            break;                    // Vector 28:  ADC12IFG11
        case 30:
            break;                    // Vector 30:  ADC12IFG12
        case 32:
            break;                    // Vector 32:  ADC12IFG13
        case 34:
            break;                    // Vector 34:  ADC12IFG14
        default:
            break;
    }
}

