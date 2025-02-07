#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_ST 1024

enum global_flags_enum {
  number,
  number_nonblank,
  show_ends,
  show_tabs,
  squeeze_blank,
  v,
  all
};

void output_file(const char *file);
void set_flag(enum global_flags_enum);
int v_flag_itter(char *symb);
int e_flag_itter(char symb);
int t_flag_itter(char *symb);
int n_flag_itter(char symb, int begin_line_num, int *line_counter);
int b_falg_itter(char previous_char, int *empty_line_counter, char ch);
int s_falg_itter(char previous_char, char next_char, char symb);

#endif  // SRC_CAT_S21_CAT_H_
