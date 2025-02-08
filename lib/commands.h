#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <unistd.h>


/**
 */
typedef struct {
  unsigned int flags_count;
  char *command;
  char **flags;
} Command;

/**
 */
char *get_input();

/**
 */
Command get_command(char *input_str);

/**
 */
int execute_command(Command *command);

#endif