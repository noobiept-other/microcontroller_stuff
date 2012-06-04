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

static int PIN_LED = 7;

void LED_init();
void EXTERNAL_INTERRUPT_init();
int isSensorActivated();


/*
    Using a pair of photo-transistor and photo-diode is useful as a limit sensor,
        to determine when some object reached its physical limits (and must stop, for example).
 */

int main(void)
{
EXTERNAL_INTERRUPT_init();
LED_init();

    // activate the global interruptions
SREG = 0x80;


while(1)
    {
        // nothing to do here
    }

return 0;
}



/*
    LED in PD7 (or digital pin 7)
 */

void LED_init()
{
DDRD |= (1 << PIN_LED);
PORTD |= (1 << PIN_LED);
}



/*
    We're configuring a pin with an external interrupt.

    We want to be warned when some object passes through the middle of the diode
        and transistor. When this happens, the photo-transistor will not get much
        light, and the tension at the collector will be V+ (5V).

    The interrupt will occur on the rising edge of the tension
        (basically, when the tension varies from 0V to 5V)

        Pin: INT0 -- PD2 (digital pin 2)
 */

void EXTERNAL_INTERRUPT_init()
{
    // PD2 as input
DDRD &= ~(1 << 2);

    // INT0 -- interrupt in rising edge
EICRA = (1 << ISC01) | (1 << ISC00);

    // enable interruption
EIMSK = 1 << INT0;

EIFR = 0;   // flags

PCICR = 0;
PCIFR = 0;

PCMSK2 = 0;
PCMSK1 = 0;
PCMSK0 = 0;
}


/*
    Returns a 'bool' value, that tells whether the sensor is activated or not.

    It is activated when the light from the diode doesn't reach the transistor,
        the transistor doesn't conduct, and thus has 5V at the collector
            (so we get '1' when we read from PIND).

    It's not activated when the transistor conducts, and we read a '0'.
 */

int isSensorActivated()
{
return PIND & (1 << 3);
}



/*
    The interrupt function.

    Toggle of the led, in this example.
 */

ISR(INT0_vect)
{
PORTD ^= (1 << PIN_LED);
}

