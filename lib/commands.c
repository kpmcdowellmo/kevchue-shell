#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

const char *__builtins_names[] = {"cd", "help", "exit"};

int __cd(char **args) {
  if (args[1] == NULL) {
    perror("kevchue!: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("kevchue!: Failed to change directory.");
    }
  }
  return 1;
}
int __help(char **args) {
  puts("The kevchue shell.");
  puts("Written by Keven McDowell 2025.");
  puts("Me trying to learn how to write my own shell. Use at your own risk!");
  puts("Built-in commands:");
  for (int i = 0; i < sizeof(__builtins_names) / sizeof(char *); ++i) {
    printf("\t%s\n", __builtins_names[i]);
  }
  puts("Please use the \"man\" command to read the manual for other "
       "commands.");
  return 1;
}
int __exit(char **args) { return 0; }

int (*__builtin_func[])(char **) = {&__cd, &__help, &__exit};

char *get_input() {
  char *buffer = NULL;
  size_t INITIAL_SIZE = 10;
  size_t buffer_size = INITIAL_SIZE;
  size_t len = 0;
  int c;

  buffer = (char *)malloc(INITIAL_SIZE);

  if (buffer == NULL) {
    puts("Memory allocation failed when processing command");
    exit(1);
  }

  while ((c = getchar()) != EOF && c != '\n') {
    if (len + 1 > buffer_size) {
      buffer_size *= 2;
      char *new_buffer = (char *)realloc(buffer, buffer_size);
      if (new_buffer == NULL) {
        free(buffer);
        puts("Memory allocation failed when processing command");
        abort();
      }
      buffer = new_buffer;
    }

    buffer[len++] = (char)c;
  }

  if (c == EOF) {
    free(buffer);
    puts("EOF found in stdin.");
    abort();
  }

  buffer[len] = '\0';

  return buffer;
}

Command get_command(char *input_str) {
  size_t buffer_size = strlen(input_str);
  unsigned int flags_count = 0, flags_length = 0;
  char *command = strtok(input_str, " ");
  char **flags = (char **)malloc(buffer_size);

  char *flag = strtok(NULL, " ");
  while (flag != NULL) {
    flags_length += strlen(flag);
    if (flags_length > buffer_size) {
      buffer_size += buffer_size - flags_length;
      char **new_flags = (char **)malloc(buffer_size);
      if (new_flags == NULL) {
        free(flags);
        puts("Memory allocation failed when processing command!");
        abort();
      }
      flags = new_flags;
    }
    flags[flags_count++] = flag;
    flag = strtok(NULL, " ");
  }

  Command command_data = {flags_count, command, flags};

  return command_data;
}

int __execute_command(char *command, char **args) {
  int status;
  pid_t pid = fork();

  if (pid == -1) {
    perror("fork");
  } else if (pid == 0) {
    // child
    if (execvp(command, args) == -1) {
      perror("kevchues");
    }
    exit(EXIT_FAILURE);
  } else {
    // parent
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && WIFSIGNALED(status));
  }

  return 1;
}

int execute_command(Command *command) {
  if (command->command == NULL) {
    return 1;
  }

  for (int i = 0; i < sizeof(__builtins_names) / sizeof(char *); ++i) {
    if (strcmp(command->command, __builtins_names[i]) == 0) {
      return (*__builtin_func[i])(command->flags);
    }
  }

  return __execute_command(command->command, command->flags);
}