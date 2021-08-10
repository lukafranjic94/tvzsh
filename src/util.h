#ifndef UTIL_H
#define UTIL_H

char **to_array(char **array, char *input, char *delim);
int delim_count(char *input, char delim);
int get_array_length(char **array);
void free_array(char **array);
char *get_cur_dir(char *cur_dir);
char *get_hostname(char *hostname);
char *get_privilege_symbol(char *symbol);
char *get_username(char *username);
char *get_home_dir();
char *expand_shortcuts(char *input);
char *to_string(char *string, char **array, char *delim, int delim_count);
void print_sh_prefix(void);
int get_strlen_sum(char **array);
void print_array(char **array);

#endif