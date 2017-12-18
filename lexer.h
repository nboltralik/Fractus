//Nick Boltralik, 2017

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef struct parser Parser;
typedef struct environment Environment;

typedef struct lexeme {
  char *type;
  char *string;
  int integer;
  int *array;
  //Function pointer field for a function an evaluation function
  struct lexeme* (*function)(struct lexeme*, Environment*);
  //I didn't make environments a Lexeme and this is what happened
  //TODO - Fix
  void *env;


  struct lexeme *left;
  struct lexeme *right;

} Lexeme;

Lexeme* lex(Parser*);
Lexeme* lexeme(char*);
char *displayLexeme(Lexeme*);

//TODO - do I ever use these outside lexer.c
Lexeme* newNode(char*, Lexeme*, Lexeme*);
Lexeme* getLeft(Lexeme*);
Lexeme* getRight(Lexeme*);
void setLeft(Lexeme*, Lexeme*);
void selRight(Lexeme*, Lexeme*);

#endif
