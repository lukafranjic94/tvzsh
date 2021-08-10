#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

enum STATUS_CODE { EXIT, BUILTIN, OTHER };

int execute_builtins(char **array) {
  if (strcmp(array[0], "exit") == 0) {
    return EXIT;
  } else if (strcmp(array[0], "cd") == 0) {
    if (array[1] != NULL) {
      if (chdir(array[1]) == -1) {
        perror("tvzsh");
        return BUILTIN;
      }
    } else {
      char *home_dir = get_home_dir();
      if (chdir(home_dir) == -1) {
        perror("tvzsh");
        return BUILTIN;
      }
    }
    return BUILTIN;
  }
  return OTHER;
}

int analyze_input(char *input) {
  if (strlen(input) == 1) {
    return 1;
  }
  input[strcspn(input, "\n")] = '\0';
  input = expand_shortcuts(input);
  int array_length = delim_count(input, ' ') + 2;
  char **array = malloc(array_length * sizeof(char *));
  array = to_array(array, input, " ");
  int code = execute_builtins(array);
  if (code == EXIT || code == BUILTIN) {
    free_array(array);
    free(array);
    return code;
  }

  pid_t wpid, pid;
  int status;
  pid = fork();
  if (pid == 0) {
    if (execvp(array[0], array) == -1) {
      perror("tvzsh");
      free_array(array);
      free(array);
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    perror("tvzsh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  free_array(array);
  free(array);
  return code;
}
