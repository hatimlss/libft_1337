/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 23:21:28 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/04 21:37:06 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;
	int	index;

	i = 0;
	index = -1;
	while (s[i])
	{
		if (s[i] == (char)c)
			index = i;
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	if (index >= 0)
		return ((char *)&s[index]);
	return (NULL);
}
