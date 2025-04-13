/*
 * File: shell.c
 * Author: Saad Alarifi and Nasser Alqahtani
 * Description: A basic UNIX command line interpreter (simple_shell)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

extern char **environ;

/**
 * is_blank - Check if a string is only whitespace
 * @str: Input string
 *
 * Return: 1 if only whitespace, 0 otherwise
 */
int is_blank(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t' && *str != '\n')
			return (0);
		str++;
	}
	return (1);
}

/**
 * main - Entry point of the shell
 *
 * Return: Always 0
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	pid_t pid;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* Ctrl+D */
			break;

		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0' || is_blank(line))
			continue;

		pid = fork();
		if (pid == 0) /* child */
		{
			char *argv[2];

			argv[0] = line;
			argv[1] = NULL;

			if (execve(argv[0], argv, environ) == -1)
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
	}

	free(line);
	return (0);
}
