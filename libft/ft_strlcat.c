/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 13:05:45 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/08 14:50:56 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	srclen;
	size_t	destlen;

	srclen = ft_strlen(src);
	i = 0;
	j = 0;
	while (i < dstsize && dst[i] != '\0')
		i++;
	destlen = i;
	if (destlen == dstsize)
		return (dstsize + srclen);
	while (src[j] && i < dstsize - 1)
		dst[i++] = src[j++];
	dst[i] = '\0';
	return (destlen + srclen);
}
