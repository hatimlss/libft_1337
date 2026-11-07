/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 17:50:04 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/03 19:32:35 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	copy_str(char *dest, char const *src, size_t *i)
{
	size_t	j;

	j = 0;
	while (src[j])
	{
		dest[*i] = src[j];
		(*i)++;
		j++;
	}
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	len;
	char	*result;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	copy_str(result, s1, &i);
	copy_str(result, s2, &i);
	result[i] = '\0';
	return (result);
}
