/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otidahoh <otidahoh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 15:19:24 by otidahoh          #+#    #+#             */
/*   Updated: 2026/10/06 15:46:18 by otidahoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    // DDRB: Data Direction Register for Port B.
    // Setting PB0 to 1 configures PB0 as an output for LED D1.
    DDRB |= (1 << PB0);

    // DDRD: Data Direction Register for Port D.
    // Setting PD2 to 0 configures PD2 as an input for SW1.
    DDRD &= ~(1 << PD2);

    // PORTD: Output register for Port D.
    // Because PD2 is an input, setting its bit to 1 enables
    // the internal pull-up resistor.
    PORTD |= (1 << PD2);

    while (1)
    {
        // PIND: Input register for Port D.
        // When SW1 is pressed, PD2 is connected to GND and reads LOW.
        if (!(PIND & (1 << PD2)))
        {
            // PORTB: Output register for Port B.
            // Set PB0 HIGH to turn LED D1 on.
            PORTB |= (1 << PB0);
        }
        else
        {
            // Set PB0 LOW to turn LED D1 off.
            PORTB &= ~(1 << PB0);
        }
        _delay_ms(100); // Debounce delay to avoid rapid toggling
    }
}