#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void analyze_input(char *input) {
  size_t bufsize = 255;
  char **array = malloc(bufsize * sizeof(char *));
  input[strcspn(input, "\n")] = 0;
}
