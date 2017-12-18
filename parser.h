//Nick Boltralik, 2017

#ifndef PARSER_H
#define PARSER_H
#include "lexer.h"

typedef struct parser {
  FILE *fp;
  struct lexeme *previous;
  struct lexeme *pending;
  FILE *out;
  int line;
} Parser;

struct lexeme *parse(FILE *fp);

void displayTree(struct lexeme *,char *);
int typeEqual(struct lexeme *,struct lexeme *);

#endif
