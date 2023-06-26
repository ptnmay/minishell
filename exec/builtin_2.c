/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 18:10:04 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 19:56:45 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_env(t_d *d)
{
	ft_lstiter(d->env, print_env);
	d->exit_status = 0;
}

void	ft_unset(char **args, t_d *d)
{
	t_list	*tmp;
	t_env	*env;
	int		i;

	i = 0;
	tmp = d->env;
	while (args[++i])
	{
		if (!check_valid(args[i]) || ft_isdigit(args[i][0]))
		{
			error_print_format_3(args[0], args[1], 1);
			d->exit_status = 1;
			return ;
		}
		while (tmp)
		{
			env = tmp->content;
			if (!ft_strcmp(env->key, args[i])
				&& ft_strlen(env->key) == ft_strlen(args[i]))
				break ;
			tmp = tmp->next;
		}
		lst_delmid(d->env, tmp);
	}
	d->exit_status = 0;
}

void	is_good_digit(char **args, t_d *d)
{
	int	i;

	i = 0;
	if (args[1][i] == '-')
		i++;
	while (args[1][i])
	{
		if (!ft_isdigit(args[1][i]))
		{
			error_print_format_3(args[0], args[1], 2);
			free_for_all(d);
			exit(255);
		}
		i++;
	}
	if (ft_atoi(args[1]) < -2147483648 || ft_atoi(args[1]) > 2147483647)
	{
		error_print_format_3(args[0], args[1], 2);
		free_for_all(d);
		exit(255);
	}
}

void	ft_exit(char **args, t_d *d)
{
	int				size;
	unsigned int	ret;

	size = args_count(args);
	if (size > 1)
	{
		is_good_digit(args, d);
		if (size > 2)
		{
			error_print_format_2(args[0], 2);
			d->exit_status = 1;
			return ;
		}
		ret = (unsigned int)ft_atoi(args[1]);
	}
	else
		ret = 0;
	free_for_all(d);
	printf("exit\n");
	exit((int)ret);
}

void	ft_cd(char **args, t_d *d)
{
	int	size;

	size = args_count(args);
	if (size == 1 || (size == 2 && !ft_strcmp(args[1], "~")
			&& ft_strlen(args[1]) == 1))
		chdir(ft_getenv(d->env, "HOME"));
	else if (size >= 2)
	{
		if (chdir(args[1]))
		{
			error_print_format_3(args[0], args[1], 3);
			d->exit_status = 1;
			return ;
		}
	}
	d->exit_status = 0;
}
