/*
 * File: shell.c
 * Author: Saad Alarifi and Nasser Alqahtani
 * Description: A simple UNIX command interpreter
 *              that handles the PATH and implements the built-in commands:
 *              exit and env. The shell does not fork if the command doesn't exist.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

extern char **environ;

/**
 * parse_line - Splits a line into tokens (command and arguments).
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
    return tokens;
}

/**
 * get_cmd_path - Searches for the command in the PATH.
 * @command: The command string (first token).
 *
 * If the command contains a slash, it is treated as an absolute or relative path.
 * Otherwise, the PATH environment variable is searched for an executable matching the command name.
 *
 * Return: A malloc'd string with the full path if found, or NULL.
 */
char *get_cmd_path(char *command)
{
    char *path_env, *path_copy, *dir, *full_path;
    size_t len;

    /* If command contains a slash, check it directly */
    if (strchr(command, '/'))
    {
        if (access(command, X_OK) == 0)
            return strdup(command);
        else
            return NULL;
    }

    /* Get the PATH environment variable */
    path_env = getenv("PATH");
    if (!path_env)
        return NULL;

    path_copy = strdup(path_env);
    if (!path_copy)
    {
        perror("strdup");
        exit(EXIT_FAILURE);
    }

    dir = strtok(path_copy, ":");
    while (dir != NULL)
    {
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
            return full_path;
        }
        free(full_path);
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}

/**
 * main - Entry point of the shell.
 *
 * Return: 0 on success.
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
        /* Display prompt if input is from a terminal */
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, ":) ", 3);

        nread = getline(&line, &len, stdin);
        if (nread == -1) /* End-of-file (Ctrl+D) */
            break;
        if (line[nread - 1] == '\n')
            line[nread - 1] = '\0';
        if (line[0] == '\0')
            continue;

        /* Parse the command line */
        args = parse_line(line);
        if (args[0] == NULL)
        {
            free(args);
            continue;
        }

        /* Built-in: exit */
        if (strcmp(args[0], "exit") == 0)
        {
            free(args);
            break;
        }

        /* Built-in: env */
        if (strcmp(args[0], "env") == 0)
        {
            int i = 0;
            while (environ[i])
            {
                printf("%s\n", environ[i]);
                i++;
            }
            free(args);
            continue;
        }

        /* Search for the command in PATH */
        cmd_path = get_cmd_path(args[0]);
        if (!cmd_path)
        {
            fprintf(stderr, "./simple_shell: 1: %s: not found\n", args[0]);
            free(args);
            continue;
        }
        /* Replace the command token with the full path */
        args[0] = cmd_path;

        /* Fork and execute the command */
        pid = fork();
        if (pid == 0)
        {
            if (execve(cmd_path, args, environ) == -1)
            {
                perror("./simple_shell");
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
    return 0;
}
