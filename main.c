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

unsigned char LED_Display;  // 8-bit variable

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

void main(void)
{
    // Configure the oscillator for the device
    ConfigureOscillator();
    // Initialize I/O and Peripherals for application
    InitApp();

    LEDDirections Direccion = IZQ2DER;
    LED_Display = 1; // inicializacion
    BOOL BotonPresionado = FALSE;
    
    while(1)
    {
        if(Direccion == IZQ2DER)
        {
            LED_Display <<= 1;          // rotate display by 1 from 0 to 7
            if (LED_Display == 0)
                LED_Display = 1;        // rotated bit out, so set bit 0
        }
        else{
            LED_Display >>= 1;          // rotate display by 1 from 0 to 7
            if (LED_Display == 0)
                LED_Display = 0x80;        // rotated bit out, so set bit 0            
        }
        PORTD = LED_Display;
        SpiTxRx(LED_Display);
        do
        {
            if (Switch_Pin == 1)
            { // busca el momento en que se suelta el boton
                BotonPresionado = FALSE;
            }
            else if (BotonPresionado == FALSE) // && (Switch_Pin == 0)
            { // se presiono el boton de cambio de direccion
                BotonPresionado = TRUE;
                // cambio de direccion
                if(Direccion == IZQ2DER)
                    Direccion = DER2IZQ;
                else
                    Direccion = IZQ2DER;
                }
        } while (INTCONbits.TMR0IF == 0); // Ciclo bobo de espera
        
        // Tiempo completado
        INTCONbits.TMR0IF = 0; // Borramos bandera
        // Establecemos el valor inicial del contador del tmr0
        TMR0H = 0b00001111; // MSB
        TMR0L = 0b00000000; // LSB = 0
    }
}

