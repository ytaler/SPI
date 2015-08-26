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

#endif

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/
void InitApp(void)
{
    // Configuracion de puertos E/S. Pines utilizados:
    // Pin 24 RA5/AN4/SS/HLVDIN/C2OUT --> Salida --> Slave Select ? SS
    // Pin 37 RC3/SCK/SCL --> Salida --> Serial Clock ? SCK/SCL
    // Pin 42 RC4/SDI/SDA --> Entrada --> Serial Data In ? SDI/SDA
    // Pin 43 RC5/SDO --> Salida -->Serial Data Out ? SDO
    
    TRISA=0b11011111; // SS debe estar como salida, el resto como entrada
    RBPU = 0; // habilita resistencias pull-up en PORTB
	WPUB0 = 1; // seleccionamos conectar la resistencia pull-up en RB0
    TRISB=0b11111111; // todo como entrada
    TRISC=0b11011111; // SDO como salida, SDI, SCK y el resto como entrada
    TRISD=0b00000000; // Todo como salida corresponden a los leds
    PORTA=0; // Limpiamos el puerto
    PORTC=0; // Limpiamos el puerto
    PORTD=0; // Limpiamos el puerto
    
    // Inicializacion de perifericos
    ANSEL = 0;	//turn off all other analog inputs
	ANSELH = 0;
    
    // Configuracion UART - Revisar
    //BRGH=1;//Modo high speed
    //BRG16=1;//tamaño 16 bits
    //SPBRG=34; //Velocidad de transmision 115200 con un FCLK de 8 MHz
    //SPEN=1; //UART port enable
    //CREN=1; //RX enable

    // Configuracion de SPI para TX
    SMP=0; // Sample bit = 0 --> Inserta bit de muestra a la mitad del tiempo de salida
    CKE=1; // Clock Select bit = 1 --> Output data changes on clock transition from active to idle
    SSPCON1=0; // Borrar valores de config. Toma todo por default.
    // bit 7 WCOL: Write Collision Detect bit --> No collision
    // bit 6 SSPOV: Receive Overflow Indicator bit --> No overflow
    SSPEN=1; // bit 5 Habilita y configura los pines seriales SCK, SDO, SDI, y SS.
    // bit 4 CKP: Clock Polarity Select bit --> Idle state for clock is a low level
    // bit 3-0 SSPM<3:0>: Synchronous Serial Port Mode Select bits --> 0000 = SPI Master mode, clock = F OSC /4
    SSPM0=1;
    SSPM2=1;
    // Configure the IPEN bit (1=on) in RCON to turn on/off int priorities
    // Interrupciones
    GIE=0; // Deshabilitadas
    
    INTCONbits.TMR0IF = 0;          // clear roll-over interrupt flag
    T0CON = 0b00000001;             // prescale 1:4 - about 1 second maximum delay.
    TMR0H = 0;                      // clear timer - always write upper byte first
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;           // start timer
}

// Funcion que envia y recibe los datos por SPI
unsigned char SpiTxRx (unsigned char data)
{
SSPBUF = data; // comienza la escritura con el dato recibido
while(!SSPIF); // espera la bandera que indica fin de tx/rx
SSPIF=0; // se borra la bandera
return SSPBUF; // en caso que el receptor responda, retorna la respuesta
}