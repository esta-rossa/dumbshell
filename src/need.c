/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   need.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arraji <arraji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/23 16:16:26 by arraji            #+#    #+#             */
/*   Updated: 2020/06/02 20:31:22 by arraji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dumbshell.h"

void	fd_saving(int savefd[2])
{
	static	int	flip = 0;

	if (flip == 0)
	{
		(savefd[0] = dup(0))  == -1 ? error(E_STANDARD, 1, NULL) : 1;
		(savefd[1] = dup(1))  == -1 ? error(E_STANDARD, 1, NULL) : 1;
	}
	else
	{
		dup2(savefd[0], 0) == -1 ? error(E_STANDARD, 1, NULL) : 1;
		close(savefd[0]);
		dup2(savefd[1], 1) == -1 ? error(E_STANDARD, 1, NULL) : 1;
		close(savefd[1]);
	}
	flip = flip == 0 ? 1 : 0;
}

void	dup_close(int fd1, int fd2)
{
	dup2(fd1, fd2) == -1 ? error(E_STANDARD, 1, NULL) : 1;
	close(fd1);
}

int	find_replace(t_env *var)
{
	t_env	*curr;

	curr = g_env;
	while (curr)
	{
		if (!ft_strcmp(curr->key, var->key))
		{
			var->next = curr->next;
			*curr = *var;
			free (var);
			return (1);
		}
		curr = curr->next;
	}
	return (0);
}
int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo"))
		return (0);
	if (!ft_strcmp(cmd,"cd"))
		return (1);
	if (!ft_strcmp(cmd,"pwd"))
		return (2);
	if (!ft_strcmp(cmd, "export"))
		return (3);
	if (!ft_strcmp(cmd, "env") )
		return (4);
	if (!ft_strcmp(cmd, "unset"))
		return (5);
	if (!ft_strcmp(cmd, "exit"))
		return (6);
	return (-1);
}
