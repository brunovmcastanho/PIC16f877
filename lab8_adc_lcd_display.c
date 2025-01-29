#include <xc.h>

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF
#define _XTAL_FREQ 4000000

#define LCD_RS PORTEbits.RE0
#define LCD_EN PORTEbits.RE1
#define LCD_DATA_PORT PORTD

void configurar_LCD();
void lcd_enviar_comando(unsigned char comando);
void lcd_enviar_caractere(unsigned char caractere);
void lcd_exibir_texto(const char *texto);
void configurar_ADC();
unsigned int ler_ADC(unsigned char canal);
void lcd_mostrar_tensao(float tensao);

void main() {
    float tensao_lida;

    TRISD = 0x00;
    TRISB = 0x00;
    TRISE = 0x00;
    TRISA = 0xFF;

    configurar_LCD();
    configurar_ADC();

    lcd_exibir_texto("Tensao:");

    while (1) {
        unsigned int valor_ADC = ler_ADC(3);
        tensao_lida = (valor_ADC * 5.0) / 1023.0;
        lcd_enviar_comando(0xC0);
        lcd_mostrar_tensao(tensao_lida);
        __delay_ms(100);
    }
}

void configurar_LCD() {
    lcd_enviar_comando(0x38);
    lcd_enviar_comando(0x0C);
    lcd_enviar_comando(0x06);
    lcd_enviar_comando(0x01);
    __delay_ms(2);
}

void lcd_enviar_comando(unsigned char comando) {
    LCD_DATA_PORT = comando;
    LCD_RS = 0;
    LCD_EN = 1;
    __delay_ms(1);
    LCD_EN = 0;
}

void lcd_enviar_caractere(unsigned char caractere) {
    LCD_DATA_PORT = caractere;
    LCD_RS = 1;
    LCD_EN = 1;
    __delay_ms(1);
    LCD_EN = 0;
}

void lcd_exibir_texto(const char *texto) {
    while (*texto) {
        lcd_enviar_caractere(*texto++);
    }
}

void configurar_ADC() {
    ADCON0 = 0b10011001;
    ADCON1 = 0b10000010;
}

unsigned int ler_ADC(unsigned char canal) {
    ADCON0 &= 0xC5;
    ADCON0 |= (canal << 3);
    __delay_ms(2);
    ADCON0bits.GO_nDONE = 1;
    while (ADCON0bits.GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void lcd_mostrar_tensao(float tensao) {
    unsigned int valor_inteiro = (unsigned int)(tensao * 100);
    unsigned char digito_centena = valor_inteiro / 100;
    unsigned char digito_dezena = (valor_inteiro % 100) / 10;
    unsigned char digito_unidade = valor_inteiro % 10;

    lcd_enviar_caractere(digito_centena + '0');
    lcd_enviar_caractere('.');
    lcd_enviar_caractere(digito_dezena + '0');
    lcd_enviar_caractere(digito_unidade + '0');
    lcd_enviar_caractere(' ');
    lcd_enviar_caractere('V');
}
