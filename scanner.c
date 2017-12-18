//Nick Boltralik, 2017

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "pretty.h"
#include "environment.h"
#include "types.h"
#include "evaluator.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr,"Wrong number of arguments.  Provide one .spqr file\n");
    return 1;
  }
  FILE *fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "File not found\n");
    return 1;
  }

  // printf("Beginning parsing\n");
  Lexeme *tree = parse(fp);
  // printf("Parsing completed!\n");

  // displayTree(tree, "");

  // prettyPrint(tree);

  Environment *global = createEnv();

  Lexeme *print = lexeme(BUILTIN);
  print->function = evalPrint;
  print->string = "scribo";
  Lexeme *var = lexeme(VARIABLE);
  var->string = "scribo";
  insertVal(var, print, global);
  // displayEnv(global);
  // Lexeme *result =
  eval(tree, global);

  // displayLexeme(result);

  return 0;
}
