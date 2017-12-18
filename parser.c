//Nick Boltralik, 2017

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "pretty.h"

//Recognizing functions
int check(Parser *, char *);
void advance(Parser *);
Lexeme *match(Parser *, char *);
void matchNoAdvance(Parser *, char *);
char *type(Lexeme *);

//Pending Functions
int lambdaPending(Parser*);
int statementListPending(Parser*);
int functionCallPending(Parser*);
int statementPending(Parser*);
int expressionPending(Parser*);
int assignmentPending(Parser*);
int operatorPending(Parser*);
int unaryPending(Parser*);
int optExpressionListPending(Parser*);
int variableDefPending(Parser*);  //TODO - change this, do a check(p, "DEFINE"
int ifStatementPending(Parser*);
int blockPending(Parser*);
int optElseIfPending(Parser*);
int optElsePending(Parser*);
int whileLoopPending(Parser*);

//Lexeme creation functions
Lexeme* lambda(Parser*);
Lexeme* statementList(Parser*);
Lexeme* functionCall(Parser*);
Lexeme* statement(Parser*);
Lexeme* expression(Parser*);
Lexeme* assignment(Parser*);
Lexeme* operator(Parser*);
Lexeme* unary(Parser*);
Lexeme* optExpressionList(Parser*);
Lexeme* variableDef(Parser*);
Lexeme* ifStatement(Parser*);
Lexeme* block(Parser*);
Lexeme* optElseIf(Parser*);
Lexeme* optElse(Parser*);
Lexeme* whileLoop(Parser*);

Lexeme *parse(FILE *fp) {
  Parser *p = malloc(sizeof(Parser));

  p->fp = fp;
  p->pending = lex(p);
  p->line = 1;
  p->out = stdout;
  Lexeme *tree = NULL;

  if(statementListPending(p)) {
    //fprintf(stderr, "found a statement\n");
    tree = statementList(p);
  } else {
    //fprintf(stderr, "%s\n", "no statementListPending");
  }

  return tree;
}

int check(Parser *p, char *t) {
    // fprintf(stderr, "type passed to check: %s\n", t);
    // fprintf(stderr, "p->pending->type: %s\n", p->pending->type);
    // fprintf(stderr, "%d\n", strcmp(type(p->pending), t));
    return type(p->pending) == t;
  }

  void advance(Parser *p) {
    p->previous = p->pending;
    p->pending = lex(p);
  }

  Lexeme *match(Parser *p, char *t) {
    Lexeme *current = p->pending;
    matchNoAdvance(p,t);
    advance(p);
    return current;
  }

  void matchNoAdvance(Parser *p, char *t) {
    if(!check(p,t)) {
        fprintf(stderr,"Expected type %s, found %s at line %d\n", t, displayLexeme(p->pending), p->line);
        // fprintf(stderr,"Previous type %s\n",displayLexeme(*(p->previous)));
        exit(1);
    }
    // printf("matched %s with %s\n", p->pending->type, t);
  }

  char *type(Lexeme *l) {
    return l->type;
  }

  int typeEqual(Lexeme *a, Lexeme *b) {
    return strcmp(a->type,b->type);
}

void displayTree(Lexeme *tree, char *space) {

  // Base case
  if (tree == NULL)
      return;

  // Increase distance between levels
  char *newSpace = malloc(sizeof(char) * 1024);
  strcat(newSpace, space);
  strcat(newSpace, "    |");

  // Process right child first
  displayTree(tree->right, newSpace);

  // Print current node after space
  printf("%s%s\n", space, displayLexeme(tree));

  // Process left child
  displayTree(tree->left, newSpace);
}

//Pending Functions definitions
//Funky indents so my editor can hide them
int lambdaPending(Parser *p) {
    return check(p, LAMBDA);
  }

  //Can be *empty*
  int statementListPending(Parser *p) {
    return statementPending(p);
  }

  int functionCallPending(Parser *p) {
    return check(p, RUN);
  }

  int statementPending(Parser *p) {
    return functionCallPending(p)
      || expressionPending(p)
      || ifStatementPending(p)
      || whileLoopPending(p)
      || lambdaPending(p)
      || variableDefPending(p)
      || assignmentPending(p);
  }

  int expressionPending(Parser *p) {
    return unaryPending(p)
      || operatorPending(p)
      || functionCallPending(p);
  }

  int assignmentPending(Parser *p) {
    return check(p, VARIABLE);
  }

  int operatorPending(Parser *p) {
    return check(p, PLUS)
    || check(p, MINUS)
    || check(p, MULTIPLY)
    || check(p, DIVIDE)
    || check(p, EXPONENTIATE)
    || check(p, MODULO)
    || check(p, LT)
    || check(p, GT)
    || check(p, LT_EQUAL)
    || check(p, GT_EQUAL)
    || check(p, EQUAL)
    || check(p, N_EQUAL)
    || check(p, AND)
    || check(p, OR)
    || check(p, NOT);
  }

  int unaryPending(Parser *p) {
    return check(p, INTEGER)
    || check(p, VARIABLE)
    || check(p, OPEN_PAREN)
    || check(p, STRING);
  }

  int optExpressionListPending(Parser *p) {
    return expressionPending(p);
  }

  int stringPending(Parser *p) {
    return check(p, QUOTE);
  }

  //Can be *empty*
  int expressionListPending(Parser *p) {
    return expressionPending(p);
  }

  int variableDefPending(Parser *p) {
    return check(p, DEFINE);
  }

  int ifStatementPending(Parser *p) {
    return check(p, IF);
  }

  int blockPending(Parser *p) {
    return statementPending(p) || check(p, OPEN_CURL);
  }

  //Can be *empty*
  int optElseIfPending(Parser *p) {
    return check(p, ELSEIF);
  }

  //Can be *empty*
  int optElsePending(Parser *p) {
    return check(p, ELSE);
  }

  int whileLoopPending(Parser *p) {
    return check(p, WHILE);
}


//Lexeme creation functions definitions
Lexeme* lambda(Parser *p) {
  // fprintf(stderr, "lambda called\n");

  Lexeme *lex, *opt = NULL, *blk;
  match(p, LAMBDA);
  match(p, OPEN_PAREN);
  if (optExpressionListPending(p)) {
    opt = optExpressionList(p);
  }
  match(p, CLOSE_PAREN);
  blk = block(p);
  lex = newNode(LAMBDA, opt, blk);
  return lex;
}

Lexeme* statementList(Parser *p) {
  Lexeme *a, *b = NULL;
  a = statement(p);
  if (statementListPending(p)) {
    b = statementList(p);
  }
  // assert(a != NULL);
  // assert(b != NULL);
  // displayLexeme(*a);
  // displayLexeme(*b);
  return newNode(STATEMENTLIST, a, b);
}

Lexeme* functionCall(Parser *p) {
  //fprintf(stderr, "functionCall called\n");
  Lexeme *lex = NULL, *var, *expr = NULL;
  match(p, RUN);

  if (check(p, VARIABLE)) {
    var = match(p, VARIABLE);
    match(p, OPEN_PAREN);
    if (optExpressionListPending(p)) {
      expr = optExpressionList(p);
    }
    match(p, CLOSE_PAREN);
    lex = newNode(FUNCTIONCALL, var, expr);
  } else if (lambdaPending(p)) {
    var = lambda(p);
    match(p, OPEN_PAREN);
    if (optExpressionListPending(p)) {
      expr = optExpressionList(p);
    }
    match(p, CLOSE_PAREN);
    lex = newNode(FUNCTIONCALL, var, expr);
  } else {
    fprintf(stderr, "Expected type VARIABLE or LAMBDA, found %s at line %d\n", displayLexeme(p->pending), p->line);
    exit(1);
  }

  return lex;
}

Lexeme* statement(Parser *p) {
  // fprintf(stderr, "statement called\n");

  Lexeme *lex = NULL;
  if (lambdaPending(p)) {
    lex = lambda(p);
  } else if (assignmentPending(p)) {
    lex = assignment(p);
  } else if (ifStatementPending(p)) {
    lex = ifStatement(p);
  } else if (whileLoopPending(p)) {
    lex = whileLoop(p);
  } else if (functionCallPending(p)) {
    lex = functionCall(p);
    match(p, SEMICOLON);
  } else if (variableDefPending(p)) {
    lex = variableDef(p);
  } else if (expressionPending(p)) {
    lex = expression(p);
    match(p, SEMICOLON);
  } else {
    fprintf(stderr, "%s\n", "Statement could not find a correct pending function");
  }
  return lex;
}

Lexeme* expression(Parser *p) {
  // fprintf(stderr, "expression called\n");

  Lexeme *lex = NULL;
  if (unaryPending(p)) {
    lex = unary(p);
  } else if (operatorPending(p)) {
    Lexeme *op = operator(p);
    if (!expressionPending(p)) {
      fprintf(stderr, "%s\n", "Expression could not find a pending expression");
    }
    Lexeme *expr1 = expression(p);
    if (!expressionPending(p)) {
      fprintf(stderr, "%s\n", "Expression could not find a pending expression");
    }
    Lexeme *expr2 = expression(p);

    Lexeme *glue = newNode(GLUE, expr1, expr2);
    lex = newNode(EXPRESSION, op, glue);
  } else if (functionCallPending(p)) {
    lex = functionCall(p);
  } else {
    fprintf(stderr, "%s\n", "Expression could not find a correct pending function");
  }
  return lex;
}

Lexeme* assignment(Parser *p) {
  // fprintf(stderr, "assignment called\n");

  Lexeme *var, *val, *lex = NULL;
  var = match(p, VARIABLE);
  if (check(p, ASSIGN)) {
    match(p, ASSIGN);
    val = expression(p);
    match(p, SEMICOLON);
    lex = newNode(ASSIGN_NARRAY, var, val);
  } else if (check(p, OPEN_SQUARE)) {
    Lexeme *index, *tmp;
    match(p, OPEN_SQUARE);
    index = expression(p);
    match(p, CLOSE_SQUARE);
    match(p, ASSIGN);
    val = expression(p);
    match(p, SEMICOLON);
    tmp = newNode(GLUE, index, val);
    lex = newNode(ASSIGN_ARRAY, var, tmp);
  }

  return lex;
}

Lexeme* operator(Parser *p) {
  return match(p, p->pending->type);
}

Lexeme* unary(Parser *p) {
  //fprintf(stderr, "unary called\n");

  Lexeme *lex = NULL;

  if (check(p, INTEGER)) {
    lex = match(p, INTEGER);
  } else if (check(p, VARIABLE)) {

    lex = match(p, VARIABLE);
    if (check(p, OPEN_SQUARE)) {
      match(p, OPEN_SQUARE);
      Lexeme *index = expression(p);
      match(p, CLOSE_SQUARE);
      return newNode(ARRAY_INDEX, lex, index);
    }

  } else if (check(p, OPEN_PAREN)) {
    match(p, OPEN_PAREN);
    if(!expressionPending(p)) {
      fprintf(stderr, "%s\n", "Expression could not find a pending expression");
    }
    lex = expression(p);
    match(p, CLOSE_PAREN);

  // } /else if (check(p, MINUS)) {
  //
  //   match(p, MINUS);
  //   //TODO
  //   lex = match(p, INTEGER);

  } else if (check(p, STRING)) {
      lex = match(p, STRING);
  } else {
    fprintf(stderr, "%s\n", "unary (parser.c:314) could not find a correct terminal");
  }
  return lex;
}

Lexeme* optExpressionList(Parser *p) {
  //fprintf(stderr, "optExpressionList called\n");

  Lexeme *lex = NULL, *tmp = NULL;
  lex = expression(p);
  if (check(p, COMMA)) {
    match(p, COMMA);
    if (optExpressionListPending(p)) {
      tmp = optExpressionList(p);
    }
  }
  return newNode(OPTEXPRESSIONLIST, lex, tmp);
}

Lexeme* variableDef(Parser *p) {
  //fprintf(stderr, "variableDef called\n");

  Lexeme *lex = NULL, *var;
  match(p, DEFINE);
  var = match(p, VARIABLE);

  if (check(p, ASSIGN)) {
    match(p, ASSIGN);
    if (check(p, STRING)) {
      Lexeme *str = match(p, STRING);
      match(p, SEMICOLON);
      lex = newNode(STRINGDEF, var, str);
    } else if (expressionPending(p)) {
      Lexeme *value = expression(p);
      match(p, SEMICOLON);
      lex = newNode(INTEGERDEF, var, value);
    } else {
      fprintf(stderr,"Expected type EXPRESSION, found %s at line %d\n" ,displayLexeme(p->pending), p->line);
    }
  } else if (check(p, LAMBDA)) {
    Lexeme *lmb = lambda(p);
    lex = newNode(FUNCTIONDEF, var, lmb);
  } else if (check(p, OPEN_SQUARE)) {
    match(p, OPEN_SQUARE);
    Lexeme *size = unary(p);
    match(p, CLOSE_SQUARE);
    match(p, SEMICOLON);
    lex = newNode(ARRAYDEF, var, size);
  } else if (check(p, SEMICOLON)) {
    match(p, SEMICOLON);
    lex = newNode(NULLDEF, var, NULL);
  } else {
    fprintf(stderr, "%s\n", "variableDef could not find a correct terminal");
  }
  return lex;
}

Lexeme* ifStatement(Parser *p) {
  //fprintf(stderr, "ifStatement called\n");

  Lexeme *expr, *blk, *elsif = NULL, *els = NULL;
  match(p, IF);
  match(p, OPEN_PAREN);
  expr = expression(p);

  match(p, CLOSE_PAREN);
  blk = block(p);
  if (optElseIfPending(p)) {
    elsif = optElseIf(p);
  }
  if (optElsePending(p)) {
    els = optElse(p);
  }
  Lexeme *tmp1 = newNode(GLUE, expr, blk);
  Lexeme *tmp2 = newNode(GLUE, elsif, els);
  return newNode(IFSTATEMENT, tmp1, tmp2);
}

Lexeme* block(Parser *p) {
  //fprintf(stderr, "block called\n");

  Lexeme *lex = NULL;
  if (statementPending(p)) {
    lex = statement(p);
  } else if (check(p, OPEN_CURL)) {
    match(p, OPEN_CURL);
    lex = statementList(p);
    match(p, CLOSE_CURL);
  }
  return lex;
}

Lexeme* optElseIf(Parser *p) {
  //fprintf(stderr, "optElseIf called\n");

  Lexeme *lex = NULL, *expr, *blk;
  if (check(p, ELSEIF)) {
    match(p, ELSEIF);
    match(p, OPEN_PAREN);
    expr = expression(p);
    match(p, CLOSE_PAREN);
    blk = block(p);
    Lexeme *tmp = newNode(GLUE, expr, blk);
    Lexeme *tmp2 = optElseIf(p);
    lex = newNode(ELSEIFSTATEMENT, tmp, tmp2);
  }
  return lex;
}

Lexeme* optElse(Parser *p) {
  //fprintf(stderr, "optElse called\n");

  Lexeme *lex = NULL;
  if (check(p, ELSE)) {
    match(p, ELSE);
    lex = block(p);
  }
  return lex;
}

Lexeme* whileLoop(Parser *p) {
  //fprintf(stderr, "whileLoop called\n");

  match(p, WHILE);
  Lexeme *lex = NULL, *expr, *blk;
  match(p, OPEN_PAREN);
  expr = expression(p);
  match(p, CLOSE_PAREN);
  blk = block(p);
  lex = newNode(WHILELOOP, expr, blk);
  return lex;
}
