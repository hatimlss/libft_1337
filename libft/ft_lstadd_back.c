/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 12:26:07 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/07 11:43:14 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = ft_lstlast(*lst);
	last->next = new;
}

// int main(void)
// {
//     t_list  *lst;
//     t_list  *node1;
//     t_list  *node2;
//     t_list  *node3;
//     int     a = 10;
//     int     b = 20;
//     int     c = 30;

//     lst = NULL;

//     node1 = malloc(sizeof(t_list));
//     node2 = malloc(sizeof(t_list));
//     node3 = malloc(sizeof(t_list));

//     if (!node1 || !node2 || !node3)
//         return (1);

//     node1->content = &a;
//     node2->content = &b;
//     node3->content = &c;

//     node1->next = NULL;
//     node2->next = NULL;
//     node3->next = NULL;

//     ft_lstadd_back(&lst, node1);
//     ft_lstadd_back(&lst, node2);
//     ft_lstadd_back(&lst, node3);

//     while (lst)
//     {
//         printf("%d -> ", *(int *)lst->content);
//         lst = lst->next;
//     }
//     printf("NULL\n");

//     free(node1);
//     free(node2);
//     free(node3);

//     return (0);
// }