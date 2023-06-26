/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csantivi <csantivi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 16:15:02 by csantivi          #+#    #+#             */
/*   Updated: 2023/06/14 21:55:37 by csantivi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <dirent.h>
# include <errno.h>
# include "../libft/libft.h"
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <fcntl.h>

# define KNRM  "\x1B[0m"
# define KRED  "\x1B[31m"
# define KGRN  "\x1B[32m"
# define KYEL  "\x1B[33m"
# define KBLU  "\x1B[34m"
# define KMAG  "\x1B[35m"
# define KCYN  "\x1B[36m"
# define KWHT  "\x1B[37m"
# define NONE  "\033[0m"

# define BLKB "\e[40m"
# define REDB "\e[41m"
# define GRNB "\e[42m"
# define YELB "\e[43m"
# define BLUB "\e[44m"
# define MAGB "\e[45m"
# define CYNB "\e[46m"
# define WHTB "\e[47m"

# define MAX_HISTORY 100

enum e_token
{
	CMD,
	PIPE,
	REIN,
	HDOC,
	REOUT,
	REAPP,
	UNKNOW
};

typedef struct s_fd
{
	int		*pipe_fd;
	int		size;
	int		i;
}		t_fd;

typedef struct s_redirect
{
	int	is_error;
	int	infile;
	int	outfile;
}			t_redirect;

typedef struct s_token
{
	char				**token;
	char				**red;
	int					*red_fd;
	char				*str;
	enum e_token		type;
	struct s_redirect	*stat;
	struct s_token		*next;
}			t_token;

typedef struct s_env
{
	char	*key;
	char	*value;
}			t_env;

typedef struct s_d
{
	int		exit_status;
	char	*buf;
	char	**envp;
	t_list	*env;
	t_token	*tkn;
	t_fd	*pipe;
}			t_d;

// LEXER
// lexer.c
int				lexer(t_d *d);
int				varlen(char *s, int i);
int				check_special(char *str);
int				is_all_wh(char *s);
// split_metachar.c
int				is_meta(char c);
int				case_cut_list(t_token **h, t_token *p, int i, int pos);
void			split_metachar(t_d *d, int pos, int size, int i);
void			define_type(t_token *ptr);
// split_to_list.c
int				count_word(char *str);
char			*ft_strndup(char *str, int n);
char			*makestr(char *str, int *k);
char			**fill_word(char *s, int n, char **str);
void			split_to_list(t_d *d, int i, int n);
// parser.c
void			parser(t_d *d);
char			*clear_quote_expand(char *str, t_d *d);
// fill_in.c
int				fill_squote(char **new, char *s, int i);
int				fill_string(char **new, char *s, int i, t_d *d);
int				fill_dquote(char **new, char *s, int i, t_d *d);
int				fill_in_str(char **new, char *s, int i);
int				fill_in_dq(char **new, char *s, int i);
// join_cmd.c
void			join_cmd(t_d *d);

// EXECUTE
// execute_1.c
void			main_execute(t_d *d);
void			clear_pipe(t_fd *pipe, int extra, int freed);
void			close_main_pipe(t_token *cmd, t_fd *p, int i);
// execute_2.c
void			handler_child(int sig);
void			setup_child_signal(void);
void			execute_from_path(char **token, t_d *d);
int				fork_exec(t_token *cmd, t_d *d, int type, int i);
// builtin_1.c
void			ft_pwd(t_d *d);
void			ft_echo(char **args, t_d *d);
int				check_valid(char *str);
void			ft_export(char **args, t_d *d);
// builtin_2.c
void			ft_env(t_d *d);
void			ft_unset(char **args, t_d *d);
void			ft_exit(char **args, t_d *d);
void			ft_cd(char **args, t_d *d);
// redi_1.c
void			do_here(t_token *h);
void			do_redirect(t_d *d, t_token *cmd);
char			*create_name(char *file, int i, int id);
// redi_2.c
void			do_outfile(char *file, int i, t_token *cmd);
void			do_append(char *file, int i, t_token *cmd);
void			setup_inout(t_d *d, t_token *cmd);

// UTIL
// free.c
void			free_2d(char **input);
void			free_env(void *content);
void			free_for_all(t_d *d);
void			close_fd(t_token *tkn);
void			delete_file(t_token *cmd);
// lst_1.c
t_token			*lst_new(char *str, enum e_token type);
t_token			*lst_last(t_token *tkn);
void			lst_addfront(t_token **tkn, t_token *new);
void			lst_addback(t_token **tkn, t_token *new);//
int				lst_size(t_token *tkn);
// lst_2.c
void			lst_delone(t_token *tkn);
void			lst_clear(t_token **tkn);
void			lst_iter(t_token *tkn, void (*f)(char *));
void			lst_insert(t_token **lst, t_token *new, int pos);
void			lst_delmid(t_list *head, t_list *terget);
// util_1.c
int				skip_quote(char *s, int i);
char			*ft_strjoin_premium(char *s1, char *s2, int option);
enum e_token	check_type(char *c);
int				expand_var_normal(char **new, char *s, int i, t_d *d);
int				expand_var_quote(char **new, char *s, int i, t_d *d);

// util_2.c
int				check_builtin(char **args);
int				do_builtin(char **str, t_d *d, int do_fork);
int				args_count(char **str);
int				is_inside(char c, char *set);
// util_3.c
int				check_error_arrange(t_d *d);
void			error_print_format_1(char *s1);
void			error_print_format_2(char *s1, int option);
void			error_print_format_3(char *s1, char *s2, int option);

// show.c
void			show_2d(char **input);
void			show_cmd(t_token *cmd);
void			show_red(t_token *cmd);
void			print_tkn(char *str);

// SRC/ENV
// env.c
char			*ft_getenv(t_list *my_env, char *str);
void			print_env(void *content);
void			init_env(t_d *d, char **envp);
void			create_new_env(char *key, char *value, t_d *d);

#endif

/*
	(LV.1) linked-list : str -> str -> str
	
	for show: 	lst_iter(d->tkn, print_tkn);
	for free: 	lst_clear(&d.tkn);

	(LV.2) linked-list : cmd -> cmd -> cmd

	fot show:	show_cmd(head);
	for free:	lst_clear(&head);
*/

/*
	t_token	*tmp;
	
	printf("from split : ");	//  --|
	lst_iter(d->tkn, print_tkn); //   |
	printf("\n");				 // --|
	printf("type       : ");		// ----
	tmp = d->tkn;							//
	while (tmp)								//
	{										//
		printf(" %d ->", tmp->type);		//
		tmp = tmp->next;					//
	}										//
	printf("\n");					// ----

	printf("from join  : ");	 //  --|
	show_cmd(d->tkn); //   |
	printf("type       : ");		// ----
	tmp = d->tkn;							//
	while (tmp)								//
	{										//
		printf(" %d ->", tmp->type);		//
		tmp = tmp->next;					//
	}										//
	printf("\n\n");					// ----
*/
