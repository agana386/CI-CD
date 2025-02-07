#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct patternGrep {
  char patternname[256];
  int val;
  struct patternGrep *next;
} patternGrep;

typedef struct optionBash {
  int flagE;
  int flagI;
  int flagV;
  int flagC;
  int flagL;
  int flagN;
  int flagF;
  int flagH;
  int flagS;
  int flagO;
} optionBash;

void s21_grep(int argc, char **argv);
void initOptionBash(optionBash *o);
int changeFlags(optionBash *o);
void messegeError(char *filename);
void *s21_to_lower(const char *str);
int parserFlag(int argc, char **argv, optionBash *o, char *pat,
               char *filenames);
void printStr(int fileCounter, char *filename, optionBash *o, int i);
void printFlags(optionBash *o);
void printFile(char *filename);
void flagN(int i);
void flagF(char **argv, int i, char *pat, optionBash *o, int *e_counter);
int findEFlag(int i, int argc, char **argv);
int findFFlag(int i, int argc, char **argv);
int combinationOrFlag(char *str);

#endif  // SRC_GREP_S21_GREP_H_
