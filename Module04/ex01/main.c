/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cyborg95 <cyborg95@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 12:23:14 by cyborg95          #+#    #+#             */
/*   Updated: 2026/10/16 12:23:22 by cyborg95         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define PWM_TOP             1000
#define DUTY_STEP_COUNT     100
#define TIMER0_TICK_DIVIDER 5

static volatile uint8_t duty_percent;
static volatile int8_t duty_direction = 1;
static volatile uint8_t timer0_ticks;

/*
 * Timer0 runs in CTC mode and calls this handler every millisecond.
 * Define the vector directly because the exercise forbids the ISR macro.
 */
void TIMER0_COMPA_vect(void) __attribute__((signal, used, externally_visible));

static void set_duty_cycle(void)
{
    uint16_t compare_value;

    compare_value = ((uint32_t)duty_percent * PWM_TOP) / DUTY_STEP_COUNT;
    if (duty_percent == 0)
    {
        /* Disconnect PWM so the pin is held low for a true 0% duty cycle. */
        TCCR1A &= ~(1 << COM1A1);
        PORTB &= ~(1 << PB1);
    }
    else if (duty_percent == DUTY_STEP_COUNT)
    {
        /* Disconnect PWM so the pin is held high for a true 100% duty cycle. */
        TCCR1A &= ~(1 << COM1A1);
        PORTB |= (1 << PB1);
    }
    else
    {
        OCR1A = compare_value;
        /* OC1A uses non-inverting PWM for duty cycles between the endpoints. */
        TCCR1A |= (1 << COM1A1);
    }
}

void TIMER0_COMPA_vect(void)
{
    timer0_ticks++;
    if (timer0_ticks < TIMER0_TICK_DIVIDER)
        return;
    timer0_ticks = 0;

    if (duty_direction > 0)
    {
        if (duty_percent < DUTY_STEP_COUNT)
            duty_percent++;
        if (duty_percent == DUTY_STEP_COUNT)
            duty_direction = -1;
    }
    else
    {
        if (duty_percent > 0)
            duty_percent--;
        if (duty_percent == 0)
            duty_direction = 1;
    }
    set_duty_cycle();
}

int main(void)
{
    /* PB1 is OC1A on the ATmega328P; it drives the LED using Timer1 PWM. */
    DDRB |= (1 << PB1);
    PORTB &= ~(1 << PB1);

    /*
     * Timer1 Fast PWM mode 14 uses ICR1 as TOP. With a prescaler of 8,
     * 16 MHz / (8 * (1000 + 1)) gives a PWM frequency of about 2 kHz,
     * high enough that the LED does not appear to flicker.
     */
    ICR1 = PWM_TOP;
    OCR1A = 0;
    TCCR1A = (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    /*
     * Timer0 CTC: 16 MHz / 64 / (249 + 1) = 1 kHz, so its interrupt
     * runs every millisecond. Updating every five ticks gives 200 duty
     * updates per second: 100 up and 100 down in one second.
     */
    TCCR0A = (1 << WGM01);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TCNT0 = 0;
    TIFR0 = (1 << OCF0A);
    TIMSK0 |= (1 << OCIE0A);

    set_duty_cycle();
    sei();

    while (1)
        ;
}