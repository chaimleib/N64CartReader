#ifndef PORTS_H
#define PORTS_H

#include <avr/io.h>

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

// 16-bit bus to N64 cart
#define AD_LOW_PORT PORTB
#define AD_LOW_DDR  DDRB
#define AD_LOW_PIN  PINB
#define AD_HI_PORT  PORTD
#define AD_HI_DDR   DDRD
#define AD_HI_PIN   PIND

// Control lines to N64 cart
#define CTL_PORT    PORTF
#define CTL_PIN     PINF
#define CTL_DDR     DDRF
#define RD          BIT5
#define ALEL        BIT6
#define ALEH        BIT7

// Status LEDs: Read from cart, 'configure'
#define ST_PORT     PORTC
#define ST_DDR      DDRC
#define ST_RD       BIT6
#define ST_CFG      BIT7

#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#endif
