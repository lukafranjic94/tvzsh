#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_input(void) {
  size_t bufsize = 255;
  char *buffer = malloc(bufsize * sizeof(char));
  getline(&buffer, &bufsize, stdin);
  return buffer;
}
