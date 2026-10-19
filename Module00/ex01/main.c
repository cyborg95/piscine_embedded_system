/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otidahoh <otidahoh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/05 14:51:20 by otidahoh          #+#    #+#             */
/*   Updated: 2026/10/05 15:14:14 by otidahoh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int main(void)
{
        // DDRB: Data Direction Register for Port B.
        // Setting PB0 to 1 configures PB0 as an output.
        DDRB |= (1 << PB0);

        // PORTB: Output register for Port B.
        // Setting PB0 to 1 drives PB0 HIGH, turning on LED D1.
        PORTB |= (1 << PB0);
}