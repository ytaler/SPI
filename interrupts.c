/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>    /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "user.h"

#endif

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

static unsigned char LED_Display=1;  // 8-bit variable
static LEDDirections Direccion = IZQ2DER;

/* High-priority service */

#if defined(__XC) || defined(HI_TECH_C)
void interrupt high_isr(void)
#elif defined (__18CXX)
#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif

{  
    // Check for INT0 interrupt
    if (INTCONbits.INT0IF)
    {
        // clear (reset) flag
        INTCONbits.INT0IF = 0;

        // change directions
        if (Direccion == IZQ2DER)
        {
            Direccion = DER2IZQ;     // change direction
        }
        else // (Direction == RIGHT2LEFT)
        {
            Direccion = IZQ2DER;     // change direction
        }
        
    }
}

/* Low-priority interrupt routine */
#if defined(__XC) || defined(HI_TECH_C)
void low_priority interrupt low_isr(void)
#elif defined (__18CXX)
#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr(void)
#else
#error "Invalid compiler selection for implemented ISR routines"
#endif
{
    // Check for Timer0 Interrupt
    if  (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;          // clear (reset) flag

        // Take an ADC conversion and use it to set Timer0
        TMR0H = 0b00010000;
        TMR0L = 0;                  // LSB = 0

        // update display variable
        if (Direccion == IZQ2DER)
        {
            LED_Display <<= 1;          // rotate display by 1 from 0 to 7
            if (LED_Display == 0)
                LED_Display = 1;        // rotated bit out, so set bit 0
        }
        else // (Direction == RIGHT2LEFT)
        {
            LED_Display >>= 1;          // rotate display by 1 from 7 to 0
            if (LED_Display == 0)
                LED_Display = 0x80;     // rotated bit out, so set bit 7
        }
        PORTD=LED_Display;
        SpiTxRx(LED_Display);
    }
}
