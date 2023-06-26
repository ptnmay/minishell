/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 20:22:02 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 21:49:16 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGTSTP || sig == SIGQUIT)
		return ;
}

void	set_control_c(void)
{
	struct termios	attributes;

	tcgetattr(STDIN_FILENO, &attributes);
	attributes.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDOUT_FILENO, TCSANOW, &attributes);
}

void	setup_signal(void)
{
	struct sigaction	s_int;
	struct sigaction	s_quit;
	struct sigaction	s_tstp;

	set_control_c();
	s_int.sa_handler = handler;
	sigemptyset(&s_int.sa_mask);
	s_int.sa_flags = 0;
	s_quit.sa_handler = SIG_IGN;
	sigemptyset(&s_quit.sa_mask);
	s_quit.sa_flags = 0;
	s_tstp.sa_handler = SIG_IGN;
	sigemptyset(&s_tstp.sa_mask);
	s_tstp.sa_flags = 0;
	sigaction(SIGINT, &s_int, NULL);
	sigaction(SIGTSTP, &s_quit, NULL);
	sigaction(SIGQUIT, &s_tstp, NULL);
}

int	main(int ac, char **av, char **envp)
{
	t_d		d;

	ac = (int) ac;
	av = (char **) av;
	init_env(&d, envp);
	printf("%s>> Start ENGINE!\n%s", KCYN, NONE);
	while (1)
	{
		setup_signal();
		d.buf = readline(KGRN "csantivi & psaeyang \% " NONE);
		if (!d.buf)
			break ;
		else if (ft_strlen(d.buf) && !is_all_wh(d.buf))
		{
			add_history(d.buf);
			if (lexer(&d))
				main_execute(&d);
			lst_clear(&d.tkn);
			free(d.buf);
		}
	}
	printf("%s>> EOF ;-;\n%s", KRED, NONE);
	ft_lstclear(&d.env, free_env);
	rl_clear_history();
}
