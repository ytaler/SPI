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

    ANSEL = 0x00; // Apagar entradas analogicas
    ANSELH = 0x00; // Apagar entradas analogicas    
    TRISA=0b11011111; // SS debe estar como salida, el resto como entrada
    PORTA=0; // Limpiamos el puerto
    
    INTCON2bits.RBPU = 0; // habilita resistencias pull-up en PORTB
	WPUBbits.WPUB0 = 1; // seleccionamos conectar la resistencia pull-up en RB0
    TRISB=0b11111111; // todo como entrada

    TRISC=0b11010111; // SDO y SCK como salida, SDI y el resto como entrada
    PORTC=0; // Limpiamos el puerto
    
    TRISD=0b00000000; // Todo como salida corresponden a los leds
    PORTD=0; // Limpiamos el puerto
   
    InitSPI();
    InitIRQ();
}

// Funcion que envia y recibe los datos por SPI
unsigned char SpiTxRx (unsigned char data)
{
    SSPCON1bits.WCOL = 0;
    SSPBUF = data; // comienza la escritura con el dato recibido
    while(!PIR1bits.SSPIF); // espera la bandera que indica fin de tx/rx
    PIR1bits.SSPIF=0; // se borra la bandera
    return SSPBUF; // en caso que el receptor responda, retorna la respuesta
}

// Inicializacion de SPI
void InitSPI(void){
    // Configuracion de SPI para TX
    // BF RCinprocess_TXcomplete; UA dontupdate; SMP Sample At Middle; P stopbit_notdetected; S startbit_notdetected; R_nW write_noTX; CKE Idle to Active; D_nA lastbyte_address; 
    SSPSTAT = 0x00;
    //CKE=1; // Clock Select bit = 1 --> Output data changes on clock transition from active to idle
    SSPCON1 = 0x00; // Borrar valores de config. Toma todo por default.
    // bit 7 WCOL: Write Collision Detect bit --> No collision
    // bit 6 SSPOV: Receive Overflow Indicator bit --> No overflow
    SSPCON1bits.SSPEN=1; // bit 5 Habilita y configura los pines seriales SCK, SDO, SDI, y SS.
    // bit 4 CKP: Clock Polarity Select bit --> Idle state for clock is a low level
    // bit 3-0 SSPM<3:0>: Synchronous Serial Port Mode Select bits --> 0000 = SPI Master mode, clock = F OSC /4
}

// Inicializa interrupciones
void InitIRQ(void){
    // Interrupciones
    // Set up Interrupts for timer
    INTCONbits.TMR0IF = 0;          // clear roll-over interrupt flag
    INTCON2bits.TMR0IP = 0;         // Timer0 is low priority interrupt
    INTCONbits.TMR0IE = 1;          // enable the Timer0 interrupt.
    // Set up timer itself
    T0CON = 0b00000001;             // prescale 1:4 - about 1 second maximum delay.
    TMR0H = 0;                      // clear timer - always write upper byte first
    TMR0L = 0;
    T0CONbits.TMR0ON = 1;           // start timer    
    
    // Set up switch interrupt on INT0
    INTCON2bits.INTEDG0 = 0;    // interrupt on falling edge of INT0 (switch pressed)
    INTCONbits.INT0IF = 0;      // ensure flag is cleared
    INTCONbits.INT0IE = 1;      // enable INT0 interrupt
    // NOTE: INT0 is ALWAYS a high priority interrupt

    // Set up global interrupts
    RCONbits.IPEN = 1;          // Enable priority levels on interrupts
    INTCONbits.GIEL = 1;        // Low priority interrupts allowed
    INTCONbits.GIEH = 1;        // Interrupting enabled.   
}

// Inicializa UART
void InitUART(void){
    // Configuracion UART - Revisar
    //BRGH=1;//Modo high speed
    //BRG16=1;//tamaño 16 bits
    //SPBRG=34; //Velocidad de transmision 115200 con un FCLK de 8 MHz
    //SPEN=1; //UART port enable
    //CREN=1; //RX enable    
}