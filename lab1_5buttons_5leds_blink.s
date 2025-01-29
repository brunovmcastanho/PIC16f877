; Projeto 1 - Exemplo de programa em assembly
; Descri��o: Controle de 5 LEDs com 5 bot�es

; ------------------- Configura��o dos FUSE BITS ------------------
            PROCESSOR 16F877
            #include <xc.inc>

;Configura��o do 'hardware' do microcontrolador
; CONFIG
      CONFIG  FOSC = HS             ; Oscillator Selection bits (HS oscillator)
      CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled)
      CONFIG  PWRTE = OFF           ; Power-up Timer Enable bit (PWRT disabled)
      CONFIG  CP = OFF              ; FLASH Program Memory Code Protection bits (Code protection off)
      CONFIG  BOREN = OFF           ; Brown-out Reset Enable bit (BOR disabled)
      CONFIG  LVP = OFF             ; Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
      CONFIG  CPD = OFF             ; Data EE Memory Code Protection (Code Protection off)
      CONFIG  WRT = OFF             ; FLASH Program Memory Write Enable (Unprotected program memory may not be written to by EECON control)

; ------------------- Pagina��o da mem�ria ----------------
bank0 MACRO 
    BCF STATUS, 5 ;
    BCF STATUS, 6 ;
    endm

bank1 MACRO  
    BSF STATUS, 5 ;
    BCF STATUS, 6 ;
    endm

bank2 MACRO  
    BCF STATUS, 5 ;
    BSF STATUS, 6 ;
    endm

; ------------------- Cria��o de vari�veis -------------------------
    PSECT udata
    TEMP1: DS 1
    TEMP2: DS 1

; ------------------- Defini��o de entradas ---------------
    #define BOTAO1 PORTD, 0
    #define BOTAO2 PORTD, 1
    #define BOTAO3 PORTD, 2
    #define BOTAO4 PORTD, 3
    #define BOTAO5 PORTD, 4

; ------------------- Defini��o de sa�das -----------------
    #define LED1   PORTD, 5
    #define LED2   PORTD, 6
    #define LED3   PORTD, 7
    #define LED4   PORTC, 0
    #define LED5   PORTC, 1

    #define LED1_ON  bsf PORTD, 5
    #define LED1_OFF bcf PORTD, 5
    #define LED2_ON  bsf PORTD, 6
    #define LED2_OFF bcf PORTD, 6
    #define LED3_ON  bsf PORTD, 7
    #define LED3_OFF bcf PORTD, 7
    #define LED4_ON  bsf PORTC, 0
    #define LED4_OFF bcf PORTC, 0
    #define LED5_ON  bsf PORTC, 1
    #define LED5_OFF bcf PORTC, 1

; ------------------- Vetor de reset -----------------
    PSECT code, delta=2, abs
    ORG 0x00      ; Define o endere�o inicial de processamento
    goto INICIO

; ------------------- Vetor de interrup��es -----------------
    PSECT intvector, global, class=CODE, delta=2
    ORG 0x04      ; Define o endere�o inicial do tratamento de interrup��es
    retfie ; Retorno da interrup��o

; ------------------- Subrotina de delay -----------------
DELAY:
    movlw   0xFF        ; Inicia a contagem
    movwf   TEMP2
DelayLoop:
    movlw   0xFF        ; Inicia a contagem em TEMP1
    movwf   TEMP1
InnerLoop:
    decfsz  TEMP1, f    ; Decrementa TEMP1
    goto    InnerLoop
    decfsz  TEMP2, f
    goto    DelayLoop
    return

; ------------------- In�cio do programa -------------
INICIO:
    clrf    PORTA   ; Limpa os registradores de portas
    clrf    PORTB
    clrf    PORTC
    clrf    PORTD
    clrf    PORTE

; ------------------- Configura��es do microcontrolador -------------
    bank1       ; ALTERA PARA O BANCO 1.
    movlw   0xFF    ; Coloca todos os pinos de PORTA como entrada
    movwf   TRISA   
    movlw   0x1F    ; PORTD com os pinos 0-4 como entrada (bot�es) e 5-7 como sa�da (LEDs)
    movwf   TRISD
    movlw   0xFC    ; PORTC com pinos 0 e 1 como sa�da (LEDs)
    movwf   TRISC
    movlw   0xFF    ; PORTB como entrada
    movwf   TRISB
    movlw   0xFF    ; PORTE como entrada
    movwf   TRISE

    bank0   ; RETORNA PARA O BANCO 0.

; ------------------- Programa principal -------------
Loop:
    ; Verifica o estado de cada bot�o e faz o LED piscar quando o bot�o for pressionado

    ; Bot�o 1
    BTFSS   BOTAO1      ; Se BOTAO1 est� em n�vel alto (n�o pressionado), pula
    goto    Blink_LED1  ; Se BOTAO1 est� em n�vel baixo (pressionado), vai para Blink_LED1
    LED1_OFF            ; Desliga o LED1

    ; Bot�o 2
    BTFSS   BOTAO2
    goto    Blink_LED2
    LED2_OFF

    ; Bot�o 3
    BTFSS   BOTAO3
    goto    Blink_LED3
    LED3_OFF

    ; Bot�o 4
    BTFSS   BOTAO4
    goto    Blink_LED4
    LED4_OFF

    ; Bot�o 5
    BTFSS   BOTAO5
    goto    Blink_LED5
    LED5_OFF

    goto    Loop

; ------------------- Sub-rotinas de piscar LEDs -------------
Blink_LED1:
    LED1_ON
    call    DELAY
    LED1_OFF
    call    DELAY
    BTFSS   BOTAO1      ; Verifica se o bot�o ainda est� pressionado
    goto    Blink_LED1  ; Se estiver pressionado, continua piscando
    goto    Loop        ; Se n�o, volta ao loop principal

Blink_LED2:
    LED2_ON
    call    DELAY
    LED2_OFF
    call    DELAY
    BTFSS   BOTAO2
    goto    Blink_LED2
    goto    Loop

Blink_LED3:
    LED3_ON
    call    DELAY
    LED3_OFF
    call    DELAY
    BTFSS   BOTAO3
    goto    Blink_LED3
    goto    Loop

Blink_LED4:
    LED4_ON
    call    DELAY
    LED4_OFF
    call    DELAY
    BTFSS   BOTAO4
    goto    Blink_LED4
    goto    Loop

Blink_LED5:
    LED5_ON
    call    DELAY
    LED5_OFF
    call    DELAY
    BTFSS   BOTAO5
    goto    Blink_LED5
    goto    Loop

    end
