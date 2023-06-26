/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 21:20:12 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 17:06:12 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	show_cmd(t_token *cmd)
{
	if (!cmd)
		printf("OH Noo!! Why your list is emty!?");
	while (cmd)
	{
		show_2d(cmd->token);
		printf(" -> ");
		cmd = cmd->next;
	}
	printf("\n");
}

void	show_red(t_token *cmd)
{
	if (!cmd)
		printf("OH Noo!! Why your list is emty!?");
	while (cmd)
	{
		show_2d(cmd->red);
		printf(" -> ");
		cmd = cmd->next;
	}
	printf("\n");
}

void	show_2d(char **input)
{
	int	i;

	if (!input || !*input)
		return ;
	i = 0;
	while (input[i])
		dprintf(2, "%s,", input[i++]);
}

void	print_tkn(char *str)
{
	printf(" %s ->", str);
}
