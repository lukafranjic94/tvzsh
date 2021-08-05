#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

void print_sh_prefix(void) {
  size_t bufsize = 255;
  char *prefix_buffer = malloc(bufsize * sizeof(char));
  strcpy(prefix_buffer, "[");
  const char *username = get_username();
  const char *hostname = get_hostname();
  const char *cur_dir = get_cur_dir();
  strcat(prefix_buffer, username);
  strcat(prefix_buffer, "@");
  strcat(prefix_buffer, hostname);
  strcat(prefix_buffer, " ");
  strcat(prefix_buffer, cur_dir);
  strcat(prefix_buffer, "] ");
  printf(prefix_buffer, "%s");
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

char **to_array(char *input, char delim) {
  size_t array_size = delim_count(input, delim) + 2;
  char **array = malloc(array_size * sizeof(char *));
  char *token = NULL;
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

const char *get_cur_dir(void) {
  char *buffer = malloc((PATH_MAX + 1) * sizeof(char));
  getcwd(buffer, PATH_MAX + 1);
  char **dir_arr = to_array(buffer, '/');
  int counter = 0;
  char *ptr = NULL;
  do {
    ptr = dir_arr[counter];
    counter++;
  } while (dir_arr[counter + 1] != NULL);
  return dir_arr[counter];
}

const char *get_hostname(void) {
  char *hostname = malloc((HOST_NAME_MAX + 1) * sizeof(char));
  gethostname(hostname, HOST_NAME_MAX + 1);
  return hostname;
}

const char *get_username(void) {
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw != NULL) {
    return pw->pw_name;
  }
  return "";
}