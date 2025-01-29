#include <xc.h>

// Configurações do microcontrolador
#pragma config FOSC = XT    // Oscilador XT (4 MHz)
#pragma config WDTE = OFF   // Watchdog Timer desativado
#pragma config PWRTE = OFF  // Power-up Timer desativado
#pragma config CP = OFF     // Proteção de código desativada

#define _XTAL_FREQ 4000000  // Frequência do oscilador

// Variáveis globais
volatile unsigned char led_estado_rd0 = 0;
volatile unsigned char led_estado_rd1 = 0;
volatile unsigned char led_estado_rd2 = 0;
volatile unsigned char rb7_estado_anterior = 0;
volatile unsigned int timer2_contador = 0;

void configurar_pinos(void);
void configurar_timer2(void);
void configurar_interrupcoes(void);

void __interrupt() interrupcao(void) {
    if (PIR1bits.TMR2IF) {  // Timer2
        PIR1bits.TMR2IF = 0;
        if (++timer2_contador >= 80) {
            timer2_contador = 0;
            led_estado_rd0 = ~led_estado_rd0;
            PORTDbits.RD0 = led_estado_rd0;
        }
    }

    if (INTCONbits.INTF) {  // RB0
        INTCONbits.INTF = 0;
        led_estado_rd1 = ~led_estado_rd1;
        PORTDbits.RD1 = led_estado_rd1;
    }

    if (INTCONbits.RBIF) {  // RB7
        INTCONbits.RBIF = 0;
        if (PORTBbits.RB7 != rb7_estado_anterior) {
            led_estado_rd2 = ~led_estado_rd2;
            PORTDbits.RD2 = led_estado_rd2;
            rb7_estado_anterior = PORTBbits.RB7;
        }
    }
}

void main(void) {
    configurar_pinos();
    configurar_timer2();
    configurar_interrupcoes();
    while (1);
}

void configurar_pinos(void) {
    TRISD = 0x00;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB7 = 1;
    PORTD = 0x00;
    rb7_estado_anterior = PORTBbits.RB7;
}

void configurar_timer2(void) {
    PR2 = 255;
    T2CONbits.T2CKPS = 0b11;    // Prescaler 1:16
    T2CONbits.TOUTPS = 0b1111;  // Pós-escaler 1:16
    T2CONbits.TMR2ON = 1;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
}

void configurar_interrupcoes(void) {
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    OPTION_REGbits.INTEDG = 1;
    INTCONbits.INTE = 1;
    INTCONbits.INTF = 0;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    OPTION_REGbits.nRBPU = 0;
}
