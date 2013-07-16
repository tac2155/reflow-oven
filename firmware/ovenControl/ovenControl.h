// reflow oven
// reflow oven control for new circuit
// http://github.com/tac2155/reflow-oven
//
// copyright 2013 tim cousins

// main control header file

// includes
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// define registers
#define DDR_SPI  DDRB
#define PORT_SPI PORTB
#define DDR_PWM DDRD
#define PORT_PWM PORTD

//define pin bits
//SPI
#define SPI_SCK  (1<<5)
#define SPI_MISO (1<<4)
#define SPI_MOSI (1<<3)
#define SPI_SS   (1<<2)
 

#define PWM_HEAT (1<<6)

// max temp for reflow oven to reach
#define MAX_TEMP 150
//#define CYCLE 255

// function prototypes
int main(void);

// init functions
void SPI_init(void);
void PWM_init(void);
void UART_init(void);


float SPI_readTemp(void);

