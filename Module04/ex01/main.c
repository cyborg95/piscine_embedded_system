/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyborg95 <cyborg95@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 12:23:14 by cyborg95          #+#    #+#             */
/*   Updated: 2026/10/19 14:21:56 by cyborg95         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t ticks = 0;
volatile uint8_t duty = 0;
volatile int8_t direction = 1;

/* Interrupt without ISR() macro */
void TIMER0_COMPA_vect(void) __attribute__((signal, used, externally_visible));

void TIMER0_COMPA_vect(void)
{
    ticks++;

    if (ticks < 5)
        return;
    ticks = 0;

    duty += direction;

    if (duty == 100)
        direction = -1;
    else if (duty == 0)
        direction = 1;

    OCR1A = duty * 10;
}

int main(void)
{
    /* PB1 output */
    DDRB |= (1 << PB1);

    /* Timer1: Fast PWM, TOP = 1000 */
    ICR1 = 1000;
    OCR1A = 0;

    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    /* Timer0: CTC interrupt every 1 ms */
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = 249;

    TIMSK0 = (1 << OCIE0A);
    sei();

    while (1)
        ;
}
