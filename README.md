# Simple shell

![Simple shell](https://i.imgur.com/9m8edhj.jpg)

## Table of Contents
- [Description](#description)
- [File Structure](#file-structure)
- [System and Library Calls](#system-and-library-calls)
- [Installation](#installation)
- [Authors](#authors)

## Description
Simple shell is a minimalist UNIX command line interpreter written in C. It mimics the behavior of `/bin/sh` by:
- Displaying a prompt and waiting for the user input.
- Parsing command lines—supporting commands with arguments.
- Searching the PATH for commands (if a slash is not present in the command).
- Executing commands via fork and execve (passing the environment).
- Handling errors, including the end-of-file condition (Ctrl+D).
- Not executing commands if the executable cannot be found.

**Basic flow of the shell:**
1. Start the shell.
2. Display a prompt and wait for user input.
3. Read and parse the command line.
4. Check for the command in the PATH.
5. Execute the command (if found) via fork/execve.
6. Return the output and display the prompt again.
7. Terminate the shell on the `exit` command or on EOF (Ctrl+D).

## File Structure
This table provides a brief overview of the core files in this project. Click on a file name to view its source code in the repository.

| File                             | Content/Functionality                                                     | Description                                                            |
| -------------------------------- | ------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| [README.md](README.md)           | Documentation                                                             | This file – provides an overview, installation instructions, examples. |
| [man_1_simple_shell](man_1_simple_shell) | Manual page for simple shell                                            | Contains the formatted manual (man page) for the shell.                |
| [AUTHORS](AUTHORS)               | Contributor list                                                          | Lists all individuals who contributed to the project.                |
| [shell.c](shell.c)               | Shell source code                                                         | Core implementation of the UNIX shell, including parsing and execution. |
| [shell.h](shell.h) *(if present)*| Header file                                                               | Contains prototypes, macros, and external variable declarations.     |

## System and Library Calls
This project uses several system and library calls. Below is a summary with links to their manual pages:

| Name       | Manual Page                                                                                  | Brief Description                                                                                   |
| ---------- | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| `access`   | [man 2 access](https://man7.org/linux/man-pages/man2/access.2.html)                          | Checks file accessibility.                                                                          |
| `chdir`    | [man 2 chdir](https://man7.org/linux/man-pages/man2/chdir.2.html)                            | Changes the current directory.                                                                      |
| `execve`   | [man 2 execve](https://man7.org/linux/man-pages/man2/execve.2.html)                          | Replaces the current process image with a new process image.                                        |
| `fork`     | [man 2 fork](https://man7.org/linux/man-pages/man2/fork.2.html)                              | Creates a new process by duplicating the calling process.                                           |
| `free`     | [man 3 free](https://man7.org/linux/man-pages/man3/free.3.html)                              | Frees dynamically allocated memory.                                                                 |
| `getcwd`   | [man 3 getcwd](https://man7.org/linux/man-pages/man3/getcwd.3.html)                          | Retrieves the current working directory.                                                            |
| `getenv`   | [man 3 getenv](https://man7.org/linux/man-pages/man3/getenv.3.html)                          | Searches for an environment variable.                                                               |
| `getline`  | [man 3 getline](https://man7.org/linux/man-pages/man3/getline.3.html)                        | Reads an entire line from a stream.                                                                 |
| `isatty`   | [man 3 isatty](https://man7.org/linux/man-pages/man3/isatty.3.html)                          | Checks if a file descriptor refers to a terminal.                                                   |
| `malloc`   | [man 3 malloc](https://man7.org/linux/man-pages/man3/malloc.3.html)                          | Allocates dynamic memory.                                                                           |
| `perror`   | [man 3 perror](https://man7.org/linux/man-pages/man3/perror.3.html)                          | Prints a descriptive error message to stderr.                                                       |
| `signal`   | [man 2 signal](https://man7.org/linux/man-pages/man2/signal.2.html)                          | Sets up a signal handler for asynchronous events.                                                 |
| `strtok`   | [man 3 strtok](https://man7.org/linux/man-pages/man3/strtok.3.html)                          | Tokenizes a string into substrings based on delimiters.                                             |
| `waitpid`  | [man 2 waitpid](https://man7.org/linux/man-pages/man2/waitpid.2.html)                        | Waits for process state changes.                                                                    |
| `write`    | [man 2 write](https://man7.org/linux/man-pages/man2/write.2.html)                            | Writes data to a file descriptor.                                                                   |

## Installation
To install and run the shell:

1. **Clone the repository:**

   ```bash
   git clone https://github.com/Saad5840/holbertonschool-simple_shell.git

## Authors
Saad Alarifi
Nasser Alqahtani
