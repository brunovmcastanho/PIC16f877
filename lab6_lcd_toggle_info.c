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

// Funções para o LCD
void lcd_comando(unsigned char cmd);
void lcd_escreve(unsigned char dado);
void lcd_inicializa(void);
void lcd_limpar(void);
void lcd_escreve_texto(const char* texto);
void lcd_set_cursor(unsigned char linha, unsigned char coluna);

void main(void) 
{
    int estado = 0;        
    int botao_anterior = 1; 

    TRISD = 0x00;  // PORTD como saída (conexão com o LCD)
    TRISB = 0x01;  // RB0 como entrada (tecla)
    TRISE = 0x00;  // PORTE como saída (controle RS e Enable)
    PORTD = 0x00;  // Zerar PORTD
    PORTE = 0x00;  // Zerar PORTE

    lcd_inicializa();

    while (1)
    {
        // Detecção da tecla com debounce
        if (tecla == 0 && botao_anterior == 1) {
            __delay_ms(50);
            if (tecla == 0) {
                estado++;  // Próximo estado
                if (estado > 2) estado = 0;  // Reinicia ciclo
            }
        }
        botao_anterior = tecla; 

        // Alterna o estado do display com base na variável estado
        if (estado == 0) {
            
            lcd_set_cursor(1, 1);  // Linha 1, Coluna 1
            lcd_escreve_texto("Nome: Bruno");
        } 
        else if (estado == 1) {
            lcd_set_cursor(2, 1);  // Linha 2, Coluna 1
            lcd_escreve_texto("RA: 2330776");
        } 
        else if (estado == 2) {
            lcd_limpar();
        }
    }
}

// Implementação das funções do LCD
void lcd_comando(unsigned char cmd) {
    PORTD = cmd;           // Comando no PORTD
    PORTEbits.RE0 = 0;     // RS = 0 para comando
    PORTEbits.RE1 = 1;     // Habilita o LCD
    __delay_ms(2);
    PORTEbits.RE1 = 0;     // Desabilita o LCD
}

void lcd_escreve(unsigned char dado) {
    PORTD = dado;          // Dados no PORTD
    PORTEbits.RE0 = 1;     // RS = 1 para dado
    PORTEbits.RE1 = 1;     // Habilita o LCD
    __delay_ms(2);
    PORTEbits.RE1 = 0;     // Desabilita o LCD
}

void lcd_inicializa(void) {
    __delay_ms(15);        // Espera para estabilizar
    lcd_comando(0x38);     // Modo 8 bits, 2 linhas
    lcd_comando(0x0C);     // Display ligado, cursor desligado
    lcd_comando(0x06);     // Incremento do cursor
    lcd_comando(0x01);     // Limpa display
    __delay_ms(2);
}

void lcd_limpar(void) {
    lcd_comando(0x01);     // Comando para limpar o display
    __delay_ms(2);
}

void lcd_escreve_texto(const char* texto) {
    while (*texto) {
        lcd_escreve(*texto++); // Escreve cada caractere
    }
}

void lcd_set_cursor(unsigned char linha, unsigned char coluna) {
    unsigned char posicao = 0;
    if (linha == 1) {
        posicao = 0x80 + (coluna - 1); // Linha 1
    } else if (linha == 2) {
        posicao = 0xC0 + (coluna - 1); // Linha 2
    }
    lcd_comando(posicao); // Define posição no LCD
}
