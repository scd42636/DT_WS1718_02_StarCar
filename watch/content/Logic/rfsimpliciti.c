#include "BatteryDefines.h"
#include "ButtonDefines.h"
#include "SystemDefines.h"

#include "Accelerator.h"
#include "Radio.h"
#include "Timer.h"
#include "MotionControl.h"

// driver
#include "display.h"

// logic
#include "rfsimpliciti.h"
#include "simpliciti.h"

// *************************************************************************************************
// Prototypes section
void simpliciti_get_data_callback(void);
void start_simpliciti_tx_only();

// *************************************************************************************************
// Defines section

// Each packet index requires 2 bytes, so we can have 9 packet indizes in 18 bytes usable payload
#define BM_SYNC_BURST_PACKETS_IN_DATA           (9u)

// flag contains status information, trigger to send data and trigger to exit SimpliciTI
unsigned char simpliciti_flag;

// 4 data bytes to send
unsigned char simpliciti_data[SIMPLICITI_MAX_PAYLOAD_LENGTH];

// 1 = send one or more reply packets, 0 = no need to reply
//unsigned char simpliciti_reply;
unsigned char simpliciti_reply_count;

// 1 = send packets sequentially from burst_start to burst_end, 2 = send packets addressed by their
// index
u8 burst_mode;

// Start and end index of packets to send out
u16 burst_start, burst_end;

// Array containing requested packets
u16 burst_packet[BM_SYNC_BURST_PACKETS_IN_DATA];

// Current packet index
u8 burst_packet_index;

// *************************************************************************************************
// @fn          sx_rf
// @brief       Start SimpliciTI. Button DOWN connects/disconnects to access point.
// @param       u8 line         LINE2
// @return      none
// *************************************************************************************************
void sx_rf(u8 line)
{
    // Exit if battery voltage is too low for radio operation
    if (System.States.LowBattery)
        return;

    // Turn off the backlight
    P2OUT &= ~BUTTON_BACKLIGHT_PIN;
    P2DIR &= ~BUTTON_BACKLIGHT_PIN;

    BUTTONS_IE &= ~BUTTON_BACKLIGHT_PIN;

    // Start SimpliciTI in tx only mode
    start_simpliciti_tx_only();

    BUTTONS_IE |= BUTTON_BACKLIGHT_PIN;

}


// *************************************************************************************************
// @fn          start_simpliciti_tx_only
// @brief       Start SimpliciTI (tx only).
// @param       simpliciti_state_t              SIMPLICITI_ACCELERATION, SIMPLICITI_BUTTONS
// @return      none
// *************************************************************************************************
void start_simpliciti_tx_only()
{
    // Clear all data bytes
    simpliciti_data[0] = MotionControl.Mode;
    simpliciti_data[1] = 0;
    simpliciti_data[2] = 0;
    simpliciti_data[3] = 0;

    // Turn on beeper icon to show activity
    display_symbol(LCD_ICON_BEEPER1, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER2, SEG_ON_BLINK_ON);
    display_symbol(LCD_ICON_BEEPER3, SEG_ON_BLINK_ON);

    // Debounce button event
    Timer_Delay(ConvertMillisecondsToTicks(BUTTONS_DEBOUNCE_TIME_OUT));

    // Prepare radio for RF communication
    Radio_Open();

    // Set SimpliciTI mode
    MotionControl.State = MotionControlState_Started;

    // Set SimpliciTI timeout to save battery power
    MotionControl.Timeout = SIMPLICITI_TIMEOUT;

    // Start SimpliciTI stack. Try to link to access point.
    // Exit with timeout or by a button DOWN press.
    if (simpliciti_link())
    {
        if (Radio_IsOpen())
        {
            // Start acceleration sensor
            Accelerator_Start();
        }

        // Enter TX only routine. This will transfer button events and/or acceleration data to
        // access point.
        simpliciti_main_tx_only();
    }

    // Set SimpliciTI state to OFF
    MotionControl.State = MotionControlState_Stopped;

    // Stop acceleration sensor
    Accelerator_Stop();

    // Powerdown radio
    Radio_Close();

    // Clear last button events
    Timer_Delay(ConvertMillisecondsToTicks(BUTTONS_DEBOUNCE_TIME_OUT));
    BUTTONS_IFG = 0x00;
    SystemButtons.Flags = 0;

    // Clear icons
    display_symbol(LCD_ICON_BEEPER1, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER2, SEG_OFF_BLINK_OFF);
    display_symbol(LCD_ICON_BEEPER3, SEG_OFF_BLINK_OFF);

    // Clean up line 1
    clear_line(LINE2);

    // Force full display update
    display.flag.line2_full_update = true;
}

// *************************************************************************************************
// @fn          display_rf
// @brief       SimpliciTI display routine.
// @param       u8 line                 LINE2
//                              u8 update               DISPLAY_LINE_UPDATE_FULL
// @return      none
// *************************************************************************************************
void display_control(u8 line, u8 update)
{
    if (MotionControl.Mode == MotionControlMode_Direction) {
        display_symbol(LCD_ICON_RECORD, SEG_ON);
    }
    else if (MotionControl.Mode == MotionControlMode_Speed) {
        display_symbol(LCD_ICON_STOPWATCH, SEG_ON);
    }
    else {
        display_symbol(LCD_ICON_RECORD, SEG_OFF);
        display_symbol(LCD_ICON_STOPWATCH, SEG_OFF);
    }

    u8 *str;

    // Redraw line
    if (update == DISPLAY_LINE_UPDATE_FULL)
    {
        // Set battery and V icon
        display_symbol(LCD_SYMB_BATTERY, SEG_ON);

        // Display result in xx.x format
        str = int_to_array(SystemBattery.Voltage, 3, 0);

        display_chars(LCD_SEG_L2_2_0, str, SEG_ON);
        display_symbol(LCD_SEG_L2_DP, SEG_ON);
    }
    else if (update == DISPLAY_LINE_UPDATE_PARTIAL)
    {
        // Display result in xx.x format
        str = int_to_array(SystemBattery.Voltage, 3, 0);

        display_chars(LCD_SEG_L2_2_0, str, SEG_ON);

        display.flag.update_battery_voltage = 0;
    }
    else if (update == DISPLAY_LINE_CLEAR)
    {
        // Clear function-specific symbols
        display_symbol(LCD_SYMB_BATTERY, SEG_OFF);
    }
}

const int controlValue[] = {
    -100,
    -97,
    -94,
    -91,
    -88,
    -85,
    -82,
    -79,
    -76,
    -73,
    -70,
    -67,
    -64,
    -61,
    -58,
    -55,
    -52,
    -48,
    -45,
    -42,
    -39,
    -36,
    -33,
    -30,
    -27,
    -24,
    -21,
    -18,
    -15,
    -12,
    -9,
    -6,
    -3,
    0,
    2,
    4,
    5,
    7,
    9,
    11,
    13,
    15,
    16,
    18,
    20,
    22,
    24,
    25,
    27,
    29,
    31,
    33,
    35,
    36,
    38,
    40,
    42,
    44,
    45,
    47,
    49,
    51,
    53,
    55,
    56,
    58,
    60,
    62,
    64,
    65,
    67,
    69,
    71,
    73,
    75,
    76,
    78,
    80,
    82,
    84,
    85,
    87,
    89,
    91,
    93,
    95,
    96,
    98,
    100,
};

// *************************************************************************************************
// @fn          simpliciti_get_ed_data_callback
// @brief       Callback function to read end device data from acceleration sensor (if available)
//                              and trigger sending. Can be also be used to transmit other data at
// different packet rates.
//                              Please observe the applicable duty limit in the chosen ISM band.
// @param       none
// @return      none
// *************************************************************************************************
void simpliciti_get_ed_data_callback(void)
{
    static u8 packet_counter = 0;

    int x = 0;
    int y = 0;

    bool isInMotionRange = false;

    // Wait for next sample
    Timer_Delay(ConvertMillisecondsToTicks(5));

    // Read from sensor if EOC pin indicates new data (set in PORT2 ISR)
    if (SystemRequests.States.MeasureAcceleration)
    {
        // Clear flag
        SystemRequests.States.MeasureAcceleration = 0;

        // Get data from sensor
        Accelerator_GetData(SystemAccelerator.xyz);

        // Transmit only every 3rd data set (= 33 packets / second)
        if (packet_counter++ > 1)
        {
            // Reset counter
            packet_counter = 0;

            y = SystemAccelerator.xyz[1];

            // Align value around zero and determine -/+ range.
            if (y > 127)
                y = -(255 - y);

            if (MotionControl.Mode == MotionControlMode_Direction) {
                // Check if watch is in a valid horizontal "command area".
                if (y > -15 && y < 0)
                    isInMotionRange = true;
            }
            else if (MotionControl.Mode == MotionControlMode_Speed) {
                // Check if watch is in a valid horizontal "command area".
                if (y > -5 && y < 5)
                    isInMotionRange = true;
            }

            x = SystemAccelerator.xyz[0];

            if (!isInMotionRange)
                x = 250;

            simpliciti_data[1] = x;

            // Trigger packet sending
            simpliciti_flag |= SIMPLICITI_TRIGGER_SEND_DATA;

//                // Align value around zero and determine -/+ range.
//                if (x > 127)
//                    x = -(255 - x);
//
//                if (x < -33)
//                    x = -100;
//                else if (x > 55)
//                    x = 100;
//                else
//                    x = controlValue[x + 33];
//
//                // Skip values lower than 5%.
//                if (x < -5 || x > 5) {
//                    simpliciti_data[1] = x + 100;

//                    // Trigger packet sending
//                    simpliciti_flag |= SIMPLICITI_TRIGGER_SEND_DATA;
//                }


//                x = SystemAccelerator.xyz[0];
//
//                // Align value around zero and determine -/+ range.
//                if (x > 127)
//                    x = -(255 - x);
//
//                // Calculate percentage portion of the value relative to its possible range.
//                if (x < 0)
//                    x = (-x / 55.0) * 100;
//                else
//                    x = (-x / 40.0) * 100;
//
//                // Skip values lower than 5%.
//                if (x > -5 && x < 5)
//                    x = 0;
//
//                // Align values lower than -100% to exact -100%.
//                if (x < -100)
//                    x = -100;
//                // Align values greater than 100% to exact 100%.
//                else if (x > 100)
//                    x = 100;
//
//                if (x < 0) {
//                    // Align value to maximum possible motion range.
//                    // The motion range -60% to 100% (without alignment).
//                    x = (x / 60.0) * 100;
//
//                    // Align values lower than -100% to exact -100%.
//                    if (x < -100)
//                        x = -100;
//                }

                // Align value into transmit range.
    //            if (x < 0)
    //                x *= (-1);
    //            else
    //                x += 100;

//                simpliciti_data[1] = (u8)(x + 100);
//            }
        }
    }
}

