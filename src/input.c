#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  // TODO: HANDLE ARRAY
  free_array(array);
  free(array);
  return code;
}
