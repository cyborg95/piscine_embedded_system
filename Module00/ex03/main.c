/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otidahoh <otidahoh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 15:39:35 by otidahoh          #+#    #+#             */
/*   Updated: 2026/10/06 11:15:48 by otidahoh         ###   ########.fr       */
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

    // With the pull-up:
    // 1 = button released
    // 0 = button pressed
    uint8_t previous_state = 1;

    while (1)
    {
        // Read the current state of PD2.
        uint8_t current_state = PIND & (1 << PD2);

        // Detect a transition from released (1) to pressed (0).
        if (previous_state && !current_state)
        {
            // Toggle PB0: ON becomes OFF, OFF becomes ON.
            PORTB ^= (1 << PB0);
            _delay_ms(20); // Debounce delay to avoid multiple toggles.
        }
        _delay_ms(10); // Short delay to reduce CPU usage.

        // Save the current state for the next iteration.
        previous_state = current_state;
    }
}