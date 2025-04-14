/*
 * File: shell.c
 * Author: Saad Alarifi and Nasser Alqahtani
 * Description: A basic UNIX command line interpreter (simple_shell 0.1+)
 *              that handles command lines with arguments.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/**
 * parse_line - Splits a line into tokens (words) delimited by spaces, tabs,
 *              or newline characters.
 * @line: The input string from getline.
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
		perror("allocation error");
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
				perror("allocation error");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, " \t\r\n");
	}
	tokens[pos] = NULL;
	return (tokens);
}

/**
 * main - Entry point of the shell
 *
 * Return: Always 0.
 */
int main(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	pid_t pid;
	int status;

	while (1)
	{
		/* Display prompt in interactive mode */
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		nread = getline(&line, &len, stdin);
		if (nread == -1) /* End-of-file (Ctrl+D) */
			break;

		/* Remove trailing newline, if any */
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';

		/* Skip empty lines */
		if (line[0] == '\0')
			continue;

		/* Tokenize the input line into command & arguments */
		args = parse_line(line);
		if (args[0] == NULL)
		{
			free(args);
			continue;
		}

		pid = fork();
		if (pid == 0)
		{
			/* Child process: execute the command */
			if (execve(args[0], args, environ) == -1)
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
			/* Parent process: wait for child to finish */
			waitpid(pid, &status, 0);
		}

		free(args);
	}

	free(line);
	return (0);
}
