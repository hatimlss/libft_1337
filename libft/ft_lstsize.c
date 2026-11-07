/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:11:18 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/03 17:19:13 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	unsigned int	c;

	c = 0;
	while (lst)
	{
		c++;
		lst = lst->next;
	}
	return (c);
}

// int main(void)
// {
//     t_list  *node1;
//     t_list  *node2;
//     t_list  *node3;
//     t_list  *lst;
//     int     a = 10;
//     int     b = 20;
//     int     c = 30;

//     node1 = malloc(sizeof(t_list));
//     node2 = malloc(sizeof(t_list));
//     node3 = malloc(sizeof(t_list));

//     if (!node1 || !node2 || !node3)
//         return (1);

//     node1->content = &a;
//     node2->content = &b;
//     node3->content = &c;

//     node1->next = node2;
//     node2->next = node3;
//     node3->next = NULL;

//     lst = node1;

//     printf("List size: %u\n", ft_lstsize(lst));

//     free(node1);
//     free(node2);
//     free(node3);

//     return (0);
// }