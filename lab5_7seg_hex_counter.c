#include <xc.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define tecla PORTBbits.RB0
#define c0 PORTCbits.RC0

unsigned char segment[] = {0x02, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x19, 0x11, 0xC1, 0x63, 0x85, 0x61, 0x71};

void main(void) 
{
    int var = 0;  // 0 para contagem crescente, 1 para contagem decrescente
    int botao_anterior = 1;  // Estado inicial do botão (1 = não pressionado)
    int i = 0;  // Inicializa o índice fora do loop para não reiniciar

    TRISD = 0x00;
    TRISA = 0x00;
    TRISB = 0x01;  // Configura RB0 como entrada
    PORTAbits.RA1 = 0;

    while (1)
    {
        // Detecta a borda de descida (botão pressionado) para inverter o sentido
        if (tecla == 0 && botao_anterior == 1) {
            var = !var;  // Alterna o sentido da contagem
            __delay_ms(50);  // Anti-repique
        }

        botao_anterior = tecla;  // Atualiza o estado anterior do botão

        // Exibe o valor atual no display
        PORTD = segment[i];
        __delay_ms(1000);

        // Atualiza o índice com base no sentido
        if (var == 0) {
            i++;  // Contagem crescente
            if (i > 15) i = 0;  // Volta ao início quando atinge o limite
        } else {
            i--;  // Contagem decrescente
            if (i < 0) i = 15;  // Volta ao final quando atinge o limite
        }
    }
    return;
}
