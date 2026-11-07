/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 12:29:54 by hlassiou          #+#    #+#             */
/*   Updated: 2026/10/31 12:10:20 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	*lst = new;
}

// int main(void)
// {
//     t_list  *lst;
//     t_list  *node1;
//     t_list  *node2;
//     t_list  *node3;

//     int     a = 10;
//     int     b = 20;
//     int     c = 5;

//     lst = NULL;

//     node1 = malloc(sizeof(t_list));
//     node2 = malloc(sizeof(t_list));
//     node3 = malloc(sizeof(t_list));

//     if (!node1 || !node2 || !node3)
//         return (1);

//     node1->content = &a;
//     node2->content = &b;
//     node3->content = &c;

//     ft_lstadd_front(&lst, node1);
//     ft_lstadd_front(&lst, node2);
//     ft_lstadd_front(&lst, node3);

//     while (lst)
//     {
//         printf("%d -> ", *(int *)lst->content);
//         lst = lst->next;
//     }
//     printf("NULL\n");

//     return (0);
// }