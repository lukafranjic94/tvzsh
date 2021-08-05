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

char **to_array(char *input) {
  size_t array_size = space_count(input) + 2;
  char **array = malloc(array_size * sizeof(char *));
  char *token = NULL;
  const char delim = ' ';
  int count = 0;
  token = strtok(input, &delim);
  while (token != NULL) {
    array[count] = token;
    count++;
    token = strtok(NULL, &delim);
  }
  array[count] = NULL;
  return array;
}

int space_count(char *input) {
  int i = 0;
  int count = 0;
  for (i = 0; i < strlen(input); i++) {
    if (input[i] == ' ')
      count++;
  }
  return count;
}