#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    s21_grep(argc, argv);
  } else {
    printf("Not enough arguments\n");
  }
  return 0;
}

void s21_grep(int argc, char **argv) {
  char patterns[4096] = {'\0'};
  char filenames[4096] = {'\0'};
  optionBash o;

  initOptionBash(&o);

  int fileCounter = 0;
  fileCounter = parserFlag(argc, argv, &o, patterns, filenames);
  int isPrint = 0;
  isPrint = changeFlags(&o);

  regex_t regex;
  int cflags = 0;
  int reti = 0;
  char *filename = NULL;

  if (o.flagI) {
    cflags = REG_EXTENDED | REG_ICASE;
  } else
    cflags = REG_EXTENDED;

  reti = regcomp(&regex, patterns, cflags);
  filename = strtok(filenames, "|");

  while (filename != NULL) {
    FILE *fp;
    regmatch_t match[1] = {0};
    char cc[4097] = {'\0'};
    int count = 0;
    if ((fp = fopen(filename, "r")) != NULL) {
      for (size_t i = 0; fgets(cc, 4096, fp); i++) {
        if (strchr(cc, '\n') == NULL) strcat(cc, "\n");
        if (o.flagO)
          reti = regexec(&regex, cc, 1, match, 0);
        else
          reti = regexec(&regex, cc, 0, NULL, 0);
        if (!reti) {
          if ((o.flagE || o.flagI == 1 || o.flagN || o.flagF || o.flagO) &&
              o.flagV == 0 && isPrint == 0) {
            printStr(fileCounter, filename, &o, i);
            if (o.flagO) {
              char *pstr = cc;
              int error = 0;
              while (!error) {
                if (match[0].rm_eo == match[0].rm_so) break;
                printf("%.*s\n", (int)(match[0].rm_eo - match[0].rm_so),
                       pstr + match[0].rm_so);
                pstr += match[0].rm_eo;
                error = regexec(&regex, pstr, 1, match, REG_NOTBOL);
              }
            } else {
              fputs(cc, stdout);
            }
          }
          if (isPrint == 1) count++;
        } else if (reti == REG_NOMATCH) {
          if (o.flagV) {
            printStr(fileCounter, filename, &o, i);
            fputs(cc, stdout);
          }
          if (isPrint == -1) count++;
        }
        memset(cc, '\0', 4096);
      }
      fclose(fp);
      if (o.flagC) {
        if (fileCounter > 1 && !o.flagH) printFile(filename);
        if (o.flagL && count > 0) count = 1;
        printf("%d\n", count);
      }
      if ((o.flagL) && count > 0) printf("%s\n", filename);
    }
    filename = strtok(NULL, "|");
  }
  regfree(&regex);
}

void printStr(int fileCounter, char *filename, optionBash *o, int i) {
  if (fileCounter > 1 && !o->flagH) printFile(filename);
  if (o->flagN) flagN(i);
}

void flagN(int i) {
  char css[10];
  sprintf(css, "%d:", i + 1);
  fputs(css, stdout);
}

void printFile(char *filename) {
  fputs(filename, stdout);
  fputc(':', stdout);
}

int changeFlags(optionBash *o) {
  int isPrint = 0;
  if (o->flagV && o->flagI) o->flagI = 2;
  if (o->flagV && o->flagE) o->flagE = 0;
  if (o->flagV && o->flagO) o->flagO = 0;
  if (o->flagC || o->flagL) {
    if (o->flagV) {
      isPrint = -1;
      o->flagV = 0;
    } else {
      isPrint = 1;
      o->flagN = 0;
      if (o->flagI) o->flagI = 2;
      o->flagE = 0;
    }
  }
  if (o->flagO == 2) {
    o->flagO = 0;
    o->flagE = 1;
  }
  if (o->flagC + o->flagE + o->flagF + o->flagI + o->flagL + o->flagN +
          o->flagO + o->flagV ==
      0)
    o->flagE = 1;
  if (o->flagE && o->flagO) o->flagE = 0;
  if (o->flagI + o->flagH + o->flagS + o->flagN == 1 && o->flagO) {
    o->flagO = 1;
    o->flagE = 0;
  }

  return isPrint;
}

int parserFlag(int argc, char **argv, optionBash *o, char *pat,
               char *filenames) {
  int i = 1;
  int e_counter = 0;
  int file_counter = 0, combinationStatus = 0;
  while (i < argc) {
    combinationStatus = combinationOrFlag(argv[i]);
    if (combinationStatus) {
      char *arg = argv[i];
      arg++;
      while (*arg != 0) {
        if (*arg == 'e' && combinationStatus) {
          o->flagE = 1;
          if (e_counter > 0) strcat(pat, "|");
          if (!argv[i + 1]) exit(1);
          i++;
          if (!strcmp(argv[i], "\0")) argv[i] = "\n";
          strcat(pat, argv[i]);
          e_counter++;
        } else if (*arg == 'l' && combinationStatus) {
          o->flagL = 1;
        } else if (*arg == 'n' && combinationStatus) {
          o->flagN = 1;
        } else if (*arg == 'c' && combinationStatus) {
          o->flagC = 1;
        } else if (*arg == 'i' && combinationStatus) {
          o->flagI = 1;
        } else if (*arg == 'v' && combinationStatus) {
          o->flagV = 1;
        } else if (*arg == 'f' && combinationStatus) {
          o->flagF = 1;
          char regfile[1024];
          char *file = regfile;
          file = argv[i + 1];
          FILE *f;
          char str[4097] = {'\0'};
          char *cc = str;
          if ((f = fopen(file, "r")) != NULL) {
            for (size_t j = 0; fgets(cc, 4096, f); j++) {
              if (e_counter > 0) strcat(pat, "|");
              if (strchr(cc, '\n')) *(cc + strlen(cc) - 1) = '\0';
              if (!strcmp(cc, "\0")) cc = "\n";
              strcat(pat, cc);
              e_counter++;
              memset(cc, '\0', 4096);
            }
            fclose(f);
          } else /*if (!o->flagS)*/ {
            messegeError(file);
            exit(1);
          }
          i++;
        } else if (*arg == 'h' && combinationStatus) {
          o->flagH = 1;
        } else if (*arg == 's' && combinationStatus) {
          o->flagS = 1;
        } else if (*arg == 'o' && combinationStatus) {
          o->flagO++;
        }
        arg++;
      }
    } else if (e_counter == 0 && !findEFlag(i, argc, argv) &&
               !findFFlag(i, argc, argv)) {
      if (!strcmp(argv[i], "\0")) argv[i] = "\n";
      strcat(pat, argv[i]);
      e_counter++;
    } else {
      FILE *f;
      if ((f = fopen(argv[i], "r")) != NULL) {
        fclose(f);
        if (file_counter > 0) strcat(filenames, "|");
        strcat(filenames, argv[i]);
        file_counter++;
      } else {
        if (!o->flagS) messegeError(argv[i]);
        file_counter++;
      }
    }
    i++;
  }
  return file_counter;
}

int combinationOrFlag(char *str) {
  int check = 0, len = strlen(str);
  if (*str == '-' && (len == 2 || len == 3)) {
    str++;
    len--;
    while (*str != 0) {
      switch (*str) {
        case 'e':
        case 'f':
        case 'n':
        case 'c':
        case 'l':
        case 'i':
        case 'h':
        case 's':
        case 'o':
        case 'v':
          check++;
          break;
      }
      str++;
    }
    if ((check == 2 && len == 2) || (check == 1 && len == 1))
      check = 1;
    else
      check = 0;
  }
  return check;
}

void messegeError(char *filename) {
  fprintf(stderr, "grep: %s: No such file or directory\n", filename);
}

int findEFlag(int i, int argc, char **argv) {
  int countE = 0;
  while (++i < argc) {
    if (combinationOrFlag(argv[i])) {
      char *str = argv[i];
      while (*str != 0) {
        if (*str == 'e') countE++;
        str++;
      }
    }
  }
  return countE;
}

int findFFlag(int i, int argc, char **argv) {
  int countF = 0;
  while (++i < argc) {
    if (combinationOrFlag(argv[i])) {
      char *str = argv[i];
      while (*str != 0) {
        if (*str == 'f') countF++;
        str++;
      }
    }
  }
  return countF;
}

void initOptionBash(optionBash *buff) {
  buff->flagE = 0;
  // buff->flagI = 0;
  buff->flagV = 0;
  buff->flagC = 0;
  buff->flagL = 0;
  buff->flagN = 0;
  buff->flagF = 0;
  buff->flagO = 0;
  buff->flagS = 0;
  buff->flagH = 0;
}
