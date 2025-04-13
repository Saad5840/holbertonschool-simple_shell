#include "shell.h"

/**
 * main - Entry point for the simple shell
 * @argc: Number of arguments
 * @argv: Array of argument strings
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	int status = 1;

	(void)argc;

	while (isatty(STDIN_FILENO) && status)
	{
		write(STDOUT_FILENO, "($) ", 4);
		nread = getline(&line, &len, stdin);
		if (nread == -1)
			break;
		if (line[nread - 1] == '\n')
			line[nread - 1] = '\0';
		args = parse_input(line);
		if (args == NULL)
			continue;
		status = execute(args, argv[0]);
		free(args);
	}

	free(line);
	return (0);
}
