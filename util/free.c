/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:23:11 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/13 23:25:04 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_2d(char **input)
{
	int	i;

	i = 0;
	if (!input)
		return ;
	while (input[i])
		free(input[i++]);
	free(input);
}

void	free_env(void *content)
{
	t_env	*env;

	env = (t_env *) content;
	free(env->key);
	free(env->value);
	free(env);
}

void	free_for_all(t_d *d)
{
	lst_clear(&d->tkn);
	ft_lstclear(&d->env, free_env);
	rl_clear_history();
	free(d->buf);
	if (d->pipe != NULL)
	{
		free(d->pipe->pipe_fd);
		free(d->pipe);
	}
}

void	close_fd(t_token *tkn)
{
	t_token	*tmp;
	int		i;
	int		size;

	tmp = tkn;
	i = 0;
	size = args_count(tmp->red) / 2;
	while (tmp->red && i < size)
	{
		if (tmp->red_fd[i] != 0)
			close (tmp->red_fd[i]);
		i++;
	}
	delete_file(tkn);
}

void	delete_file(t_token *cmd)
{
	int		i;
	int		id;
	char	*name;

	id = 0;
	while (cmd)
	{
		i = 0;
		while (cmd->red[i])
		{
			if (check_type(cmd->red[i]) == HDOC)
			{
				name = create_name(cmd->red[i + 1], (i / 2), id);
				unlink(name);
				free(name);
			}
			i += 2;
		}
		id++;
		cmd = cmd->next;
	}
}
