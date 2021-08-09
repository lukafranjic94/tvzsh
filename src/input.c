#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int analyze_input(char *input) {
  if (strlen(input) == 1) {
    return 1;
  }
  input[strcspn(input, "\n")] = '\0';
  int array_length = delim_count(input, ' ') + 2;
  char **array = malloc(array_length * sizeof(char *));
  array = to_array(array, input, " ");
  int code = strcmp(array[0], "exit") == 0 ? 0 : 1;
  if (code == 0) {
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
