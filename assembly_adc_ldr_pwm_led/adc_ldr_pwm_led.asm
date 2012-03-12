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

 /*
	- Read from a sensor (LDR)

	- Control a LED with a PWM
  */



.include <m328pdef.inc>
.CSEG
.ORG 0x0000

	JMP Main


.ORG 0x002A      // called when a conversion ends (ADC)
	
	JMP Conversion


.ORG 0x0034

Main:

	CALL InitAD
	CALL InitPWM

		// activate the interruptions
	LDI R20, 0x80
	OUT SREG, R20
	
	Main_loop:
			
			// doesn't need anything here
	
	JMP Main_loop




/*
	ADC (Analog to Digital Converter)

		ADMUX

			Internal 1.1 V

			ADC1 -- PC1 -- analog input 1
			
			Left adjust the result (ADLAR)


		ADCSRA / ADCSRB

			free running mode

			prescaler: 128

 */

InitAD:

	LDI R17, 0b11100001
	STS ADMUX, R17		// STS due to this register being out of range of OUT

		
		// start right from the start converting
	LDI R17, 0b11101111
	STS ADCSRA, R17


		// free-running mode
	LDI R17, 0
	STS ADCSRB, R17

RET



/*
	Fast PWM -- timer0 8 bits

	PWM output in OC0A (PD6 or digital pin 6)


	f_PWM = f_clock / (prescaler * 256)

	prescaler: 1, 8, 64, 256 or 1024

	OCR0A to define the duty cycle
	
	
 */

InitPWM:

	LDI R16, 0b01000000
	OUT DDRD, R16

		// clear OC0A on comparating, set OC0A when it starts again from 0
		// mode 3 -- fast PWM
	LDI R16, 0b10000011
	OUT TCCR0A, R16

	LDI R16, 0b00000011		// prescaler: 64
	OUT TCCR0B, R16


	LDI R16, 127
	OUT OCR0A, R16

RET




/*
	Read the registers ADCL and ADCH to get the value (ADCL has to be read first, and then ADCH)

	ADC 10 bits -- 0 -> 1023
	
	For ease of use, we'll use just one register (8 bits -- since its left adjusted, we read ADCH)
		(you loose a bit of accuracy, but doesn't matter for this purpose)

	ADC = Vin * 1024 / Vref
*/

Conversion:
	
		// we're just reading the most significant 8 bits (ADCH register), the other 2 we discard (assume as 0)
	LDS R18, ADCH		// LDS due to ADCH being out of range for IN

		// we want the LED to light when its dim (low luminosity)
		// when that happens, we have a tension near 0V, so we invert the number we get from ADC
		// so, low tension --> low luminosity --> small number --> we invert --> bigger number --> we light the LED
	LDI R19, 0xFF
	SUB R19, R18

	
		// update the duty cycle of the PWM
	OUT OCR0A, R19
	
	// RETI and not RET -- the difference is that when an interruption function is called, all other interruptions are turn off (in SREG -- global interrupt bit),
	// and by calling RETI, you turn them back on
	// RET is for normal functions
RETI
