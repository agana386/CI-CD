#include "s21_cat.h"

int global_flags[all] = {0};

int main(int argc, char *argv[]) {
  const char *short_options = "nbetsvE";
  const struct option long_options[] = {
      {"number", no_argument, NULL, 'n'},
      {"number-nonblank", no_argument, NULL, 'b'},
      {"show-ends", no_argument, NULL, 'e'},
      {"show-ends", no_argument, NULL, 'E'},
      {"show-tabs", no_argument, NULL, 't'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {"v", no_argument, NULL, 'v'},
      {NULL, 0, NULL, 0}};

  int res;
  int option_index;

  while ((res = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (res) {
      case 'n':
        set_flag(number);
        break;

      case 'b':
        set_flag(number_nonblank);
        break;

      case 'e':
        set_flag(show_ends);
        set_flag(v);
        break;

      case 'E':
        set_flag(show_ends);
        break;

      case 't':
        set_flag(v);
        set_flag(show_tabs);
        break;

      case 's':
        set_flag(squeeze_blank);
        break;

      case 'v':
        set_flag(v);
        break;

      case '?':
      default:
        printf("Error found !\n");
        break;
    }
  }
  if (optind == argc) {
    output_file("-");
  }
  while (optind < argc) {
    output_file(argv[optind++]);
  }
  exit(0);
}

void output_file(const char *fn) {
  FILE *pf;
  char symb[MAX_ST] = {0};
  int begin_line_num = 1;
  int line_counter = 1;
  int empty_line_counter = 1;
  char previous_char = '\n';
  char next_char = ' ';

  if ((pf = fopen(fn, "r")) == NULL) {
    if (strcmp(fn, "-") == 0) {
      pf = stdin;
    } else if ((pf = fopen(fn, "r")) == NULL) {
      printf("cat: %s: No such file or directory\n", fn);
      exit(1);
    }
  }
  while ((*symb = fgetc(pf)) != EOF) {
    if (global_flags[squeeze_blank] &&
        s_falg_itter(previous_char, next_char, *symb))
      continue;
    if (global_flags[number] && !global_flags[number_nonblank]) {
      begin_line_num = n_flag_itter(*symb, begin_line_num, &line_counter);
    }
    if (global_flags[number_nonblank]) {
      begin_line_num = b_falg_itter(previous_char, &empty_line_counter, *symb);
    }
    if (global_flags[show_tabs]) {
      t_flag_itter(symb);
    }
    if (global_flags[show_ends]) {
      e_flag_itter(*symb);
    }
    if (global_flags[v]) {
      v_flag_itter(symb);
    }
    next_char = previous_char;
    previous_char = *symb;
    if (*symb == '\0') {
      fputc(*symb, stdout);
    } else {
      fputs(symb, stdout);
    }
    memset(symb, '\0', 1024);
  }
  if (ferror(pf)) {
    perror("cat");
  }
  // if (pf != stdin) {
  //     fclose(f);
  // }
  fclose(pf);
}

int t_flag_itter(char *symb) {
  unsigned char temp = *symb;
  if (*symb == '\t') {
    *symb++ = '^';
    *symb++ = 'I';
  }
  return temp;
}

int v_flag_itter(char *symb) {
  unsigned char temp = *symb;
  if ((temp <= 8) || (temp >= 11 && temp <= 31)) {
    // symb = asciiSymbols[(int)temp];
    *symb++ = '^';
    *symb++ = temp + 64;
  } else if (temp == 127) {
    *symb++ = '^';
    *symb++ = temp - 64;
  } else if (temp >= 128 && temp <= 159) {
    *symb++ = 'M';
    *symb++ = '-';
    *symb++ = '^';
    *symb++ = temp - 64;
    // symb = asciiSymbols[(int)temp];
  }
  return temp;
}

int e_flag_itter(char symb) {
  if (symb == '\n') {
    printf("$");
  }
  symb = '\n';
  return symb;
}

int n_flag_itter(char symb, int begin_line_num, int *line_counter) {
  if (begin_line_num) {
    printf("%6d\t", (*line_counter)++);
  }
  return symb == '\n';
}

int b_falg_itter(char previous_char, int *empty_line_counter, char symb) {
  if (previous_char == '\n' && symb != '\n') {
    printf("%6d\t", (*empty_line_counter)++);
  }
  return symb == '\n';
}

int s_falg_itter(char previous_char, char next_char, char symb) {
  return symb == '\n' && previous_char == '\n' && next_char == '\n';
}

void set_flag(enum global_flags_enum current_flag) {
  global_flags[current_flag] = 1;
}
