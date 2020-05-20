/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arraji <arraji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/18 16:04:52 by arraji            #+#    #+#             */
/*   Updated: 2020/05/20 17:40:15 by arraji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dumbshell.h"

void	freak_out(int bits, char *line, int index)
{
	if (AND(bits, BPIPE))
		error(E_SYNTAX, &line[index]);
	else if (AND(bits, BD_Q))
		error(E_SYNTAX, &line[index]);
	else if (AND(bits, BS_Q))
		error(E_SYNTAX, &line[index]);
	else if (AND(bits, BRED_TO) || AND(bits, BRED_TO_APP)
	|| AND(bits, BRED_FROM))
		error(E_SYNTAX, &line[index]);
}

void	chill(int *bits)
{
	BIT_OFF(*bits, BPIPE);
	BIT_OFF(*bits, BSEMIC);
	BIT_OFF(*bits, BCHECK);
	BIT_OFF(*bits, BRED_FROM);
	BIT_OFF(*bits, BRED_TO);
	BIT_OFF(*bits, BRED_TO_APP);
}

int		error(int err, char *need)
{
	if (need && *need == '\0')
		need = "newline";
	else if (err == E_SYNTAX && need)
	{
		need[0] = *need;
		need[1] = '\0';
	}
	ft_fprintf(2, "dumbshell: ");
	err == E_SYNTAX ? ft_fprintf(2, "parse error near `%s'\n", need) : 1;
	err == E_STANDARD ? ft_fprintf(2, "%s.\n", strerror(errno)) : 1;
	err == E_FILE ? ft_fprintf(2, "`%s` %s\n", need, strerror(errno)) : 1;
	exit(err);
}

void	checker(char *line)
{
	int index = -1;

	write(1 , "check : ", 9);
	while (line[++index])
	{
		if (line[index] > 0)
		{
			ft_putchar_fd(line[index], 1);
			continue;
		}
		if (line[index] == WORD_SEP)
			ft_fprintf(1, "%s<word sep>%s", PRINT_RED, RESET);
		else if (line[index] == REMOVED)
			ft_fprintf(1, "%s<removed>%s", PRINT_RED, RESET);
		else if (line[index] == PIPELINE_SEP)
			ft_fprintf(1, "%s<pipeline sep>%s", PRINT_RED, RESET);
		else if (line[index] == CMD_SEP)
			ft_fprintf(1, "%s<cmd sep>%s", PRINT_RED, RESET);
		else if (line[index] == VAR)
			ft_fprintf(1, "%s<var>%s", PRINT_RED, RESET);
		else if (line[index] == RED_TO)
			ft_fprintf(1, "%s<red to>%s", PRINT_RED, RESET);
		else if (line[index] == RED_FROM)
			ft_fprintf(1, "%s<red from>%s", PRINT_RED, RESET);
		else if (line[index] == RED_TO_APP)
		{
			ft_fprintf(1, "%s<red to app>%s", PRINT_RED, RESET);
			// index++;
		}
	}
	write(1, "\n", 1);
}

void	list_checker(t_all *all)
{
	int		pipe_number;
	int		command_number;
	t_command	*cmd;
	t_pipeline	*pipe;
	t_args		*args;

	pipe_number = 1;
	pipe = all->pipe;
	while (pipe)
	{
		cmd = pipe->cmd_head;
		command_number = 1;
		ft_fprintf(1, "pipe %d:\n", pipe_number);
		while (cmd)
		{
			args = cmd->list_args;
			ft_fprintf(1, "cmd %d: ", command_number);
			while (args)
			{
				if (args->type == 0)
				{
					if (args->c == WORD_SEP)
						ft_fprintf(1, "%s<WORD SEP>%s", PRINT_RED, RESET);
					else
						ft_fprintf(1, "%c", args->c);
				}
				else
				{
					ft_fprintf(1, "%s", args->str);
				}
				args = args->next;
			}
			command_number++;
			write(1, "\n", 1);
			cmd = cmd->next;
		}
		pipe_number++;
		pipe = pipe->next;
	}
}
