/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 14:56:00 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 13:31:28 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_outfile(char *file, int i, t_token *cmd)
{
	int	fd;

	if (!access(file, F_OK) && access(file, W_OK))
	{
		error_print_format_2(file, 4);
		cmd->stat->is_error = 1;
	}
	else
	{
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		cmd->red_fd[i] = fd;
	}
}

void	do_append(char *file, int i, t_token *cmd)
{
	int	fd;

	if (!access(file, F_OK) && access(file, W_OK))
	{
		error_print_format_2(file, 4);
		cmd->stat->is_error = 1;
	}
	else
	{
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		cmd->red_fd[i] = fd;
	}
}

void	set_default_inout(t_fd *p, t_token *cmd)
{
	int	i;
	int	size;

	i = 0;
	size = lst_size(cmd) - 1;
	if (size == 0)
		return ;
	while (cmd)
	{
		if (i == 0)
			cmd->stat->outfile = p->pipe_fd[(i * 2) + 1];
		else if (cmd->next == NULL)
			cmd->stat->infile = p->pipe_fd[(i * 2) - 2];
		else
		{
			cmd->stat->infile = p->pipe_fd[(i * 2) - 2];
			cmd->stat->outfile = p->pipe_fd[(i * 2) + 1];
		}
		i++;
		cmd = cmd->next;
	}
}

void	setup_inout(t_d *d, t_token *cmd)
{
	int	i;

	set_default_inout(d->pipe, cmd);
	while (cmd)
	{
		i = 0;
		while (!cmd->stat->is_error && cmd->red[i])
		{
			if (check_type(cmd->red[i]) == REIN
				|| check_type(cmd->red[i]) == HDOC)
				cmd->stat->infile = cmd->red_fd[i / 2];
			else if (check_type(cmd->red[i]) == REOUT
				|| check_type(cmd->red[i]) == REAPP)
				cmd->stat->outfile = cmd->red_fd[i / 2];
			i += 2;
		}
		cmd = cmd->next;
	}
}
