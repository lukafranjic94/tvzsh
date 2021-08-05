#ifndef UTIL_H
#define UTIL_H

char **to_array(char *input, char delim);
const char *get_cur_dir(void);
const char *get_hostname(void);
const char *get_username(void);
int delim_count(char *input, char delim);
void print_sh_prefix(void);

#endif