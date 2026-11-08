/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/28 03:13:00 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/04 21:22:32 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(long nb)
{
	int	c;

	c = 1;
	if (nb < 0)
	{
		c++;
		nb = -nb;
	}
	while (nb >= 10)
	{
		c++;
		nb /= 10;
	}
	return (c);
}

char	*ft_itoa(int n)
{
	int		c;
	long	nb;
	char	*p;

	nb = n;
	c = ft_count(nb);
	p = malloc(c + 1);
	if (!p)
		return (NULL);
	p[c] = '\0';
	if (nb < 0)
	{
		p[0] = '-';
		nb = -nb;
	}
	while (c > (n < 0))
	{
		p[--c] = (nb % 10) + '0';
		nb /= 10;
	}
	return (p);
}
