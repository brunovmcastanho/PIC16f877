#include <xc.h>

// Configura��es do microcontrolador
#pragma config FOSC = XT    // Oscilador XT (4 MHz)
#pragma config WDTE = OFF   // Watchdog Timer desativado
#pragma config PWRTE = OFF  // Power-up Timer desativado
#pragma config CP = OFF     // Prote��o de c�digo desativada

#define _XTAL_FREQ 4000000  // Frequ�ncia do oscilador (4 MHz)

// Defini��o dos pinos do LCD
#define LCD_RS PORTEbits.RE0
#define LCD_EN PORTEbits.RE1
#define LCD_DATA_PORT PORTD  // Dados do LCD conectados ao PORTD

// Defini��o dos bot�es
#define BOTAO_RESET PORTBbits.RB0  // Bot�o para resetar o contador

// Prot�tipos das fun��es
void configurar_LCD();
void lcd_enviar_comando(unsigned char comando);
void lcd_enviar_caractere(unsigned char caractere);
void lcd_exibir_texto(const char *texto);
void lcd_numero(unsigned int numero);
void configurar_pinos();
void configurar_timer1();
unsigned int ler_timer1();

void main() {
    unsigned int contador_anterior = 0;

    configurar_pinos();
    configurar_LCD();
    configurar_timer1();

    lcd_enviar_comando(0x80);  // In�cio da primeira linha
    lcd_exibir_texto("Contagem:");

    while (1) {
        // Verifica se o bot�o de reset foi pressionado
        if (BOTAO_RESET == 0) {
            __delay_ms(20);  // Debounce
            if (BOTAO_RESET == 0) {
                TMR1H = 0x00;  // Zera o Timer1 (byte alto)
                TMR1L = 0x00;  // Zera o Timer1 (byte baixo)
                lcd_enviar_comando(0xC0);  // In�cio da segunda linha
                lcd_exibir_texto("");
                while (BOTAO_RESET == 0);  // Espera soltar o bot�o
            }
        }

        // Atualiza o display se o contador mudou
        unsigned int contador_atual = ler_timer1();
            lcd_enviar_comando(0xC0);  // In�cio da segunda linha
            lcd_numero(contador_atual);
            contador_anterior = contador_atual;
        
    }
}

void configurar_pinos() {
    // Configura��o dos pinos
    TRISD = 0x00;         // PORTD como sa�da (dados do LCD)
    TRISE = 0x00;         // PORTE como sa�da (controle do LCD)
    TRISBbits.TRISB0 = 1; // RB0 como entrada (bot�o de reset)
    TRISCbits.TRISC0 = 1; // RC0 como entrada (fonte de clock para Timer1)

    PORTD = 0x00;
    PORTE = 0x00;

    // Ativar pull-up interno para PORTB
    OPTION_REGbits.nRBPU = 0;  // Habilita pull-ups globais em PORTB
}

void configurar_LCD() {
    __delay_ms(20);              // Tempo de estabiliza��o do LCD
    lcd_enviar_comando(0x38);    // Configura LCD para 2 linhas e 8 bits
    lcd_enviar_comando(0x0C);    // Liga display, sem cursor
    lcd_enviar_comando(0x06);    // Incrementa cursor
    lcd_enviar_comando(0x01);    // Limpa display
    __delay_ms(2);
}

void configurar_timer1() {
    T1CON = 0x87;  // Timer1 ligado, modo contador externo, borda de subida, prescaler 1:1
    TMR1H = 0x00;  // Zera o Timer1 (byte alto)
    TMR1L = 0x00;  // Zera o Timer1 (byte baixo)
}

// Fun��o para leitura sincronizada do Timer1
unsigned int ler_timer1() {
    unsigned char high, low;
    unsigned int valor;

    // Desabilita temporariamente o Timer1 para leitura consistente
    T1CONbits.TMR1ON = 0;  // Desliga o Timer1
    high = TMR1H;          // L� o byte alto
    low = TMR1L;           // L� o byte baixo
    T1CONbits.TMR1ON = 1;  // Liga o Timer1 novamente

    // Combina os dois bytes
    valor = ((unsigned int)high << 8) | (unsigned int)low;
    return valor;
}

void lcd_enviar_comando(unsigned char comando) {
    LCD_DATA_PORT = comando;
    LCD_RS = 0;
    LCD_EN = 1;
    __delay_us(40);
    LCD_EN = 0;
}

void lcd_enviar_caractere(unsigned char caractere) {
    LCD_DATA_PORT = caractere;
    LCD_RS = 1;
    LCD_EN = 1;
    __delay_us(40);
    LCD_EN = 0;
}

void lcd_exibir_texto(const char *texto) {
    while (*texto) {
        lcd_enviar_caractere(*texto++);
    }
}

void lcd_numero(unsigned int numero) {
    unsigned char milhar = (numero / 1000) % 10;
    unsigned char centena = (numero / 100) % 10;
    unsigned char dezena = (numero / 10) % 10;
    unsigned char unidade = numero % 10;

    lcd_enviar_caractere(milhar + '0');
    lcd_enviar_caractere(centena + '0');
    lcd_enviar_caractere(dezena + '0');
    lcd_enviar_caractere(unidade + '0');
}
