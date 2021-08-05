#include <limits.h>
#include <pwd.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

void print_sh_prefix(void) {
  size_t bufsize = 255;
  char *prefix_buffer = malloc(bufsize * sizeof(char));
  strcpy(prefix_buffer, "[");
  char *username = malloc(bufsize * sizeof(char));
  get_username(username);
  char *hostname = malloc(bufsize * sizeof(char));
  get_hostname(hostname);
  char *cur_dir = malloc(bufsize * sizeof(char));
  get_cur_dir(cur_dir);
  strcat(prefix_buffer, username);
  strcat(prefix_buffer, "@");
  strcat(prefix_buffer, hostname);
  strcat(prefix_buffer, " ");
  strcat(prefix_buffer, cur_dir);
  strcat(prefix_buffer, "] ");
  printf(prefix_buffer, "%s");
  free(username);
  free(hostname);
  free(cur_dir);
}

int delim_count(char *input, char delim) {
  int i = 0;
  int count = 0;
  for (i = 0; i < strlen(input); i++) {
    if (input[i] == delim)
      count++;
  }
  return count;
}

char **to_array(char **array, char *input, char delim) {
  char *token = NULL;
  int count = 0;
  token = strtok(input, &delim);
  while (token != NULL) {
    array[count] = malloc((strlen(token) + 1) * sizeof(char));
    strcpy(array[count], token);
    count++;
    token = strtok(NULL, &delim);
  }
  array[count] = NULL;
  return array;
}

void free_array(char **array) {
  int array_length = get_array_length(array);
  int i = 0;
  for (i = 0; i < array_length; i++) {
    free(array[i]);
  }
}

int get_array_length(char **array) {
  int counter = 0;
  char *ptr = array[0];
  while (ptr != NULL) {
    counter++;
    ptr = array[counter];
  }
  return counter;
}

void get_cur_dir(char *cur_dir) {
  char *buffer = malloc((PATH_MAX + 1) * sizeof(char));
  getcwd(buffer, PATH_MAX + 1);
  int array_length = delim_count(buffer, '/');
  char **array = malloc((array_length + 1) * sizeof(char **));
  to_array(array, buffer, '/');
  strcpy(cur_dir, array[array_length - 1]);
  free_array(array);
  free(array);
  free(buffer);
}

void get_hostname(char *hostname) { gethostname(hostname, HOST_NAME_MAX + 1); }

void get_username(char *username) {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw != NULL) {
    strcpy(username, pw->pw_name);
  } else {
    strcpy(username, "");
  }
}