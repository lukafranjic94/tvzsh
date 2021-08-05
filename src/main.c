#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *buffer = NULL;
  do {
    buffer = read_input();
    printf(buffer, "%s");
  } while (strcmp(buffer, "exit\n") != 0);
  free(buffer);
  return 0;
}
