#include <xc.h>
#include <stdint.h>

// CONFIGURAÇÃO DO HARDWARE
#pragma config FOSC  = HS
#pragma config WDTE  = OFF
#pragma config PWRTE = ON
#pragma config BOREN = ON
#pragma config LVP   = OFF
#pragma config CPD   = OFF
#pragma config WRT   = OFF
#pragma config CP    = OFF

#define _XTAL_FREQ 20000000UL   // Frequência do clock

// DEFINIÇÕES DO LCD
#define LCD_RS     RE0
#define LCD_EN     RE1
#define LCD_D4     RD4
#define LCD_D5     RD5
#define LCD_D6     RD6
#define LCD_D7     RD7

#define LCD_RS_DIR TRISE0
#define LCD_EN_DIR TRISE1
#define LCD_D4_DIR TRISD4
#define LCD_D5_DIR TRISD5
#define LCD_D6_DIR TRISD6
#define LCD_D7_DIR TRISD7

// DEFINIÇÕES DO TECLADO 4x4
#define KEYPAD_ROW0   RC0
#define KEYPAD_ROW1   RC1
#define KEYPAD_ROW2   RC2
#define KEYPAD_ROW3   RC3

#define KEYPAD_COL0   RB0
#define KEYPAD_COL1   RB1
#define KEYPAD_COL2   RB2
#define KEYPAD_COL3   RB3

// PROTÓTIPOS
void LCD_Init(void);
void LCD_Cmd(unsigned char cmd);
void LCD_Char(char data);
void LCD_String(const char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Clear(void);

void Keypad_Init(void);
char Keypad_GetChar(void);

void UART_Init(void);
void UART_SendChar(char data);
void UART_SendString(const char *str);

void __interrupt() ISR(void);  // Declaração de interrupção

// VARIÁVEIS GLOBAIS
volatile char uart_received_data[17]; // Buffer para receber string UART
volatile uint8_t uart_index = 0;
volatile uint8_t uart_data_ready = 0;

// MAPA DO TECLADO
static const char keypad_map[4][4] = 
{
    {'0','1','2','3'},
    {'4','5','6','7'},
    {'8','9','A','B'},
    {'C','D','E','F'}
};

void main(void)
{
    char key_pressed;

    // Inicializações
    LCD_Init();
    Keypad_Init();
    UART_Init();

    INTCONbits.GIE = 1;   // Habilita interrupções globais
    INTCONbits.PEIE = 1;  // Habilita interrupções periféricas

    LCD_Clear();
    LCD_SetCursor(1, 1);
    LCD_String("Recebendo:"); // Mensagem inicial no LCD

    while(1)
    {
        // TECLADO -> UART (Apenas envia via UART)
        key_pressed = Keypad_GetChar();
        UART_SendChar(key_pressed); // Envia a tecla pressionada via UART
      
    }
}

// INTERRUPÇÃO UART
void __interrupt() ISR(void)
{
    if(PIR1bits.RCIF) // Verifica se há dado recebido pela UART
    {
        char data = RCREG; // Lê o dado
        if (data == '\n' || uart_index >= 16) // Se receber quebra de linha ou buffer cheio
        {
            uart_received_data[uart_index] = '\0'; // Finaliza a string
            uart_index = 0;  // Reseta o índice
            uart_data_ready = 1; // Sinaliza que a string está pronta
        }
        else
        {
            uart_received_data[uart_index++] = data; // Armazena o caractere no buffer
        }
            LCD_Clear();
            LCD_SetCursor(1, 1);
            LCD_String("Recebido:");
            LCD_SetCursor(2, 1);
            LCD_String(uart_received_data); // Exibe a string completa no LCD
    }
}

// UART
void UART_Init(void)
{
    TRISC6 = 1;   // TX como saída
    TRISC7 = 1;   // RX como entrada
    
    SPBRG = 31;   // Baud rate 9600 para Fosc=20MHz
    TXSTAbits.SYNC = 0;  // Assíncrono
    TXSTAbits.BRGH = 1;  // High speed
    RCSTAbits.SPEN = 1;  // Habilita UART
    TXSTAbits.TXEN = 1;  // Habilita transmissão
    RCSTAbits.CREN = 1;  // Habilita recepção
    PIE1bits.RCIE = 1;   // Habilita interrupção de RX
}

void UART_SendChar(char data)
{
    while(!TXIF); // Espera buffer de transmissão ficar livre
    TXREG = data; // Envia o caractere
}

void UART_SendString(const char *str)
{
    while(*str)
    {
        UART_SendChar(*str++);
    }
}

// LCD
void LCD_Init(void)
{
    LCD_RS_DIR = LCD_EN_DIR = LCD_D4_DIR = LCD_D5_DIR = LCD_D6_DIR = LCD_D7_DIR = 0;

    __delay_ms(20); 
    LCD_Cmd(0x03); __delay_ms(5);
    LCD_Cmd(0x03); __delay_us(200);
    LCD_Cmd(0x03); __delay_us(200);
    LCD_Cmd(0x02); LCD_Cmd(0x28); LCD_Cmd(0x0C);
    LCD_Cmd(0x06); LCD_Cmd(0x01);
    __delay_ms(2);
}

void LCD_Cmd(unsigned char cmd)
{
    LCD_RS = 0;
    PORTD = (PORTD & 0x0F) | (cmd & 0xF0);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0;

    PORTD = (PORTD & 0x0F) | ((cmd << 4) & 0xF0);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0;

    __delay_ms(2);
}

void LCD_Char(char data)
{
    LCD_RS = 1;
    PORTD = (PORTD & 0x0F) | (data & 0xF0);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0;

    PORTD = (PORTD & 0x0F) | ((data << 4) & 0xF0);
    LCD_EN = 1; __delay_us(10); LCD_EN = 0;

    __delay_ms(2);
}

void LCD_String(const char *str)
{
    while(*str)
    {
        LCD_Char(*str++); // Envia cada caractere para o LCD
    }
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 1) ? 0x80 + (col - 1) : 0xC0 + (col - 1);
    LCD_Cmd(address);
}

void LCD_Clear(void)
{
    LCD_Cmd(0x01);
    __delay_ms(2);
}

// TECLADO 4x4
void Keypad_Init(void)
{
    TRISC = 0xF0;  // Linhas como saída, colunas como entrada
    OPTION_REGbits.nRBPU = 0; 
}

char Keypad_GetChar(void)
{
    while(1)
    {
        for(uint8_t row = 0; row < 4; row++)
        {
            PORTC = (unsigned char)(~(1 << row));
            __delay_us(50);

            if(KEYPAD_COL0 == 0) { while(KEYPAD_COL0==0); return keypad_map[row][0]; }
            if(KEYPAD_COL1 == 0) { while(KEYPAD_COL1==0); return keypad_map[row][1]; }
            if(KEYPAD_COL2 == 0) { while(KEYPAD_COL2==0); return keypad_map[row][2]; }
            if(KEYPAD_COL3 == 0) { while(KEYPAD_COL3==0); return keypad_map[row][3]; }
        }
    }
}
