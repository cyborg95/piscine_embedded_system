/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyborg95 <cyborg95@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 11:46:06 by cyborg95          #+#    #+#             */
/*   Updated: 2026/10/16 12:20:42 by cyborg95         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * This flag is written by the INT0 interrupt handler and read by main().
 * volatile ensures the compiler reloads it instead of assuming it never
 * changes asynchronously.
 */
static volatile uint8_t button_pressed;

/*
 * Define the INT0 vector directly instead of using the AVR ISR() macro.
 * The attributes make this function use the interrupt calling convention
 * and keep the vector symbol available to the linker.
 */
void INT0_vect(void) __attribute__((signal, used, externally_visible));

void INT0_vect(void)
{
    /* Mask INT0 so switch bounce cannot trigger more toggles immediately. */
    EIMSK &= ~(1 << INT0);

    /* Toggle PB0, which drives LED D1: on becomes off, and off becomes on. */
    PORTB ^= (1 << PB0);

    /* Tell the main loop to wait out the switch bounce before re-arming INT0. */
    button_pressed = 1;
}

int main(void)
{
    /* Configure PB0 as an output and start with D1 turned off. */
    DDRB |= (1 << PB0);
    PORTB &= ~(1 << PB0);

    /* Configure PD2 (the INT0 input connected to SW1) as an input. */
    DDRD &= ~(1 << PD2);

    /* Enable PD2's internal pull-up: released is HIGH, pressed is LOW. */
    PORTD |= (1 << PD2);

    /*
     * Select a falling-edge interrupt for INT0. With the pull-up enabled,
     * pressing SW1 pulls PD2 from HIGH to LOW.
     */
    EICRA = (EICRA & ~((1 << ISC01) | (1 << ISC00))) | (1 << ISC01);

    /* Clear a stale INT0 request, then enable INT0 and global interrupts. */
    EIFR = (1 << INTF0);
    EIMSK |= (1 << INT0);
    sei();

    while (1)
    {
        if (button_pressed)
        {
            /*
             * Keep INT0 masked during this delay so mechanical contact
             * bounce does not register as additional button presses.
             */
            _delay_ms(200);

            /* Discard any pending bounce edge, reset the flag, and re-arm. */
            EIFR = (1 << INTF0);
            button_pressed = 0;
            EIMSK |= (1 << INT0);
        }
    }
}
