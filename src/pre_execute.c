/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pre_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arraji <arraji@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/22 12:32:22 by arraji            #+#    #+#             */
/*   Updated: 2020/06/04 06:47:10 by arraji           ###   ########.fr       */
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

static	bool	get_path(t_command *cmd, char *name)
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
			{
				free(name);
				return (error(E_NOCMD, 127, cmd->cmd_name));
			}
			break ;
		}
		curr = curr->next;
	}
	free(name);
	return (true);
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

static	void	prepare_fd(t_command *cmd, int pipefd[2], int savefd[2])
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

bool	pre_execute(t_command *cmd, int pipefd[2], int savefd[2], int builthin)
{
	struct	stat buf;

	if (builthin < 0)
	{
		if (with_path(cmd->cmd_name))
			cmd->full_path = ft_strdup(cmd->cmd_name);
		else
		{
			if (get_path(cmd, ft_strjoin("/", cmd->cmd_name)) == false)
				return (false);
		}
		if (stat(cmd->full_path, &buf) != 0)
			return (error(E_WPATH, 127, cmd->full_path));
		else if (S_ISDIR(buf.st_mode))
			return (error(E_ISDIR, 126, cmd->full_path));
	}
	prepare_fd(cmd, pipefd, savefd);
	return (true);
}
