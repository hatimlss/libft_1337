/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/16 19:27:03 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/06 21:23:31 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy_forward(unsigned char *dest,
		const unsigned char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
}

static void	copy_backward(unsigned char *dest,
		const unsigned char *src, size_t n)
{
	while (n > 0)
	{
		dest[n - 1] = src[n - 1];
		n--;
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*p;
	const unsigned char	*p1;

	if (dest == src || n == 0)
		return (dest);
	if (!dest || !src)
		return (dest);
	p = (unsigned char *)dest;
	p1 = (const unsigned char *)src;
	if (p > p1)
		copy_backward(p, p1, n);
	else
		copy_forward(p, p1, n);
	return (dest);
}
