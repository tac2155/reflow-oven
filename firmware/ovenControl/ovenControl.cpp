// reflow oven
// reflow oven control for new circuit
// http://github.com/tac2155/reflow-oven
//
// copyright 2013 tim cousins

// main control c++ file

// include header
#include "ovenControl.h"

int main(void) {
	float temp;

	PWM_init();
	SPI_init();

	bool desiredTemp = false;

	while (!desiredTemp) {
		temp = SPI_readTemp();
		if (temp == 0) {
			// error to catch if thermocouple plugged in
		}
		else if (temp > MAX_TEMP) {
			desiredTemp = true;
			PORT_PWM &= (~PWM_HEAT);
		}
		else {
			PORT_PWM |= PWM_HEAT;
		}
	}

	return 0;
}

void SPI_init(void) {
	// Set MOSI, SCK and SS as output
	DDR_SPI |= (SPI_SCK | SPI_MOSI | SPI_SS);
	// Set MISO as input
	DDR_SPI &= ~SPI_MISO;

	// Set up SPI
	// Enable SPI, set as Master, sample trailing edge, set clock as fosc/8
	SPCR |= ((1 << SPE) | (1 << MSTR) | (1 << CPHA) | (1 << SPR0));
	// Disable SPI interupt, MSB first, leading edge rising, clock as fosc/8
	SPCR &= ~((1 << SPIE) | (1 << DORD) | (1 << CPOL) | (1 << SPR1));
	SPSR |= (1 << SPI2X);

	// Short delay to let SPI get ready
	_delay_ms(500);
}

void PWM_init(void) {
	// sets pin 6 for heat as output
	DDR_PWM |= PWM_HEAT;
	PORT_PWM &= ~PWM_HEAT;
}

void UART_init(void) {
	//set Baud Rate
	UBRR0H = (uint8_t) (MYUBRR >> 8);
	UBRR0L = (uint8_t) MYUBRR;


	// enable transmission and receiving
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	// Set frame format: 8 data, 2 stop bit
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

float SPI_readTemp(void) {
	uint16_t value;
	float currentTemp;

	uint8_t hi = 0;
	uint8_t lo = 0;

	// set SS to low to beging transfer
	PORT_SPI &= ~SPI_SS;
	// slight delay
	_delay_us(1);

	// read two bytes from thermocouple
	// write to SPDR to initiate transmission
	SPDR = 0;
	// wait for transmission to complete
	while(!(SPSR & (1<<SPIF)));
	hi = SPDR;

	// write to SPDR to initiate transfer of secnd byte
	SPDR = lo;
	// wait for transmission to complete
	while(!(SPSR & (1<<SPIF)));
	lo = SPDR;

	// set SS high to end transfer
	PORT_SPI |= SPI_SS;

	// if bit 2 of lo is hi, thermocouple is unplugged
	if (lo & (1 << 2)) {
		// return error value temp
		return 0;
	}

	else {
		value = ((hi << 5) | (lo >> 3));
		//bit 15 unnecessary
		value &= ~(1<<15);
		// temp is 1/4 of outputed value
		currentTemp = 0.25*value;
		return currentTemp;
	}
}

/*void USART_Transmit(float temp) {
	//break 32 bit float into 4 bytes
	uint8_t data4 = (uint8_t) (temp >> 24);
	uint8_t data3 = (uint8_t) (temp >> 16);
	uint8_t data2 = (uint8_t) (temp >> 8);
	uint8_t data1 = (uint8_t) temp;

	// Wait for empty transmit buffer 
	while ( !( UCSR0A & (1<<UDRE0)) );
	// Put data into buffer, sends the data 
	UDR0 = data4;

	// Wait for empty transmit buffer 
	while ( !( UCSR0A & (1<<UDRE0)) );
	// Put data into buffer, sends the data 
	UDR0 = data3;

	// Wait for empty transmit buffer 
	while ( !( UCSR0A & (1<<UDRE0)) );
	// Put data into buffer, sends the data 
	UDR0 = data2;

	// Wait for empty transmit buffer 
	while ( !( UCSR0A & (1<<UDRE0)) );
	// Put data into buffer, sends the data 
	UDR0 = data1;	
}*/

