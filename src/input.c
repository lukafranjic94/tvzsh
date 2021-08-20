#include "input.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

enum STATUS_CODE { EXIT, BUILTIN, OTHER };

int execute_builtins(char **array) {
  if (!array) {
    fprintf(stderr, "Array should not be NULL\n");
    exit(EXIT_FAILURE);
  }
  if (strcmp(array[0], "exit") == 0) {
    return EXIT;
  } else if (strcmp(array[0], "cd") == 0) {
    if (array[1] != NULL) {
      if (chdir(array[1]) == -1) {
        perror("tvzsh");
        return BUILTIN;
      }
    } else {
      char *home_dir = get_home_dir();
      if (chdir(home_dir) == -1) {
        perror("tvzsh");
        return BUILTIN;
      }
    }
    return BUILTIN;
  }
  return OTHER;
}

int execute_pipe(char *input) {
  int array_length = delim_count(input, '|') + 2;
  char **array = malloc(array_length * sizeof(char *));
  array = to_array(array, input, "|");
  char *first = array[0];
  char *second = array[1];
  int length_first = delim_count(first, ' ') + 2;
  char **arr_first = malloc(length_first * sizeof(char *));
  arr_first = to_array(arr_first, array[0], " ");
  int length_second = delim_count(second, ' ') + 2;
  char **arr_second = malloc(length_second * sizeof(char *));
  arr_second = to_array(arr_second, array[1], " ");

  int pipefd[2], status1, status2;
  pid_t p1, p2, wpid1, wpid2;

  if (pipe(pipefd) < 0) {
    perror("tvzsh");
    free_array(arr_first);
    free_array(arr_second);
    free_array(array);
    free(arr_first);
    free(arr_second);
    free(array);
    exit(EXIT_FAILURE);
  }
  p1 = fork();
  if (p1 < 0) {
    perror("tvzsh");
  }

  if (p1 == 0) {
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    if (execvp(arr_first[0], arr_first) < 0) {
      perror("tvzsh");
      exit(EXIT_FAILURE);
    }
  } else {
    p2 = fork();

    if (p2 < 0) {
      perror("tvzsh");
    }

    if (p2 == 0) {
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);

      if (execvp(arr_second[0], arr_second) < 0) {
        perror("tvzsh");
        exit(EXIT_FAILURE);
      }
    } else {
      close(pipefd[0]);
      close(pipefd[1]);
      wait(NULL);
      wait(NULL);
    }
  }
  free_array(arr_first);
  free_array(arr_second);
  free_array(array);
  free(arr_first);
  free(arr_second);
  free(array);
  return 1;
}

int redirect_output(char *input) {
  int array_length = delim_count(input, '>') + 2;
  char **array = malloc(array_length * sizeof(char *));
  array = to_array(array, input, ">");
  char *first = array[0];
  char *second = array[1];
  int length_first = delim_count(first, ' ') + 2;
  char **arr_first = malloc(length_first * sizeof(char *));
  arr_first = to_array(arr_first, array[0], " ");
  int length_second = delim_count(second, ' ') + 2;
  char **arr_second = malloc(length_second * sizeof(char *));
  arr_second = to_array(arr_second, array[1], " ");

  pid_t wpid, pid;
  int status;
  pid = fork();
  if (pid == 0) {
    FILE *fp;
    int fd;
    fp = fopen(arr_second[0], "w");
    fd = fileno(fp);
    dup2(fd, STDOUT_FILENO);
    fclose(fp);
    if (execvp(arr_first[0], arr_first) == -1) {
      perror("tvzsh");
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    perror("tvzsh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  free_array(arr_first);
  free_array(array);
  free(arr_first);
  free(array);
  return 1;
}

int analyze_input(char *input) {
  if (!input) {
    fprintf(stderr, "Input should not be NULL\n");
    exit(EXIT_FAILURE);
  }
  if (strcmp(input, "\n") == 0) {
    return OTHER;
  }
  input[strcspn(input, "\n")] = '\0';
  input = expand_shortcuts(input);
  int pipe_count = delim_count(input, '|');
  int output_redirect_count = delim_count(input, '>');
  if (pipe_count > 1) {
    fprintf(stdout, "Currently, no more than one pipe operator is supported\n");
    return OTHER;
  }
  if (pipe_count == 1) {
    return execute_pipe(input);
  }
  if (output_redirect_count > 1) {
    fprintf(stdout, "Currently, no more than one output redirection operator "
                    "is supported\n");
  }

  if (output_redirect_count == 1) {
    return redirect_output(input);
  }

  int array_length = delim_count(input, ' ') + 2;
  char **array = malloc(array_length * sizeof(char *));
  array = to_array(array, input, " ");
  int code = execute_builtins(array);
  if (code == EXIT || code == BUILTIN) {
    free_array(array);
    free(array);
    return code;
  }

  pid_t wpid, pid;
  int status;
  pid = fork();
  if (pid == 0) {
    if (execvp(array[0], array) == -1) {
      perror("tvzsh");
      free_array(array);
      free(array);
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    perror("tvzsh");
  } else {
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  free_array(array);
  free(array);
  return code;
}
