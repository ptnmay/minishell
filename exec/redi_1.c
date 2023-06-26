/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redi_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/11 01:48:45 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 13:30:54 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_name(char *file, int i, int id)
{
	char	*name;
	char	*new_id;

	new_id = ft_strjoin_premium(ft_itoa(id), ft_itoa(i), 3);
	name = ft_strjoin_premium(file, new_id, 2);
	return (name);
}

void	create_hdoc(char *file, int i, int id, t_token *cmd)
{
	int		fd;
	char	*name;
	char	*buff;

	name = create_name(file, i, id);
	fd = open(name, O_WRONLY | O_APPEND | O_CREAT, 0644);
	buff = NULL;
	buff = readline("> ");
	while (ft_strcmp(file, buff)
		|| (ft_strlen(file) != ft_strlen(buff)))
	{
		buff = ft_strjoin_premium(buff, "\n", 1);
		write (fd, buff, ft_strlen(buff));
		free(buff);
		buff = readline("> ");
	}
	free(buff);
	close(fd);
	fd = open(name, O_RDONLY);
	free(name);
	cmd->red_fd[i] = fd;
}

void	do_here(t_token *cmd)
{
	int	i;
	int	id;

	id = 0;
	while (cmd)
	{
		i = 0;
		while (cmd->red[i])
		{
			if (check_type(cmd->red[i]) == HDOC)
				create_hdoc(cmd->red[i + 1], (i / 2), id, cmd);
			i += 2;
		}
		id++;
		cmd = cmd->next;
	}
}

void	do_infile(char *file, int i, t_token *cmd)
{
	int	fd;

	if (access(file, F_OK | R_OK))
	{
		if (access(file, F_OK))
			error_print_format_2(file, 3);
		else if (access(file, R_OK))
			error_print_format_2(file, 4);
		cmd->stat->is_error = 1;
	}
	else
	{
		fd = open(file, O_RDONLY);
		cmd->red_fd[i] = fd;
	}
}

void	do_redirect(t_d *d, t_token *cmd)
{
	t_token	*tmp;
	int		i;

	tmp = cmd;
	while (cmd)
	{
		i = 0;
		while (!cmd->stat->is_error && cmd->red[i])
		{
			if (check_type(cmd->red[i]) == REIN)
				do_infile(cmd->red[i + 1], (i / 2), cmd);
			else if (check_type(cmd->red[i]) == REOUT)
				do_outfile(cmd->red[i + 1], (i / 2), cmd);
			else if (check_type(cmd->red[i]) == REAPP)
				do_append(cmd->red[i + 1], (i / 2), cmd);
			i += 2;
		}
		cmd = cmd->next;
	}
	setup_inout(d, tmp);
}
