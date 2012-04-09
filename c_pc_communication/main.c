/*
    Copyright - 2012 - Pedro Ferreira

    This file is part of microcontroller_stuff.

    microcontroller_stuff is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    microcontroller_stuff is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with microcontroller_stuff.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include <avr/interrupt.h>

/*
    f_osc = 16 MHz

    baud rate: 9600
 */

void initPcCommunication()
{
    // 9600 baud rate
UBRR0H = 0;
UBRR0L = 103;

    // clear flags
UCSR0A = 0;

    // activate receiver and transmitter
UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // USART asynchronous
    // 1 stop-bit
    // 8 bits of data
UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


/*
    Sends one byte
    UDREn -- tells if transmitter buffer is empty (and if ready for the next transmission)
 */

void transmit( unsigned char data )
{
    // wait for the transmission buffer to be empty
while ( !( UCSR0A & (1 << UDRE0) ) )
    ;

    // write to the buffer --> it then transmits the data
UDR0 = data;
}


/*
    The RXCn flag tells if there's unread data in the receive buffer
 */

int receivedData()
{
if ( UCSR0A & (1 << RXC0) )
    {
    return 1;
    }

return 0;
}

/*
    Get the received data
 */

unsigned char getData()
{
return UDR0;
}



int main(void)
{
initPcCommunication();


int i = 0;
char a[7] = " 1234 ";   // some dummy text

while(1)
    {
    if ( receivedData() )
        {
        transmit( getData() );


        for (i = 0 ; i < 6 ; i++)
            {
            transmit( a[i] );
            }
        }
    }

return 0;
}
