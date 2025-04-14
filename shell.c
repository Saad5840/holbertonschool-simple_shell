/*
 * File: shell.c
 * Author: Saad Alarifi and Nasser Alqahtani
 * Description: A basic UNIX command line interpreter (simple_shell 0.2+)
 *              that handles the PATH. Fork is not called if the command
 *              does not exist.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/**
 * parse_line - Splits a line into tokens separated by whitespace.
 * @line: The input string.
 *
 * Return: A NULL-terminated array of tokens.
 */
char **parse_line(char *line)
{
	int bufsize = 64, pos = 0;
	char **tokens = malloc(bufsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token = strtok(line, " \t\r\n");
	while (token != NULL)
	{
		tokens[pos] = token;
		pos++;
		if (pos >= bufsize)
		{
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char *));
			if (!tokens)
			{
				perror("realloc");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " \t\r\n");
	}
	tokens[pos] = NULL;
	return (tokens);
}

/**
 * get_cmd_path - Finds the full path for a command.
 * @command: The command string.
 *
 * If the command contains a '/', it is treated as an absolute or relative path.
 * Otherwise, the PATH environment variable is searched for the executable.
 *
 * Return: A malloc'd string containing the full path if found, or NULL if not.
 */
char *get_cmd_path(char *command)
{
	char *path_env, *path_copy, *dir, *full_path;
	size_t len;

	/* If command contains a slash, check it directly */
	if (strchr(command, '/'))
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		else
			return (NULL);
	}

	/* Get the PATH environment variable */
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	dir = strtok(path_copy, ":");
	while (dir != NULL)
	{
		/* Construct candidate path: dir/command */
		len = strlen(dir) + 1 + strlen(command) + 1;
		full_path = malloc(len);
		if (!full_path)
		{
			perror("malloc");
			free(path_copy);
			exit(EXIT_FAILURE);
		}
		snprintf(full_path, len, "%s/%s", dir, command);
		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}

/**
 * main - Entry point of the shell.
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	char *cmd_path;
	pid_t pid;
	int status;

	while (1)
	{
		/* Display prompt in interactive mode */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, ":) ", 3);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* End-of-file (Ctrl+D) */
			break;

		/* Remove the trailing newline, if present */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Skip empty lines */
		if (line[0] == '\0')
			continue;

		/* Parse the line into tokens (command and arguments) */
		args = parse_line(line);
		if (args[0] == NULL)
		{
			free(args);
			continue;
		}

		/* Resolve the command path using PATH */
		cmd_path = get_cmd_path(args[0]);
		if (!cmd_path)
		{
			fprintf(stderr, "./shell: 1: %s: not found\n", args[0]);
			free(args);
			continue;
		}

		/* Replace args[0] with the full path */
		args[0] = cmd_path;

		/* Fork and execute only if the command exists */
		pid = fork();
		if (pid == 0)
		{
			if (execve(cmd_path, args, environ) == -1)
			{
				perror("./shell");
				exit(EXIT_FAILURE);
			}
		}
		else if (pid < 0)
		{
			perror("fork");
		}
		else
		{
			waitpid(pid, &status, 0);
		}

		free(cmd_path);
		free(args);
	}

	free(line);
	return (0);
}
