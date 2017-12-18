//Nick Boltralik, 2017

#include <stdio.h>
#include "types.h"
#include "lexer.h"
#include "environment.h"
#include "parser.h"
// #include <assert.h>

int sameVar(Lexeme*, Lexeme*);
List *append(List*, Lexeme*);

Lexeme *lookupVar(Lexeme *variable, Environment *env) {
  if(variable->type != VARIABLE) {
      fprintf(stderr, "Attempted to lookup a non variable, %s\n", displayLexeme(variable));
      return NULL;
    }

//UNCOMMENT HERE
  // fprintf(stderr, "Looking up %s\n", displayLexeme(variable));
//UNCOMMENT HERE

  if(env != NULL) {

    List *vars = env->varList;
    List *vals = env->valList;

    while(vars != NULL) {
      if(sameVar(variable, vars->lex)) {
        // assert(vals->lex->type != NULL);
        // fprintf(stderr, "Found %s\n", displayLexeme(vals->lex));
        return vals->lex;
      }
      vars = vars->next;
      vals = vals->next;
    }

    return lookupVar(variable, env->outer);
  }

    fprintf(stderr, "Variable %s is undefined\n", displayLexeme(variable));
    exit(1);

    return NULL;
}

Lexeme *updateVal(Lexeme *variable, Lexeme *value, Environment *env) {
//UNCOMMENT HERE
  // printf("Updating %s\n", displayLexeme(variable));
//UNCOMMENT HERE

  if(env != NULL) {
    // displayEnv(env);
    //TODO - should I use lookup here?
    List *vars = env->varList;
    List *valHead = env->valList;

    while(vars != NULL ) {
      if(sameVar(variable, vars->lex)) {
        env->valList->lex = value;
        env->valList = valHead;

//UNCOMMENT HERE
        // fprintf(stderr, "Environment is:\n");
        // displayEnv(env);
        // fprintf(stderr, "\n");
//UNCOMMENT HERE

        return value;
      }
      vars = vars->next;
      env->valList = env->valList->next;

    }

    return updateVal(variable, value, env->outer);
  }

  fprintf(stderr,"Variable %s is undefined\n", displayLexeme(variable));
  exit(1);

  return NULL;
}

Lexeme *insertVal(Lexeme *variable, Lexeme *value, Environment *env) {

//UNCOMMENT HERE
  // printf("Inserting %s\n", displayLexeme(variable));
//UNCOMMENT HERE

  env->varList = append(env->varList, variable);
  // printf("2%s\n", displayLexeme(env->varList->lex));
  env->valList = append(env->valList, value);

//UNCOMMENT HERE
  // fprintf(stderr, "Environment is:\n");
  // displayEnv(env);
  // fprintf(stderr, "\n");
//UNCOMMENT HERE

  return value;
}

Environment *extendEnv(List *vars, List *vals, Environment *env) {

  Environment *newEnv = malloc(sizeof(Environment));
  newEnv->outer = env;

  newEnv->varList = vars;
  newEnv->valList = vals;

  return newEnv;
}

Environment *createEnv() {
 return extendEnv(NULL, NULL, NULL);
}

void displayEnv(Environment *env) {
  List *vars = env->varList;
  List *vals = env->valList;

  printf("Lists:\n");
  while(vars != NULL && vars->lex != NULL) {
    printf("Var: %s\t\n", displayLexeme(vars->lex));
    printf("Val: %s\n\n", displayLexeme(vals->lex));
    vars = vars->next;
    vals = vals->next;
  }

  if (env->outer != NULL) {
    printf("Enclosing Environment:\n");
    displayEnv(env->outer);
  }
}

int sameVar(Lexeme *a, Lexeme *b) {

  // fprintf(stderr, "a is %s\n", displayLexeme(a));
  // fprintf(stderr, "b is %s\n", displayLexeme(b));
 return (strcmp(a->string, b->string) == 0);
}

List *append(List *vars, Lexeme *l) {
  List *tmp = malloc(sizeof(List));
  tmp->lex = l;
  tmp->next = vars;
  return tmp;
}

List *makeList(Lexeme *lex) {
  List *tmp = malloc(sizeof(List));

  if (lex != NULL) {
    tmp->lex = getLeft(lex);
    tmp->next = makeList(getRight(lex));
  } else {
    tmp = NULL;
  }

  return tmp;
}
