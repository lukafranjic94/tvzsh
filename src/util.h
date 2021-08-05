#ifndef UTIL_H
#define UTIL_H

char **to_array(char **array, char *input, char delim);
void get_cur_dir(char *cur_dir);
void get_hostname(char *hostname);
void get_username(char *username);
int delim_count(char *input, char delim);
void print_sh_prefix(void);
void free_array(char **array);
int get_array_length(char **array);

#endif