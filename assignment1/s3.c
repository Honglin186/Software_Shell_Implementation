#include "s3.h"

///Simple for now, but will be expanded in a following section
void construct_shell_prompt(char shell_prompt[])
{
    strcpy(shell_prompt, "[s3]$ ");
}

///Prints a shell prompt and reads input from the user
void read_command_line(char line[])
{
    char shell_prompt[MAX_PROMPT_LEN];
    construct_shell_prompt(shell_prompt);
    printf("%s", shell_prompt);

    ///See man page of fgets(...)
    if (fgets(line, MAX_LINE, stdin) == NULL)
    {
        perror("fgets failed");
        exit(1);
    }
    ///Remove newline (enter)
    line[strlen(line) - 1] = '\0';
}

void parse_command(char line[], char *args[], int *argsc)
{
    ///Implements simple tokenization (space delimited)
    ///Note: strtok puts '\0' (null) characters within the existing storage, 
    ///to split it into logical cstrings.
    ///There is no dynamic allocation.

    ///See the man page of strtok(...)
    char *token = strtok(line, " ");
    *argsc = 0;
    while (token != NULL && *argsc < MAX_ARGS - 1)
    {
        args[(*argsc)++] = token;
        token = strtok(NULL, " ");
    }
    
    args[*argsc] = NULL; ///args must be null terminated
}

//returns 1 if the command contains a redirection operator (>, >> or <)
int command_with_redirection(char line[])
{
    if (strchr(line, '>') != NULL || strchr(line, '<') != NULL) {
        return 1;
    }
    return 0;
}

///Returns the index of the redirection operator in args, or -1 if none
int find_redirection(char *args[], int argsc)
{
    for (int i = 0; i < argsc; i++) {
        if (strcmp(args[i], ">") == 0 ||
            strcmp(args[i], "<") == 0 ||
            strcmp(args[i], ">>") == 0) {
            return i;
        }
    }
    return -1;
}

///Launch related functions
void child(char *args[], int argsc)
{
    ///Implement this function:

    ///Use execvp to load the binary 
    ///of the command specified in args[ARG_PROGNAME].
    ///For reference, see the code in lecture 3.
    execvp(args[ARG_PROGNAME], args);
    perror("exec failed");
    exit(1);
}

void launch_program(char *args[], int argsc)
{
    ///checks for if we need to exit immediately
    ///either nothing typed or 'exit' typed
    if (argsc == 0) {
        return;
    }

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    }

    ///Implement this function:

    ///fork() a child process.
    pid_t pid = fork();
    ///In the child part of the code,
    ///call child(args, argv)
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0) {
        child(args, argsc);
    }
    ///For reference, see the code in lecture 2.

    ///Handle the 'exit' command;
    ///so that the shell, not the child process,
    ///exits.
}

void launch_program_with_redirection(char *args[], int argsc)
{
}