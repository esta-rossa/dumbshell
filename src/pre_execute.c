/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arraji <arraji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/22 12:32:22 by arraji            #+#    #+#             */
/*   Updated: 2020/05/23 12:53:38 by arraji           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dumbshell.h"

static	char	*parse_path(char *all_paths, char *name)
{
	char	**path;
	int		index;
	char	*full_path;
	struct stat buf;

	index = 0;
	path = ft_split(all_paths, ':');
	while (path[index])
	{
		if (path[index][ft_strlen(path[index])] == '/')
			path[index][ft_strlen(path[index])] = 0;
		full_path = ft_strjoin(path[index], name);
		if (stat(full_path, &buf) == 0)
		{
			free_tab(path, ft_tablen(path));
			return (full_path);
		}
		free(full_path);
		index++;
	}
	free_tab(path, ft_tablen(path));
	return (NULL);
}

static	void	get_path(t_command *cmd, char *name)
{
	t_env	*curr;
	char	*path;

	curr = g_env;
	while (curr)
	{
		if (ft_strcmp(curr->key, "PATH") == 0)
		{
			path = parse_path(curr->value, name);
			if (path != NULL)
				cmd->full_path = path;
			else
				error(E_NOCMD, cmd->cmd_name);
		break ;
		}
		curr = curr->next;
	}
	free(name);
}

static	int		with_path(char *name)
{
	int index;

	index = 0;
	while (name[index])
	{
		if (name[index] == '/')
			return (1);
		index++;
	}
	return (0);
}
void	dup_close(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd1);
}

void	pre_execute(t_command *cmd, int pipefd[2], int savefd[2])
{
	struct	stat buf;

	{
	if (with_path(cmd->cmd_name))
		cmd->full_path = ft_strdup(cmd->cmd_name);
	else
		get_path(cmd, ft_strjoin("/", cmd->cmd_name));
	if (stat(cmd->full_path, &buf) != 0)
		error(E_WPATH, cmd->full_path);
	else if (S_ISDIR(buf.st_mode))
		error(E_ISDIR, cmd->full_path);
	}
	{
		pipe(pipefd);
		if (cmd->file && AND(cmd->read_type, RED_FROM * -1))
			dup_close(cmd->fd, STDIN_FILENO);
		else if(cmd->file && (AND(cmd->read_type, RED_TO * -1) || AND(cmd->read_type, RED_TO_APP * -1)))
			dup_close(cmd->fd, STDOUT_FILENO);
		else if (cmd->next)
			dup_close(pipefd[WRITE_END], STDOUT_FILENO);
		else if (cmd->next == NULL)
			dup2(savefd[1], STDOUT_FILENO);
	}
}
