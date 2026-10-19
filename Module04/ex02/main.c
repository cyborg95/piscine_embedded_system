/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyborg95 <cyborg95@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 15:19:24 by otidahoh          #+#    #+#             */
/*   Updated: 2026/10/19 14:20:14 by cyborg95         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t count = 0;

void INT0_vect(void)
    __attribute__((signal, used, externally_visible));
void INT1_vect(void)
    __attribute__((signal, used, externally_visible));

void display(void)
{
    PORTB = (PORTB & ~0x17)
          | (count & 0x07)
          | ((count & 0x08) << 1);
}

/* SW1: increment */
void INT0_vect(void)
{
    count = (count + 1) & 0x0F;
    display();
}

/* SW2: decrement */
void INT1_vect(void)
{
    count = (count - 1) & 0x0F;
    display();
}

int main(void)
{
    /* D1, D2, D3, D4 outputs */
    DDRB |= (1 << PB0) | (1 << PB1)
          | (1 << PB2) | (1 << PB4);

    /* SW1 and SW2 inputs with pull-ups */
    DDRD &= ~((1 << PD2) | (1 << PD3));
    PORTD |= (1 << PD2) | (1 << PD3);

    /* INT0 and INT1: falling edge */
    EICRA = (1 << ISC01) | (1 << ISC11);

    /* Enable both interrupts */
    EIFR = (1 << INTF0) | (1 << INTF1);
    EIMSK = (1 << INT0) | (1 << INT1);

    display();
    sei();

    while (1)
        ;
}
