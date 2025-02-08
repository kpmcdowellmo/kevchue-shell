#include "commands.h"

int main() {
  int status;
  do {

    printf("kevchues!> ");

    char *input = get_input();
    Command command = get_command(input);

    printf("command: %s\n", command.command);

    for (int i = 0; i < command.flags_count; ++i) {
      printf("flag: %s\n", command.flags[i]);
    }

    status = execute_command(&command);
    free(input);
    free(command.flags);
  } while (status);

  return 0;
}