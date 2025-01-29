#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define RS PORTEbits.RE0
//#define RW PORTEbits.RE1
#define EN PORTEbits.RE1

// Funções do LCD
void lcd_data(unsigned char data);
void lcd_command(unsigned char cmd);
void lcd_string(const char *str, unsigned char num);
void lcd_initialise(void);
void lcd_set_cursor(unsigned char linha, unsigned char coluna);
unsigned char scan_keypad(void);

void main(void)
{
    unsigned char tecla, tecla_anterior = 0xFF;
    unsigned int cont = 0;
    char buffer[10];

    // Configurações dos pinos
    TRISC = 0x0F;  // RC0-RC3 como saída (linhas), RC4-RC7 como entrada (não utilizado)
    TRISB = 0x0F;  // RB0-RB3 como entrada (colunas), RB4-RB7 como saída (não utilizado)
    TRISE = 0x00;  // PORTE como saída para controle do LCD
    TRISD = 0x00;  // PORTD como saída para dados do LCD
    PORTC = 0x00;  // Inicializa PORTC com zero
    PORTB = 0x00;  // Inicializa PORTB com zero

    lcd_initialise();
    lcd_command(0x80);
    lcd_string("Contador cliques", 15);

    while (1)
    {
        tecla = scan_keypad();

        if (tecla != 0xFF)
        {
            if (tecla == 0) // Pressionou a primeira tecla
            {
                cont++;
            }
            else // Qualquer outra tecla
            {
                cont = 0;
            }

            // Atualiza o display com o valor do contador
            lcd_set_cursor(2, 1);
            lcd_string("N:", 2);
            lcd_set_cursor(2, 4); // Alinhado à direita
            sprintf(buffer, "%04d", cont);
            lcd_string(buffer, 4);
        }

        tecla_anterior = tecla;
    }
}

// Implementação das funções do LCD
void lcd_data(unsigned char data)
{
    PORTD = data;
    RS = 1;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTD = cmd;
    RS = 0;
    EN = 1;
    __delay_ms(5);
    EN = 0;
}

void lcd_string(const char *str, unsigned char num)
{
    unsigned char i;
    for (i = 0; i < num; i++)
    {
        lcd_data(str[i]);
    }
}

void lcd_initialise(void)
{
    lcd_command(0x38); // Modo 8 bits, 2 linhas
    lcd_command(0x06); // Incremento do cursor
    lcd_command(0x0C); // Display ligado, cursor desligado
    lcd_command(0x01); // Limpa display
    __delay_ms(5);
}

void lcd_set_cursor(unsigned char linha, unsigned char coluna)
{
    unsigned char posicao = (linha == 1) ? 0x80 : 0xC0;
    posicao += (coluna - 1);
    lcd_command(posicao);
}

// Função para escanear o teclado matricial
unsigned char scan_keypad(void)
{
    unsigned char linha, coluna;

    // Configura RC0-RC3 como saídas (linhas) e RB0-RB3 como entradas (colunas)
    TRISC = 0xF0; // RC0-RC3 como saída, RC4-RC7 como entrada (não utilizado)
    TRISB = 0x0F; // RB0-RB3 como entrada

    for (linha = 0; linha < 4; linha++)
    {
        PORTC = ~(1 << linha); // Ativa uma linha de cada vez, enviando 0 (nível baixo) para a linha correspondente, outras em nível alto
        __delay_us(10); // Tempo menor para estabilizar o sinal

        coluna = (PORTB & 0x0F); // Lê as colunas (RB0-RB3)
        
        if (coluna != 0x0F)    // Alguma coluna foi ativada, ou seja, alguma tecla foi pressionada
        { 
            __delay_ms(50);
            
            coluna = (PORTB & 0x0F); // Lê as colunas (RB0-RB3)
            
             if (coluna != 0x0F)    // Alguma coluna foi ativada, ou seja, alguma tecla foi pressionada
        {
            // Debounce: verifica se o mesmo valor persiste após um pequeno atraso
            __delay_ms(10); 
     
                switch (coluna)
                {
                    case 0x0E: return linha * 4 + 0;     // Coluna 1
                    case 0x0D: return linha * 4 + 1;     // Coluna 2
                    case 0x0B: return linha * 4 + 2;     // Coluna 3
                    case 0x07: return linha * 4 + 3;     // Coluna 4
                    default: return 0xFF;                // Nenhuma tecla válida
                }
                
            }
        }
    }

    return 0xFF; // Nenhuma tecla pressionada
}
