/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <pic16f628a.h>

/*****************************************************************************
                            PIC16F628A CONFIGURATION
         '#pragma config' directive not supported for pic14  SDCC port
*****************************************************************************/

// Use internal oscillator, I/O functions on all the pins
// Watch dog timer disabled
// Power timer disabled
// MCLR pin disabled
// Brown out detection disabled
// Low level programming disabled
// Memory protection disabled
static __code char __at(_CONFIG) CONFIG = _INTOSC_OSC_NOCLKOUT & _WDT_OFF &
                                   _PWRTE_OFF & _MCLRE_ON & _BOREN_OFF &
                                   _LVP_OFF & DATA_CP_OFF & _CP_OFF;

/*****************************************************************************
*****************************************************************************/



/*
 * HT1621 controll defines
 */

// HT1621 pins
#define CS RB3
#define DATA RB4
#define DATA_CFG TRISB4
#define RD_ RB5
#define WR_ RB6

// Commands
#define READ_CMD_FLAG 0xC0 // Read command (use 3 bits only)

// Misc
#define ADDR_SIZE 6 // Address is 6 bits
#define DATA_SIZE 4 // Data is 4 bits




/*
 * Init and configure uart hardware
 */
void uart_init(void)
{
	// TRISB 1,2 set as inputs for USART USE
	TRISB1 = 1;
	TRISB2 = 1;

	// BRGH - High Baud Rate Select Bit
    // TXSTAbits.BRGH
	BRGH = 1; // high speed
	// SPBRG - Baud Rate Generator Register
	SPBRG = 25; // 4MHz => 9600 baud (BRGH = 1)

	// SPEN - Serial Port Enable Bit
    // RCSTAbits.SPEN
	SPEN = 1; // Enable USART

	// TXIE - USART Transmit Interupt Enable Bit
    // PIE1bits.TXIE
	TXIE = 0; // Transmission interrupt disabled

	// TX9 - 9-bit Transmit Enable Bit
    // TXSTAbits.TX9
	TX9 = 0; // 8-bit transmission

	// TXEN - Transmit Enable Bit
    // TXSTAbists.TXEN
	TXEN = 1; // Transmission enabled

	// SYNC - USART Mode select Bit
    // TXSTAbits.SYNC
	SYNC = 0; // asynchrone

	// RCIE - USART Receive Interupt Enable Bit
    // PIE1bits.RCIE
	RCIE = 0; // No receive interrupt

	// RX9 - 9-bit Receive Enable Bit
    // RCSTAbits.RX9
	RX9 = 0; // 8-bit reception

	// CREN - Continuous Receive Enable Bit
    // RCSTAbits.CREN
	CREN = 1; // Continuos receive
}



// Blocking function
void putchar(char c)
{
    // Wait until transmission buffer is clear
    // PIR1bits.TXIF
    while( ! TXIF );

    // Write transmission buffer
    TXREG = c;
}


// Blocking function
int getchar(char* c)
{
    // RCSTAbits.OEERR
	if (OERR)
	{
		//IN CASE OF ERROR RESET CREN
        // RCSTAbits.CREN
        CREN=0;
		CREN=1;
	}

    // Wait reception interrupt
    // PIR1bits.RCIF
	while( ! RCIF );

	*c = RCREG;
	return 1;
}



/*
 * uC working at 4 Mhz
 * HT1621 maximum reading freq. = 150Mhz
 *
 * A wait function is needed between bits
 */
void wait()
{
    // Variable parameters
    unsigned int delay = 90000;
	unsigned int a = 0;
	unsigned int i=0;
	for(i=0;i<delay;i++)
	{
		while(a < 10)
		{
			a = a+1;
		}
		a=0;
	}
}



/*
 * Send up to CNT bits of DATA to the HT1621
 */
void send_bits(unsigned char data, unsigned char cnt)
{
    unsigned char i;
    DATA_CFG = 0; // DATA is output
    for(i=0; i<cnt; i++)
    {
        if((data&0x80)==0)
        {
            DATA=0;
        }
        else
        {
            DATA=1;
        }

        WR_=0;
        wait();
        WR_=1;
        wait();
        data<<=1;
    }
}



/*
 * Send LCD data to UART port
 */
void read_display()
{
    unsigned char i;
    unsigned char j;
    unsigned char c = 0x00;
    CS=0; // Start HT1621 selection

    // Send READ command
    send_bits(READ_CMD_FLAG, 3);
    // Read from memory start
    send_bits(0x00, ADDR_SIZE);


    // Read 6 segments (4 bits each one)
    DATA_CFG = 1; // DATA is input
    for(i=0; i<3; i++)
    {
        // Read 8 bits (2 segments per char)
        for(j=0; j<8; j++)
        {
            RD_=0;
            wait();
            RD_=1;
            // According to the data sheet it is recommended to read the bit
            // between the raising edge and the next falling edge
            c |= DATA << (7-j);
        }

        putchar(c);
    }

    CS=1; // Stop HT1621 selection
}



void main(void)
{
	//UART INIT
	uart_init();



    // Init B port
	TRISB = 0; // B port is output
	PORTB = 0; // Init B port
    CS = 1; // HT1621 disabled
    RD_ = 1;
    WR_ = 1;
    DATA = 0;

    putchar('a');
    putchar(' ');
    read_display();
    putchar(' ');
    while(1){}
}
