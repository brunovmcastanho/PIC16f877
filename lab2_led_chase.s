; Projeto 1 - Exemplo de programa em assembly
; Descrição: Controle de 5 LEDs com 5 botões

; ------------------- Configuração dos FUSE BITS ------------------
PROCESSOR 16F877
#include <xc.inc>

    ;Configuração do 'hardware' do microcontrolador
; CONFIG
  CONFIG  FOSC = HS             ; Oscillator Selection bits (HS oscillator)
  CONFIG  WDTE = OFF            ; Watchdog Timer Enable bit (WDT disabled)
  CONFIG  PWRTE = OFF           ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  CP = OFF              ; FLASH Program Memory Code Protection bits (Code protection off)
  CONFIG  BOREN = OFF           ; Brown-out Reset Enable bit (BOR disabled)
  CONFIG  LVP = OFF             ; Low Voltage In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
  CONFIG  CPD = OFF             ; Data EE Memory Code Protection (Code Protection off)
  CONFIG  WRT = OFF             ; FLASH Program Memory Write Enable (Unprotected program memory may not be written to by EECON control)

; ------------------- Paginação da memória ----------------
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

; ------------------- Criação de variáveis -------------------------
PSECT udata
 TEMP1:DS 1
 TEMP2:DS 1

; ------------------- Definição de entradas ---------------

#define BOTAO1	PORTD, 0
#define BOTAO2	PORTD, 1
#define BOTAO3	PORTD, 2
#define BOTAO4	PORTD, 3
#define BOTAO5	PORTD, 4

; ------------------- Definição de saídas -----------------

#define LED1	PORTD, 5
#define LED2	PORTD, 6
#define LED3	PORTD, 7
#define LED4	PORTC, 0
#define LED5	PORTC, 1

#define LED1_ON	bsf PORTD, 5
#define LED1_OFF bcf PORTD, 5
#define LED2_ON	bsf PORTD, 6
#define LED2_OFF bcf PORTD, 6
#define LED3_ON	bsf PORTD, 7
#define LED3_OFF bcf PORTD, 7
#define LED4_ON	bsf PORTC, 0
#define LED4_OFF bcf PORTC, 0
#define LED5_ON	bsf PORTC, 1
#define LED5_OFF bcf PORTC, 1

; ------------------- Vetor de reset -----------------

PSECT code, delta=2, abs
ORG	0x00		; Define o endereço inicial de processamento
goto INICIO	 

; ------------------- Vetor de interrupções -----------------
PSECT intvector, global, class=CODE, delta=2
ORG	0x04		; Define o endereço inicial do tratamento de interrupções
retfie	; Retorno da interrupção

; ------------------- Subrotina de delay -----------------
DELAY:
	movlw  	11111111B		; Inicia a contagem
	movwf  	TEMP2
label:  
	movlw  	255			; Inicia a contagem em TEMP1
	movwf  	TEMP1
	decfsz	TEMP1			; Decrementa TEMP1
	goto    $-1
	decfsz	TEMP2
	goto    label
return

; ------------------- Início do programa ------------- 
INICIO:
clrf	PORTA	; Limpa os registradores de portas
clrf	PORTB
clrf	PORTC
clrf	PORTD
clrf	PORTE

; ------------------- Configurações do microcontrolador ------------- 
bank1		; ALTERA PARA O BANCO 1.
movlw	0	; Coloca todos os pinos de PORTA como entrada
movwf	TRISA	
movlw	0	
movwf	TRISD
movlw	0	; PORTC com pinos 0 e 1 como saída (LEDs)
movwf	TRISC
movlw	0	; PORTB como entrada
movwf	TRISB
movlw	0	; PORTE como entrada
movwf	TRISE


bank0	; RETORNA PARA O BANCO 0.

; ------------------- Programa principal ------------- 
    
movlw 1
movwf PORTD
    
Loop:
; Verifica o estado de cada botão e liga/desliga o LED correspondente
RRF  PORTD
call DELAY
goto Loop
end