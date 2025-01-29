# Exercises Repository - Microcontrollers

This repository contains **C** and **Assembly** implementations for the **PIC16F877**, developed as part of the **Microcontrollers** course exercises. Each exercise list covers different aspects of programming and hardware control using GPIO, timers, interrupts, UART, EEPROM, and more.

## Repository Structure
The files are organized according to the exercise list number and their respective functionality:

### **Lists and Exercises**

#### **List 1 - GPIO (Assembly)**
- `lab1_5buttons_5leds.asm` - Control of 5 LEDs with 5 buttons.
- `lab1_5buttons_5leds_blink.asm` - LEDs blinking when buttons are pressed.

#### **List 2 - GPIO (Assembly)**
- `lab2_led_chase.asm` - LED sequence turning on one at a time on PORTD.
- `lab2_led_chase_direction.asm` - LED sequence with direction control via a button.

#### **List 3 - GPIO (Assembly)**
- `lab3_toggle_leds_buttons.asm` - LED toggling controlled by two buttons.

#### **List 4 - Counters (Assembly)**
- `lab4_button_counter.asm` - Binary counter triggered by a button, displayed on PORTD LEDs.
- `lab4_keypad_counter.asm` - Binary counter triggered by a 4x4 keypad, with LED indicator for value 15.

#### **List 5 - 7-Segment Display and Stepper Motor (C)**
- `lab5_7seg_hex_counter.c` - Hexadecimal counter on a 7-segment display.
- `lab5_stepper_toggle_dir.c` - Stepper motor direction control.

#### **List 6 - LCD and Matrix Keypad (C)**
- `lab6_lcd_toggle_info.c` - LCD information display toggling with a button.
- `lab6_lcd_click_counter.c` - Keypad click counter displayed on the LCD.

#### **List 7 - LCD and Matrix Keypad (C)**
- `lab7_lcd_keypad_display.c` - Displaying pressed keys on the LCD with scrolling and automatic clearing.

#### **List 8 - ADC Conversion and PWM (C)**
- `lab8_adc_lcd_display.c` - Voltage reading via ADC and display on the LCD.
- `lab8_adc_pwm_led.c` - LED brightness control using ADC and PWM.

#### **List 9 - Timers (C)**
- `lab9_timer0_led_blink.c` - LED blinking using Timer 0 with a 1s interval.
- `lab9_timer1_button_counter.c` - Button press counter using Timer 1 and display on LCD.

#### **List 10 - Interrupts (C)**
- `lab10_interrupts_led_control.c` - LED control using Timer 2 and external interrupts.
- `lab10_interrupts_motor_timer.c` - DC motor control with Timer 1 and button interrupts.

#### **List 11 - EEPROM (C)**
- `lab11_eeprom_keypad_lcd.c` - 4x4 keypad reading, display on LCD, and EEPROM storage.

#### **List 12 - UART (C)**
- `lab12_uart_keypad_lcd.c` - Communication between matrix keypad, UART, and LCD using interrupts.

---
## **Requirements**
To compile and simulate the codes, the following software can be used:
- **MPLAB X IDE** + **MPASM** (for Assembly)
- **MPLAB XC8 Compiler** (for C)
- **Proteus** or **SimulIDE** (for circuit simulation)



## **Contributing**
Feel free to open issues or submit pull requests with code improvements.

---
**Author:** Bruno Castanho  
**Contact:** brunovmcastanho@gmail.com

