/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 19:06:55 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/06 21:21:33 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*p;
	const unsigned char	*p2;

	p = (unsigned char *)dest;
	p2 = (const unsigned char *)src;
	while (n > 0)
	{
		*p = *p2;
		p++;
		p2++;
		n--;
	}
	return (dest);
}
