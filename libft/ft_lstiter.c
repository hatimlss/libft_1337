/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/01 13:00:21 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/01 13:07:31 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

// void print_content(void *content)
// {
//     printf("%d\n", *(int *)content);
// }

// int main(void)
// {
//     t_list *node1;
//     t_list *node2;
//     t_list *node3;

//     node1 = malloc(sizeof(t_list));
//     node2 = malloc(sizeof(t_list));
//     node3 = malloc(sizeof(t_list));

//     if (!node1 || !node2 || !node3)
//         return (1);

//     node1->content = malloc(sizeof(int));
//     node2->content = malloc(sizeof(int));
//     node3->content = malloc(sizeof(int));

//     if (!node1->content || !node2->content || !node3->content)
//         return (1);

//     *(int *)node1->content = 10;
//     *(int *)node2->content = 20;
//     *(int *)node3->content = 30;

//     node1->next = node2;
//     node2->next = node3;
//     node3->next = NULL;

//     ft_lstiter(node1, print_content);

//     return (0);
// }