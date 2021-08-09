#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  printf("Welcome\n");
  size_t bufsize = 256;
  char *buffer = malloc(bufsize * sizeof(char));
  while (1) {
    print_sh_prefix();
    getline(&buffer, &bufsize, stdin);
    int code = analyze_input(buffer);
    if (code == 0) {
      break;
    }
  }
  free(buffer);
  printf("Exiting...\n");
  return 0;
}
