#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  printf("Welcome\n");
  size_t bufsize = 256;
  char *buffer = malloc(bufsize * sizeof(char));
  int code = 0;
  do {
    print_sh_prefix();
    getline(&buffer, &bufsize, stdin);
    code = analyze_input(buffer);
  } while (code);
  free(buffer);
  printf("Exiting...\n");
  return 0;
}
