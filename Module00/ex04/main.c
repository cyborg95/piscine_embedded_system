/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyborg95 <cyborg95@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/06 10:41:10 by otidahoh          #+#    #+#             */
/*   Updated: 2026/10/10 12:53:51 by cyborg95         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    uint8_t counter = 0;

    /* LEDs D1, D2, D3 and D4
     * D1 -> PB0
     * D2 -> PB1
     * D3 -> PB2
     * D4 -> PB4
     */
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB4);

    /* SW1 -> PD2
     * SW2 -> PD4
     */
    DDRD &= ~(1 << PD2);
    DDRD &= ~(1 << PD4);

    /* Enable internal pull-ups */
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD4);

    while (1)
    {
        /* SW1 pressed -> increment */
        if (!(PIND & (1 << PD2)))
        {
            if (counter < 15)
                counter++;

            /* Wait for button release */
            while (!(PIND & (1 << PD2)))
                ;

            _delay_ms(200);
        }

        /* SW2 pressed -> decrement */
        if (!(PIND & (1 << PD4)))
        {
            if (counter > 0)
                counter--;

            /* Wait for button release */
            while (!(PIND & (1 << PD4)))
                ;

            _delay_ms(200);
        }

        /* Display counter on LEDs
         *
         * counter bit 0 -> PB0 (D1)
         * counter bit 1 -> PB1 (D2)
         * counter bit 2 -> PB2 (D3)
         * counter bit 3 -> PB4 (D4)
         */

        PORTB &= ~((1 << PB0) |
                   (1 << PB1) |
                   (1 << PB2) |
                   (1 << PB4));

        if (counter & (1 << 0))
            PORTB |= (1 << PB0);

        if (counter & (1 << 1))
            PORTB |= (1 << PB1);

        if (counter & (1 << 2))
            PORTB |= (1 << PB2);

        if (counter & (1 << 3))
            PORTB |= (1 << PB4);
    }
}