#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 20000000

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define RS PORTEbits.RE0
#define EN PORTEbits.RE1

// Protótipos
void lcd_data(unsigned char data);
void lcd_command(unsigned char cmd);
void lcd_string(const char *str, unsigned char num);
void lcd_initialise(void);
void lcd_set_cursor(unsigned char linha, unsigned char coluna);
unsigned char scan_keypad(void);

// Mapeamento do teclado (0-9, A-F)
const char tecla_map[] = {'0', '1', '2', '3', 
                          '4', '5', '6', '7', 
                          '8', '9', 'A', 'B', 
                          'C', 'D', 'E', 'F'};

void main(void)
{
    unsigned char tecla, linha = 1, coluna = 1;

    // Configuração dos pinos
    TRISC = 0xF0; // RC0-RC3 como saída (linhas), RC4-RC7 como entrada
    TRISB = 0x0F; // RB0-RB3 como entrada (colunas)
    TRISE = 0x00; // PORTE como saída
    TRISD = 0x00; // PORTD como saída para dados do LCD
    PORTC = 0x00; 
    PORTB = 0x00;

    lcd_initialise();
    lcd_command(0x01); // Limpa o display

    while (1)
    {
        tecla = scan_keypad();
        if (tecla != 0xFF) // Tecla válida
        {
            lcd_set_cursor(linha, coluna);
            lcd_data(tecla_map[tecla]);
            coluna++;

            // Verifica mudança de linha
            if (coluna > 16)
            {
                linha++;
                coluna = 1;
            }

            // Verifica se as duas linhas foram preenchidas
            if (linha > 2 & coluna > 1)
            {
                lcd_command(0x01); // Limpa o display
                linha = 1;
                coluna = 1;
            }

            __delay_ms(300); // Debounce
        }
    }
}

// Implementação das funções do LCD
void lcd_data(unsigned char data)
{
    PORTD = data;
    RS = 1;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_command(unsigned char cmd)
{
    PORTD = cmd;
    RS = 0;
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_string(const char *str, unsigned char num)
{
    for (unsigned char i = 0; i < num; i++)
    {
        lcd_data(str[i]);
    }
}

void lcd_initialise(void)
{
    lcd_command(0x38); // Modo 8 bits, 2 linhas
    lcd_command(0x06); // Incrementa cursor
    lcd_command(0x0C); // Display ON, cursor OFF
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

    for (linha = 0; linha < 4; linha++)
    {
        PORTC = ~(1 << linha); // Ativa uma linha por vez
        __delay_us(10);

        coluna = (PORTB & 0x0F); // Lê as colunas

        if (coluna != 0x0F) // Alguma tecla foi pressionada
        {
            __delay_ms(50); // Debounce
            coluna = (PORTB & 0x0F); // Lê novamente

            if (coluna != 0x0F)
            {
                switch (coluna)
                {
                    case 0x0E: return linha * 4 + 0; // Coluna 1
                    case 0x0D: return linha * 4 + 1; // Coluna 2
                    case 0x0B: return linha * 4 + 2; // Coluna 3
                    case 0x07: return linha * 4 + 3; // Coluna 4
                }
            }
        }
    }

    return 0xFF; // Nenhuma tecla pressionada
}
