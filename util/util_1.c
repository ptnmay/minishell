/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/03 00:19:03 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 15:38:25 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_quote(char *s, int i)
{
	int	q;
	int	start;

	start = i;
	q = s[i];
	i++;
	while (s[i] && s[i] != q)
		i++;
	if (!s[i])
		i--;
	i++;
	return (i - start);
}

char	*ft_strjoin_premium(char *s1, char *s2, int i)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	if (s1 && i != 2 && i)
		free(s1);
	if (s2 && i != 1 && i)
		free(s2);
	return (new);
}

enum e_token	check_type(char *c)
{
	if (c[0] == '|')
		return (PIPE);
	else if (c[0] == '<' && c[1] && c[1] == c[0])
		return (HDOC);
	else if (c[0] == '>' && c[1] && c[1] == c[0])
		return (REAPP);
	else if (c[0] == '>' || (c[0] == '<' && c[1] == '>'))
		return (REOUT);
	else if (c[0] == '<')
		return (REIN);
	else
		return (CMD);
}

int	expand_var_normal(char **new, char *s, int i, t_d *d)
{
	char	*var_name;
	char	*value;
	int		len;

	if (s[i] == '$')
		i++;
	len = varlen(s, i);
	if (!len && (s[i] && s[i] != '\'' && s[i] != '\"'))
	{
		printf("WHY?? [%c]\n", s[i]);
		*new = ft_strjoin_premium(*new, "$\0", 1);
	}
	else if (s[i] == '?')
		*new = ft_strjoin_premium(*new, value = ft_itoa(d->exit_status), 3);
	else
	{
		var_name = ft_substr(s, i, len);
		value = ft_getenv(d->env, var_name);
		if (!value)
			value = "\0";
		free(var_name);
		*new = ft_strjoin_premium(*new, value, 1);
	}
	return (len + 1);
}

int	expand_var_quote(char **new, char *s, int i, t_d *d)
{
	char	*var_name;
	char	*value;
	int		len;

	if (s[i] == '$')
		i++;
	len = varlen(s, i);
	if (!len)
		*new = ft_strjoin_premium(*new, "$\0", 1);
	else if (s[i] == '?')
		*new = ft_strjoin_premium(*new, value = ft_itoa(d->exit_status), 3);
	else
	{
		var_name = ft_substr(s, i, len);
		value = ft_getenv(d->env, var_name);
		if (!value)
			value = "\0";
		free(var_name);
		*new = ft_strjoin_premium(*new, value, 1);
	}
	return (len + 1);
}
