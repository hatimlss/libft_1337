/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/21 18:12:34 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/03 17:07:13 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in(char s, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (s == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	start;
	size_t	end;
	char	*p;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && is_in(s1[start], set) == 1)
		start++;
	end = ft_strlen(s1);
	while (end > start && is_in(s1[end - 1], set) == 1)
		end--;
	p = malloc(end - start + 1);
	if (!p)
		return (NULL);
	i = 0;
	while (i < end - start)
	{
		p[i] = s1[start + i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

// int main()
// {
//     char const *p = "yxhixy";
//     char const *set = "xy";
//     char *res = ft_strtrim(p, set);
//     printf("%s\n", res);
//     free(res);
//     return (0);
// }