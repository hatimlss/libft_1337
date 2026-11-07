/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:19:55 by hlassiou          #+#    #+#             */
/*   Updated: 2026/10/31 12:25:11 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

// int main(void)
// {
//     t_list  *node1;
//     t_list  *node2;
//     t_list  *node3;
//     t_list  *last;
//     int     a = 10;
//     int     b = 20;
//     int     c = 30;

//     node1 = malloc(sizeof(t_list));
//     node2 = malloc(sizeof(t_list));
//     node3 = malloc(sizeof(t_list));

//     if (!node1 || !node2 || !node3)
//         return (1);

//     node1->content = &a;
//     node1->next = node2;

//     node2->content = &b;
//     node2->next = node3;

//     node3->content = &c;
//     node3->next = NULL;

//     last = ft_lstlast(node1);

//     if (last)
//         printf("Last node content: %d\n", *(int *)last->content);

//     free(node1);
//     free(node2);
//     free(node3);

//     return (0);
// }