#include <xc.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit
#pragma config BOREN = OFF      // Brown-out Reset Enable bit
#pragma config LVP = ON         // Low-Voltage Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define tecla PORTBbits.RB0


unsigned char passos[] = {0x09, 0x0A, 0x06, 0x05};  // Sequência completa

void main(void) 
{
    int sentido = 0;        
    int botao_anterior = 1; 
    int i = 0;               

    TRISD = 0x00;          
    TRISB = 0x01;           
    PORTD = 0x00;            


    while (1)
    {
        
        if (tecla == 0 && botao_anterior == 1) {
            __delay_ms(50);
            if (tecla == 0) {  
                sentido = !sentido; 
            }
        }
        botao_anterior = tecla; 

      
        PORTD = passos[i];

    
        if (sentido == 0) {
            i++;  
            if (i >= 4) i = 0;  
        } else {
            i--; 
            if (i < 0) i = 3; 
        }

        __delay_ms(70); 
    }
}
