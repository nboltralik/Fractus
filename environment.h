//Nick Boltralik, 2017

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "lexer.h"

typedef struct list {
  Lexeme* lex;
  struct list *next;
} List;

typedef struct environment {
  struct environment *outer;
  List *varList;
  List *valList;
} Environment;

Lexeme *lookupVar(Lexeme*, Environment*) ;
Lexeme *updateVal(Lexeme*, Lexeme*, Environment*);
Lexeme *insertVal(Lexeme*, Lexeme*, Environment*);
Environment *extendEnv(List*, List*, Environment*);
Environment *createEnv();
void displayEnv(Environment*);
List *append(List*, Lexeme*);
List *makeList(Lexeme*);

#endif
