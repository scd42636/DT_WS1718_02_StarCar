// -------------------------------------------------------------------------------------------------
// <copyright file="ButtonDefines.h" company="OTH Regensburg">
//     Copyright (c) OTH Regensburg, Algorithmen für Sensornetze. All rights reserved.
// </copyright>
// <autor>Dominik Scharnagl (MatNr. 3054541)</autor>
// -------------------------------------------------------------------------------------------------

#ifndef BUTTONDEFINES_H
#define BUTTONDEFINES_H

#include "Defines.h"


// Port, pins and interrupt resources for buttons
#define BUTTONS_IN              (P2IN)
#define BUTTONS_OUT             (P2OUT)
#define BUTTONS_DIR             (P2DIR)
#define BUTTONS_REN             (P2REN)
#define BUTTONS_IE              (P2IE)
#define BUTTONS_IES             (P2IES)
#define BUTTONS_IFG             (P2IFG)
#define BUTTONS_IRQ_VECT2       (PORT2_VECTOR)

// Button ports
#define BUTTON_STAR_PIN         (BIT2)
#define BUTTON_NUM_PIN          (BIT1)
#define BUTTON_UP_PIN           (BIT4)
#define BUTTON_DOWN_PIN         (BIT0)
#define BUTTON_BACKLIGHT_PIN    (BIT3)
#define ALL_BUTTONS             (BUTTON_STAR_PIN + BUTTON_NUM_PIN + BUTTON_UP_PIN + BUTTON_DOWN_PIN + BUTTON_BACKLIGHT_PIN)

// Button debounce time (msec)
#define BUTTONS_DEBOUNCE_TIME_IN        (5u)
#define BUTTONS_DEBOUNCE_TIME_OUT       (250u)
#define BUTTONS_DEBOUNCE_TIME_LEFT      (50u)

// Macros for button press detection
#define ButtonIsStarPressed          ((BUTTONS_IN & BUTTON_STAR_PIN) == BUTTON_STAR_PIN)
#define ButtonIsNumPressed           ((BUTTONS_IN & BUTTON_NUM_PIN) == BUTTON_NUM_PIN)
#define ButtonIsUpPressed            ((BUTTONS_IN & BUTTON_UP_PIN) == BUTTON_UP_PIN)
#define ButtonIsDownPressed          ((BUTTONS_IN & BUTTON_DOWN_PIN) == BUTTON_DOWN_PIN)
#define ButtonIsBacklightPressed     ((BUTTONS_IN & BUTTON_BACKLIGHT_PIN) == BUTTON_BACKLIGHT_PIN)
#define ButtonIsAnyPressed           ((BUTTONS_IN & ALL_BUTTONS) != 0)


typedef union
{
    struct
    {
        // Manual button events
        u16 HasStarPressed : 1;          // Short STAR button press
        u16 HasNumPressed : 1;           // Short NUM button press
        u16 HasUpPressed : 1;            // Short UP button press
        u16 HasDownPressed : 1;          // Short DOWN button press
        u16 HasBacklightPressed : 1;     // Short BACKLIGHT button press
    } States;
    u16 Flags;                           // Shortcut to all display flags (for reset)
} ButtonInfo;

extern volatile ButtonInfo SystemButtons;

#endif
