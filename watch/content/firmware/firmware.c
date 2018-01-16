// -------------------------------------------------------------------------------------------------
// <copyright file="Firmware.c" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#include "bsp.h"

#include "Firmware.h"
#include "Defines.h"

#include "Accelerator.h"
#include "Buzzer.h"
#include "Radio.h"
#include "Timer.h"
#include "Watch.h"

#include "MotionControl.h"

#include "adc12.h"
#include "buzzer.h"
//#include "simpliciti.h"
#include "rfsimpliciti.h"

// Number of calibration data bytes in INFOA memory
#define CALIBRATION_DATA_LENGTH         (13u)

bool isRunning = false;
u8 rf_frequoffset;


// ---------- Private methods (private predefines) ----------

void do_acceleration_measurement();
void battery_measurement();
void read_calibration_values();

void ProcessDisplayUpdate();
void ProcessRequests();
void ProcessWakeupEvent();


// ---------- Public methods (public defines) ----------

void Firmware_Setup()
{
    Watch_Setup();

    Timer_Setup();
    Radio_Setup();
    Accelerator_Setup();
    Buzzer_Setup();

    MotionControl_Setup();

    // Read calibration values from info memory
    read_calibration_values();

    // Init system flags
    display.all_flags = 1;

    // Force full display update when starting up
    display.flag.full_update = 1;

    // Reset battery measurement
    battery_measurement();
}

bool Firmware_Loop()
{
    if (!isRunning) {
        isRunning = true;
        Watch_ShowWelcome();
    }

    Watch_GoIntoIdleMode();

    if (SystemButtons.Flags || System.Flags)
        ProcessWakeupEvent();

    if (SystemRequests.Flags)
        ProcessRequests();

    if (display.all_flags)
        ProcessDisplayUpdate();

    return isRunning;
}

void Firmware_Exit()
{
}


// ---------- Private methods (private defines) ----------

void ProcessDisplayUpdate()
{
    if (display.flag.full_update || display.flag.line1_full_update) {
        clear_line(LINE1);
        display_chars(LCD_SEG_L1_2_0, (u8*) "Car", SEG_ON);
    }

    if (display.flag.full_update || display.flag.line2_full_update) {
        clear_line(LINE2);
        display_chars(LCD_SEG_L2_4_0, (u8*) "-----", SEG_ON);

        if (MotionControl.Mode == MotionControlMode_Direction) {
            display_symbol(LCD_ICON_STOPWATCH, SEG_OFF);
            display_symbol(LCD_ICON_RECORD, SEG_ON);
        }
        else if (MotionControl.Mode == MotionControlMode_Speed) {
            display_symbol(LCD_ICON_STOPWATCH, SEG_ON);
            display_symbol(LCD_ICON_RECORD, SEG_OFF);
        }
        else {
            display_symbol(LCD_ICON_STOPWATCH, SEG_OFF);
            display_symbol(LCD_ICON_RECORD, SEG_OFF);
        }

        if (MotionControl.State == MotionControlState_Started) {
            display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_ON);
            display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_ON);
            display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_ON);
        }
        else {
            display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
            display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
            display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);
        }
    }

    display.all_flags = 0;
}

void ProcessRequests()
{
    // Do acceleration measurement
    if (SystemRequests.States.MeasureAcceleration)
        do_acceleration_measurement();

    // Do voltage measurement
    if (SystemRequests.States.MeasureVoltage)
        battery_measurement();

    // Generate alarm (two signals every second)
    if (SystemRequests.States.Buzzer)
        Buzzer_Start(2, BUZZER_ON_TICKS, BUZZER_OFF_TICKS);

    // Reset request flag
    SystemRequests.Flags = 0;
}

void ProcessWakeupEvent()
{
    // Enable idle timeout
    System.States.IdleTimeoutEnabled = true;

    // Process single button press event (after button was released)
    if (SystemButtons.Flags)
    {
        // M1 button event ---------------------------------------------------------------------
        if (SystemButtons.States.HasStarPressed)
        {
            if (MotionControl.Mode == MotionControlMode_None)
                MotionControl.Mode = MotionControlMode_Direction;
            else if (MotionControl.Mode == MotionControlMode_Direction)
                MotionControl.Mode = MotionControlMode_Speed;
            else if (MotionControl.Mode == MotionControlMode_Speed)
                MotionControl.Mode = MotionControlMode_Direction;

            display.flag.line2_full_update = true;
            SystemButtons.States.HasStarPressed = false;
        }
        // NUM button event ------------------------------------------------------------------------
        // (Short) Advance to next menu item
        else if (SystemButtons.States.HasNumPressed)
        {
            display.flag.line2_full_update = true;
            SystemButtons.States.HasNumPressed = false;

            start_simpliciti_tx_only();
        }
        // UP button event -------------------------------------------------------------------------
        else if (SystemButtons.States.HasUpPressed)
        {
            SystemButtons.States.HasUpPressed = false;
        }
        // DOWN button event -----------------------------------------------------------------------
        else if (SystemButtons.States.HasDownPressed)
        {
            SystemButtons.States.HasDownPressed = false;
        }
    }

    // Process internal events
    if (System.Flags)
    {
        // Idle timeout ----------------------------------------------------------------------------
        if (System.States.IdleTimeout)
        {
            // Clear timeout flag
            System.States.IdleTimeout = 0;

            // Clear display
            clear_display();

            // Set display update flags
            display.flag.full_update = 1;
        }
    }

    // Disable idle timeout
    System.States.IdleTimeoutEnabled = false;
}

// *************************************************************************************************
// @fn          do_acceleration_measurement
// @brief       Get sensor data and store in sAccel struct
// @param       none
// @return      none
// *************************************************************************************************
void do_acceleration_measurement()
{
    // Get data from sensor
    Accelerator_GetData(SystemAccelerator.xyz);

    // Set display update flag
    display.flag.update_acceleration = 1;
}

// *************************************************************************************************
// @fn          battery_measurement
// @brief       Init ADC12. Do single conversion of AVCC voltage. Turn off ADC12.
// @param       none
// @return      none
// *************************************************************************************************
void battery_measurement()
{
    return;

    u16 voltage;

    // Convert external battery voltage (ADC12INCH_11=AVCC-AVSS/2)
    voltage = adc12_single_conversion(REFVSEL_1, ADC12SHT0_10, ADC12INCH_11);

    // Convert ADC value to "x.xx V"
    // Ideally we have A11=0->AVCC=0V ... A11=4095(2^12-1)->AVCC=4V
    // --> (A11/4095)*4V=AVCC --> AVCC=(A11*4)/4095
    voltage = (voltage * 2 * 2) / 41;

    // Correct measured voltage with calibration value
    voltage += SystemBattery.Offset;

    // Discard values that are clearly outside the measurement range
    if (voltage > BATTERY_HIGH_THRESHOLD)
    {
        voltage = SystemBattery.Voltage;
    }

    // Filter battery voltage
    SystemBattery.Voltage = ((voltage * 2) + (SystemBattery.Voltage * 8)) / 10;

    // If battery voltage falls below low battery threshold, set system flag and modify LINE2
    // display function pointer
    if (SystemBattery.Voltage < BATTERY_LOW_THRESHOLD)
    {
        System.States.LowBattery = true;

        // Set sticky battery icon
        display_symbol(LCD_SYMB_BATTERY, SEG_ON);
    }
    else
    {
        System.States.LowBattery = false;

        // Clear sticky battery icon
        display_symbol(LCD_SYMB_BATTERY, SEG_OFF);
    }
    // Update LINE2
    display.flag.line2_full_update = 1;

    // Indicate to display function that new value is available
    display.flag.update_battery_voltage = 1;
}

// *************************************************************************************************
// @fn          read_calibration_values
// @brief       Read calibration values for temperature measurement, voltage measurement
//                              and radio from INFO memory.
// @param       none
// @return      none
// *************************************************************************************************
void read_calibration_values()
{
    u8 cal_data[CALIBRATION_DATA_LENGTH]; // Temporary storage for constants
    u8 i;
    u8 *flash_mem;                        // Memory pointer

    // Read calibration data from Info D memory
    flash_mem = (u8 *) 0x1800;
    for (i = 0; i < CALIBRATION_DATA_LENGTH; i++)
    {
        cal_data[i] = *flash_mem++;
    }

    if (cal_data[0] == 0xFF)
    {
        // If no values are available (i.e. INFO D memory has been erased by user), assign
        // experimentally derived values
        rf_frequoffset = 4;
        SystemBattery.Offset = -10;

        SystemData.EndDeviceAddress[0] = 0x79;
        SystemData.EndDeviceAddress[1] = 0x56;
        SystemData.EndDeviceAddress[2] = 0x34;
        SystemData.EndDeviceAddress[3] = 0x12;
    } else
    {
        // Assign calibration data to global variables
        rf_frequoffset = cal_data[1];

        // Range check for calibrated FREQEST value (-20 .. + 20 is ok, else use default value)
        if ((rf_frequoffset > 20) && (rf_frequoffset < (256 - 20)))
        {
            rf_frequoffset = 0;
        }

        SystemBattery.Offset = (s16) ((cal_data[4] << 8) + cal_data[5]);

        SystemData.EndDeviceAddress[0] = cal_data[6];
        SystemData.EndDeviceAddress[1] = cal_data[7];
        SystemData.EndDeviceAddress[2] = cal_data[8];
        SystemData.EndDeviceAddress[3] = cal_data[9];
    }
}
