#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  size_t bufsize = 4096;
  char *buffer = malloc(bufsize * sizeof(char));
  do {
    print_sh_prefix();
    getline(&buffer, &bufsize, stdin);
    analyze_input(buffer);
  } while (strcmp(buffer, "exit") != 0);
  free(buffer);
  return 0;
}
