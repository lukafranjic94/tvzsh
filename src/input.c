#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void analyze_input(char *input) {
  input[strcspn(input, "\n")] = '\0';
  int array_length = delim_count(input, ' ') + 2;
  char **array = malloc(array_length * sizeof(char *));
  to_array(array, input, ' ');
  // TODO: HANDLE ARRAY
  free_array(array);
  free(array);
}
