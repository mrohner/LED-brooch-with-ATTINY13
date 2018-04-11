/*
  [ 0,1,2 ] -> [Resistor 47R] -> [LED] -> [Ground]
  [4 ]      -> Interrupt to 5V
  For ATTiny13 Arduino Pinout: https://goo.gl/ijL0of
*/
#include <avr/sleep.h>

const byte Interrupt_Pin = 4;
EMPTY_INTERRUPT(PCINT0_vect);

void setup() {
  pinMode(Interrupt_Pin, INPUT);    // interrupt pin
  PORTB &= ~0b00000111; // Set 0,1,2 LOW
  DDRB  |= 0b00000111;  // Enable output for pins 0,1,2
  MCUCR = (1 << SE)     // To enter any sleep modes, the SE bit in MCUCR must be written to logic one
          | (1 << SM1);   // power down sleep mode
  PCMSK = 1 << PCINT4;
  ADCSRA = 0 << ADEN;   // Power reduction ADC 
}


void loop() {
  uint8_t i;
  for (i = 0; i < 24; i++) {
    pattern1a(11);
  }
  for (i = 0; i < 38; i++) {
    pattern2(10);
  }
  for (i = 0; i < 22; i++) {
    pattern1(8);
    pattern1a(8);
  }
  for (i = 0; i < 150; i++) {
    TwinkleRandom(8);
  }
  delay(120);
}


void pattern1(byte wait_time) { // unäufä
  Yellow_high();
  delay(wait_time);
  Yellow_low();
  Orange_high();
  delay(wait_time);
  Orange_low();
  Red_high();
  delay(wait_time);
  Red_low();
}


void pattern1a(byte wait_time) { //obenabä
  Red_high();
  delay(wait_time);
  Red_low();
  Orange_high();
  delay(wait_time);
  Orange_low();
  Yellow_high();
  delay(wait_time);
  Yellow_low();
}


void pattern2(byte wait_time) { //blink
  PORTB = (1 << PB2) | (1 << PB1) | (1 << PB0);
  delay(map(tiny_random(), 0, 65534, wait_time, 2 * wait_time));
  PORTB &= ~0b00000111; // Set 0,1,2 LOW
  delay(wait_time);
}


void TwinkleRandom(byte wait_time) {//random
  byte any = map(tiny_random(), 0, 65534, 0, 3);
  PORTB &= ~0b00000111; // Set 0,1,2 LOW
  switch (any) {
    case 0:
      Red_high();
      break;
    case 1:
      Orange_high();
      break;
    case 2:
      Yellow_high();
      break;
  }
  delay(wait_time);//stay on
}


void Yellow_high() {
  PORTB = (1 << PB2);
}

void Yellow_low() {
  PORTB = (0 << PB2);
}

void Orange_high() {
  PORTB = (1 << PB1);
}

void Orange_low() {
  PORTB = (0 << PB1);
}

void Red_high() {
  PORTB = (1 << PB0);
  if (digitalRead(Interrupt_Pin)) {
    PORTB &= ~0b00000111; // Set 0,1,2 LOW
    delay(50);
    GIMSK = 1 << PCIE;    // enable Pin change interrupt in global interrupt mask
    sei(); //enable interrupt
    sleep_cpu();
    GIMSK = 0 << PCIE;    // disable INT0 in global interrupt mask
  }
}

void Red_low() {
  PORTB = (0 << PB0);
}
