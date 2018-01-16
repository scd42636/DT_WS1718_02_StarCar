#include "Defines.h"
#include "SystemDefines.h"


// *************************************************************************************************
// Include section
#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "simpliciti.h"


// *************************************************************************************************
// Defines section
#define TIMEOUT                                 (10u)

// *************************************************************************************************
// Prototypes section


// *************************************************************************************************
// Extern section
extern uint8_t sInit_done;

// SimpliciTI has no low power delay function, so we have to use ours
extern void Timer_Delay(u16 ticks);


// *************************************************************************************************
// Global Variable section
static linkID_t sLinkID1;



// *************************************************************************************************
// @fn          simpliciti_link
// @brief       Init hardware and try to link to access point.
// @param       none
// @return      unsigned char		0 = Could not link, timeout or external cancel.
//									1 = Linked successful.
// *************************************************************************************************
unsigned char simpliciti_link(void)
{
    uint8_t timeout;
    addr_t lAddr;
    uint8_t i;
    uint8_t pwr;

    // Configure timer
    BSP_InitBoard();

    // Change network address to value set in calling function
    for (i = 0; i < NET_ADDR_SIZE; i++)
    {
        lAddr.addr[i] = SystemData.EndDeviceAddress[i];
    }
    SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &lAddr);

    // Set flag
    simpliciti_flag = SIMPLICITI_STATUS_LINKING;

    /* Keep trying to join (a side effect of successful initialization) until
     * successful. Toggle LEDS to indicate that joining has not occurred.
     */
    timeout = 0;
    while (SMPL_SUCCESS != SMPL_Init(0))
    {
        NWK_DELAY(1000);

#ifdef USE_WATCHDOG
        // Service watchdog
        WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK + WDTCNTCL;
#endif

        // Stop connecting after defined numbers of seconds (15)
        if (timeout++ > TIMEOUT)
        {
            // Clean up SimpliciTI stack to enable restarting
            sInit_done = 0;
            simpliciti_flag = SIMPLICITI_STATUS_ERROR;
            return (0);
        }

        // Break when flag bit SIMPLICITI_TRIGGER_STOP is set
        if (getFlag(simpliciti_flag, SIMPLICITI_TRIGGER_STOP))
        {
            // Clean up SimpliciTI stack to enable restarting
            sInit_done = 0;
            return (0);
        }
    }

    // Set output power to +3.3dmB
    pwr = IOCTL_LEVEL_2;
    SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SETPWR, &pwr);

    /* Unconditional link to AP which is listening due to successful join. */
    timeout = 0;
    while (SMPL_SUCCESS != SMPL_Link(&sLinkID1))
    {
        NWK_DELAY(1000);

#ifdef USE_WATCHDOG
        // Service watchdog
        WDTCTL = WDTPW + WDTIS__512K + WDTSSEL__ACLK + WDTCNTCL;
#endif

        // Stop linking after timeout
        if (timeout++ > TIMEOUT)
        {
            // Clean up SimpliciTI stack to enable restarting
            sInit_done = 0;
            simpliciti_flag = SIMPLICITI_STATUS_ERROR;
            return (0);
        }

        // Exit when flag bit SIMPLICITI_TRIGGER_STOP is set
        if (getFlag(simpliciti_flag, SIMPLICITI_TRIGGER_STOP))
        {
            // Clean up SimpliciTI stack to enable restarting
            sInit_done = 0;
            return (0);
        }
    }
    simpliciti_flag = SIMPLICITI_STATUS_LINKED;

    return (1);
}

// *************************************************************************************************
// @fn          simpliciti_main_tx_only
// @brief       Get data through callback. Transfer data when external trigger is set.
// @param       none
// @return      none
// *************************************************************************************************
void simpliciti_main_tx_only(void)
{
    while (1)
    {
        // Get end device data from callback function
        simpliciti_get_ed_data_callback();

        // Send data when flag bit SIMPLICITI_TRIGGER_SEND_DATA is set
        if (getFlag(simpliciti_flag, SIMPLICITI_TRIGGER_SEND_DATA))
        {
            // Get radio ready. Wakes up in IDLE state.
            SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);

            // Control value and control mode packets are 2 bytes long
            SMPL_SendOpt(sLinkID1, simpliciti_data, 2, SMPL_TXOPTION_NONE);

            // Put radio back to SLEEP state
            SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, 0);

            clearFlag(simpliciti_flag, SIMPLICITI_TRIGGER_SEND_DATA);
        }

        // Exit when flag bit SIMPLICITI_TRIGGER_STOP is set
        if (getFlag(simpliciti_flag, SIMPLICITI_TRIGGER_STOP))
        {
            // Clean up SimpliciTI stack to enable restarting
            sInit_done = 0;
            break;
        }
    }
}

