/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>        /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>       /* HiTech General Include File */
#elif defined(__18CXX)
    #include <p18cxxx.h>   /* C18 General Include File */
#endif

#if defined(__XC) || defined(HI_TECH_C)

#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#endif

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */

/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    // Configure the oscillator for the device
    ConfigureOscillator();
    // Initialize I/O and Peripherals for application
    InitApp();
    
    while(1)
    {
        PORTD = SpiTxRx(0xFF);
        do
            ;
        while (INTCONbits.TMR0IF == 0); // Ciclo bobo de espera
        
        // Tiempo completado
        INTCONbits.TMR0IF = 0; // Borramos bandera
        // Establecemos el valor inicial del contador del tmr0
        TMR0H = 0b00100000; // MSB
        TMR0L = 0b00000000; // LSB = 0
    }
}