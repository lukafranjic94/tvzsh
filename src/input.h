#ifndef INPUT_H
#define INPUT_H

int analyze_input(char *input);
int execute_builtins(char **array);
int execute_pipe(char *input);
int redirect_output(char *input);

#endif /* INPUT_H */