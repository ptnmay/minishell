/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/13 12:18:19 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 21:48:58 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_from_path(char **token, t_d *d)
{
	char	**path;
	int		i;

	if (!token)
		return ;
	path = ft_split(ft_getenv(d->env, "PATH"), ':');
	i = -1;
	if (is_inside('/', token[0]))
		execve(token[0], token, d->envp);
	while (path && path[++i])
	{
		path[i] = ft_strjoin_premium(path[i], "/", 1);
		path[i] = ft_strjoin_premium(path[i], token[0], 1);
		if (!access(path[i], F_OK))
			execve(path[i], token, d->envp);
	}
	error_print_format_2(token[0], 1);
	free_2d(path);
	free_for_all(d);
	exit(127);
}

void	manage_direction(t_token *cmd)
{
	dup2(cmd->stat->infile, STDIN_FILENO);
	dup2(cmd->stat->outfile, STDOUT_FILENO);
}

void	handler_child(int sig)
{
	if (sig == SIGINT)
		printf("\n");
}

void	setup_child_signal(void)
{
	struct sigaction	s_int;

	s_int.sa_handler = handler_child;
	sigemptyset(&s_int.sa_mask);
	s_int.sa_flags = 0;
	sigaction(SIGINT, &s_int, NULL);
}

int	fork_exec(t_token *cmd, t_d *d, int type, int i)
{
	int	pid;

	setup_child_signal();
	pid = fork();
	if (pid == 0)
	{
		manage_direction(cmd);
		if (!type)
			execute_from_path(cmd->token, d);
		else if (type)
			do_builtin(cmd->token, d, 1);
		if (i != -1)
			clear_pipe(d->pipe, 1, 0);
	}
	close_main_pipe(cmd, d->pipe, i);
	return (pid);
}
