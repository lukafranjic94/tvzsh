#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *buffer = NULL;
  do {
    print_sh_prefix();
    buffer = read_input();
  } while (strcmp(buffer, "exit\n") != 0);
  free(buffer);
  return 0;
}
