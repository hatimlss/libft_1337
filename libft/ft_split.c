/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 01:27:57 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/03 19:33:30 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	wordcount(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i] && s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static void	all_free(char **p, size_t i)
{
	while (i > 0)
		free(p[--i]);
	free(p);
}

static char	**fill_words(char **p, char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	end;

	i = 0;
	start = 0;
	while (s[start])
	{
		while (s[start] == c && s[start])
			start++;
		if (!s[start])
			break ;
		end = start;
		while (s[end] && s[end] != c)
			end++;
		p[i] = ft_substr(s, start, end - start);
		if (!p[i])
			return (all_free(p, i), NULL);
		i++;
		start = end;
	}
	p[i] = NULL;
	return (p);
}

char	**ft_split(char const *s, char c)
{
	char	**p;
	size_t	count;

	if (!s)
		return (NULL);
	count = wordcount(s, c);
	p = malloc((count + 1) * sizeof(char *));
	if (!p)
		return (NULL);
	return (fill_words(p, s, c));
}

// int main()
// {
//     char **p = ft_split("hello world", ' ');
//     int i = 0;

//     while (i < 2)
//     {
//         printf("%s\n", p[i++]);
//     }
//     free(p);
// }