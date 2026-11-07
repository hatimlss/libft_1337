/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hlassiou <hlassiou@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/31 19:51:38 by hlassiou          #+#    #+#             */
/*   Updated: 2026/11/01 12:23:27 by hlassiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}

// void del_content(void *content)
// {
//     free(content);
// }

// int main(void)
// {
//     t_list *node;

//     node = malloc(sizeof(t_list));
//     if (!node)
//         return (1);

//     node->content = malloc(sizeof(int));
//     if (!node->content)
//     {
//         free(node);
//         return (1);
//     }

//     *(int *)node->content = 42;
//     node->next = NULL;

//     printf("Before deletion: %d\n", *(int *)node->content);

//     ft_lstdelone(node, del_content);

//     printf("Node deleted successfully!\n");

//     return (0);
// }