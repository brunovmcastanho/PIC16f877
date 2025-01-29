#include <xc.h>

// Configurações do microcontrolador
#pragma config FOSC = HS    // Oscilador de alta velocidade (16 MHz)
#pragma config WDTE = OFF   // Watchdog Timer desativado
#pragma config PWRTE = OFF  // Power-up Timer desativado
#pragma config CP = OFF     // Proteção de código desativada

#define _XTAL_FREQ 16000000  // Define a frequência do oscilador (16 MHz)

volatile unsigned int contador = 0;  

void configurar_Timer0() {
    OPTION_REGbits.T0CS = 0; 
    OPTION_REGbits.PSA = 0;  
    OPTION_REGbits.PS = 0b111;
    
    TMR0 = 0;
  
    INTCONbits.T0IE = 1; 
    INTCONbits.T0IF = 0; 
    INTCONbits.GIE = 1;   
}

void __interrupt() isr(void) {
    if (INTCONbits.T0IF) {
   
        TMR0 = 0;  
        contador++;

        if (contador >= 78) { 
            PORTCbits.RC2 = ~PORTCbits.RC2; 
            contador = 0;                   
        }

        INTCONbits.T0IF = 0; 
    }
}

void main() {
    
    TRISCbits.TRISC2 = 0; 
    PORTCbits.RC2 = 0;    

    configurar_Timer0(); 

    while (1) {
       
    }
}
