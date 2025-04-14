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

extern char **environ;

/**
 * is_blank - Check if a string is made up of only spaces or tabs
 * @str: Input string
 *
 * Return: 1 if only whitespace, 0 otherwise
 */
int is_blank(const char *str)
{
	while (*str)
	{
		if (*str != ' ' && *str != '\t')
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
		/* Display prompt if input is from a terminal */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* end-of-file (Ctrl+D) */
			break;

		/* Remove the trailing newline */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Skip empty or whitespace-only lines */
		if (line[0] == '\0' || is_blank(line))
			continue;

		pid = fork();
		if (pid == 0)
		{
			/* Instead of static initializer, assign elements separately */
			char *argv[2];
			argv[0] = line;
			argv[1] = NULL;

			if (execve(line, argv, environ) == -1)
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
