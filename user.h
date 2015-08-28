/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

typedef enum { IZQ2DER,
               DER2IZQ} LEDDirections;
typedef enum {FALSE, TRUE} BOOL;

/** D E F I N I T I O N S ****************************************************/
#define Switch_Pin      PORTBbits.RB0

/******************************************************************************/
/* User Function Prototypes                                                   */
void InitApp(void); // Inicializacion del pines e/s y perifericos
void InitSPI(void); // Inicializa modulo SPI
void InitIRQ(void); // Inicializa interrupciones
void InitUART(void); // Inicializa UART
unsigned char SpiTxRx (unsigned char data); // Envia y recibe los datos por SPI