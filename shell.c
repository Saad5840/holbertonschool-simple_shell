/*
 * File: shell.c
 * Auth: Saad Alarifi and Nasser Alqahtani
 * Description: A minimal UNIX command line interpreter.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

extern char **environ;

/**
 * main - Entry point for the simple shell
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
		if (nread == -1) /* EOF (Ctrl+D) */
			break;

		/* remove newline */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		if (line[0] == '\0')
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
			wait(&status);
		}
	}

	free(line);
	return (0);
}
