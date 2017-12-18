//Nick Boltralik, 2017

#include <stdio.h>
// #include <stdlib.h
#include <math.h>
#include "lexer.h"
#include "parser.h"
#include "types.h"
#include "environment.h"
#include "evaluator.h"
#include "pretty.h"

// Lexeme *eval(Lexeme*, Environment*);
Lexeme *eval(Lexeme *tree, Environment *env);

//non-terminal definitions
Lexeme *evalStatementList(Lexeme*, Environment*);
Lexeme *evalFunctionCall(Lexeme*, Environment*);
Lexeme *evalASSIGN_ARRAY(Lexeme*, Environment*);
Lexeme *evalASSIGN_NARRAY(Lexeme*, Environment*);
Lexeme *evalExpression(Lexeme*, Environment*);
Lexeme *evalIfStatement(Lexeme*, Environment*);
Lexeme *evalElseIf(Lexeme*, Environment*);
Lexeme *evalElse(Lexeme*, Environment*);
Lexeme *evalWhileLoop(Lexeme*, Environment*);
Lexeme *evalLambda(Lexeme*, Environment*);
Lexeme *evalFunctionDef(Lexeme*, Environment*);
Lexeme *evalVariableDef(Lexeme*, Environment*);
Lexeme *evalArrayDef(Lexeme*, Environment*);
Lexeme *evalArrayIndex(Lexeme*, Environment*);
Lexeme *evalOptExpressionList(Lexeme*, Environment*);
Lexeme *evalOPERATOR(Lexeme*, Environment*);

Lexeme *eval(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "Evaluating %s\n", displayLexeme(tree));
  // fprintf(stderr, "Environment is:\n");
  // displayEnv(env);
  // fprintf(stderr, "\n");
  if (tree == NULL) {
    return NULL;
  } else if (strcmp(tree->type, STATEMENTLIST) == 0) {
    return evalStatementList(tree, env);
  } else if (strcmp(tree->type, FUNCTIONCALL) == 0) {
    return evalFunctionCall(tree, env);
  } else if (strcmp(tree->type, ASSIGN_ARRAY) == 0) {
    return evalASSIGN_ARRAY(tree, env);
  } else if (strcmp(tree->type, ASSIGN_NARRAY) == 0) {
    return evalASSIGN_NARRAY(tree, env);
  } else if (strcmp(tree->type, EXPRESSION) == 0) {
    return evalExpression(tree, env);
  } else if (strcmp(tree->type, IFSTATEMENT) == 0) {
    return evalIfStatement(tree, env);
  } else if (strcmp(tree->type, ELSEIF) == 0) {
    return evalElseIf(tree, env);
  } else if (strcmp(tree->type, ELSE) == 0) {
    return evalElse(tree, env);
  } else if (strcmp(tree->type, WHILELOOP) == 0) {
    return evalWhileLoop(tree, env);
  } else if (strcmp(tree->type, LAMBDA) == 0) {
    return evalLambda(tree, env);
  } else if (strcmp(tree->type, INTEGERDEF) == 0
          || strcmp(tree->type, STRINGDEF) == 0
          || strcmp(tree->type, NULLDEF) == 0) {
    return evalVariableDef(tree, env);
  } else if (strcmp(tree->type, FUNCTIONDEF) == 0) {
    return evalFunctionDef(tree, env);
  } else if (strcmp(tree->type, ARRAYDEF) == 0) {
    return evalArrayDef(tree, env);
  } else if (strcmp(tree->type, ARRAY_INDEX) == 0) {
    return evalArrayIndex(tree, env);
  } else if (strcmp(tree->type, OPTEXPRESSIONLIST) == 0) {
    return evalOptExpressionList(tree, env);

    //TODO - change this when type == OPERATOR
  } else if (strcmp(tree->type, PLUS) == 0 || strcmp(tree->type, MINUS) == 0
    || strcmp(tree->type, MULTIPLY) == 0 || strcmp(tree->type, DIVIDE) == 0
    || strcmp(tree->type, EXPONENTIATE) == 0 || strcmp(tree->type, MODULO) == 0
    || strcmp(tree->type, LT) == 0 || strcmp(tree->type, GT) == 0
    || strcmp(tree->type, LT_EQUAL) == 0 || strcmp(tree->type, GT_EQUAL) == 0
    || strcmp(tree->type, EQUAL) == 0 || strcmp(tree->type, N_EQUAL) == 0
    || strcmp(tree->type, AND) == 0 || strcmp(tree->type, OR) == 0
    || strcmp(tree->type, NOT) == 0) {
    return evalOPERATOR(tree, env);
  } else if (strcmp(tree->type, INTEGER) == 0 || strcmp(tree->type, STRING) == 0) {
    return tree;
  } else if (strcmp(tree->type, VARIABLE) == 0) {
    return lookupVar(tree, env);
  }
  // } else if (strcmp(tree->type, ) == 0) {
  //   return eval(tree, env);
  // } else if (strcmp(tree->type, ) == 0) {
  //   return eval(tree, env);
  // } else if (strcmp(tree->type, ) == 0) {
  //   return eval(tree, env);
  // } else if (strcmp(tree->type, ) == 0) {
  //   return eval(tree, env);
  // } else if (strcmp(tree->type, ) == 0) {
  //   return eval(tree, env);
  // }

  fprintf(stderr, "No suitable type found for %s\n", tree->type);
  return NULL;
}

Lexeme *evalStatementList(Lexeme *tree, Environment *env) {
  Lexeme *result = eval(getLeft(tree), env);
  if (getRight(tree) != NULL) {
    result = eval(getRight(tree), env);
  }
  return result;
}

Lexeme *evalFunctionCall(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "Function call\n");
  //TODO - clean up this mess; seriously

  Lexeme *eArgs = NULL;
  //FunctionCall with a bound variable
  if (getLeft(tree)->type == VARIABLE) {
    // fprintf(stderr, "function is %s\n", displayLexeme(getLeft(tree)));

    Lexeme *closure = lookupVar(getLeft(tree), env);
    Lexeme *args = getRight(tree);
    if (args != NULL) {
      Lexeme tmp = *args;     //Avoiding eval-ing actual args by using pointers
      eArgs = evalOptExpressionList(&tmp, env);   //Theres a better way to do this I just dont have time
    } else {
      eArgs = evalOptExpressionList(args, env);
    }
    // fprintf(stderr, "Args: %s\n", displayLexeme(args));
    // fprintf(stderr, "eArgs: %s\n", displayLexeme(eArgs));

    if (closure->type == BUILTIN) {
      return closure->function(eArgs, env);
    } else {

      Lexeme *params = getLeft(closure);
      // fprintf(stderr, "Params: %s\n", displayLexeme(params));
      Lexeme *body = getRight(closure);
      Environment *enclEnv = (Environment*) closure->env;

      //Need to append the function definition itself to support recursion
      List *vars = makeList(params);
      // vars = append(vars, getLeft(tree));
      List *vals = makeList(eArgs);
      // vals = append(vals, closure);

      Environment *xEnv = extendEnv(vars, vals, enclEnv);
      // fprintf(stderr, "Extended env is:\n");
      // displayEnv(xEnv);
      return eval(body, xEnv);
    }

  //Calling a LAMBDA after its definition
  } else {
    Lexeme *closure = getLeft(tree);
    closure->type = CLOSURE;
    closure->env = env;

    Lexeme *args = getRight(tree);
    Lexeme tmp = *args;     //Avoiding eval-ing args with pointers
    // fprintf(stderr, "Args: %s\n", displayLexeme(args));
    Lexeme *params = getLeft(closure);
    // fprintf(stderr, "Params: %s\n", displayLexeme(params));
    Lexeme *body = getRight(closure);
    Environment *enclEnv = (Environment*) closure->env;
    Lexeme *eArgs = evalOptExpressionList(&tmp, env);   //Theres a better way to do this I just dont have time
    // fprintf(stderr, "eArgs: %s\n", displayLexeme(eArgs));
    List *vars = makeList(params);
    List *vals = makeList(eArgs);


    Environment *xEnv = extendEnv(vars, vals, enclEnv);
    return eval(body, xEnv);
  }
}

Lexeme *evalASSIGN_ARRAY(Lexeme *tree, Environment *env) {
  Lexeme *index = eval(getLeft(getRight(tree)), env);
  Lexeme *value = eval(getRight(getRight(tree)), env);
  Lexeme *arr = lookupVar(getLeft(tree), env);
  // fprintf(stderr, "%s\n", displayLexeme(index));

  if (index->integer >= arr->integer) {
    fprintf(stderr, "Index out of bounds. Size of array is %d index is %d\n", arr->integer, index->integer);
    exit(1);
  }

  arr->array[index->integer] = value->integer;
  // fprintf(stderr, "value->integer is %d\nindex->integer is %d\narr->array[index->integer] is %d\n", value->integer, index->integer, arr->array[index->integer]);

  return updateVal(getLeft(tree), arr, env);  //TODO - how to find the correct array index to update
}

Lexeme *evalASSIGN_NARRAY(Lexeme *tree, Environment *env) {
  Lexeme *value = eval(getRight(tree), env);
  // fprintf(stderr, "Updating %s with value %d\n", displayLexeme(getLeft(tree)), value->integer);
  return updateVal(getLeft(tree), value, env);
}

Lexeme *evalExpression(Lexeme *tree, Environment *env) {
  return evalOPERATOR(tree, env); //TODO - how does this work for boolean operators
}

Lexeme *evalIfStatement(Lexeme *tree, Environment *env) {

  Lexeme *cond = eval(getLeft(getLeft(tree)), env);
  Lexeme *result = NULL;
  if (cond->integer) {
    // fprintf(stderr, "If condition evaluated to true\n");
    return eval(getRight(getLeft(tree)), env);
  } else if (getLeft(getRight(tree)) != NULL) {
    // fprintf(stderr, "Else if condition found\n");
    result = evalElseIf(getLeft(getRight(tree)), env);
  }

  // fprintf(stderr, "\"%s\"\n", displayLexeme(result));
  if (result == NULL && getRight(getRight(tree)) != NULL) {
    // fprintf(stderr, "Else condition found\n");
    result = evalElse(getRight(getRight(tree)), env);
  } else {
    // fprintf(stderr, "No condition evaluated to true or else case not present\n");
    return NULL;
  }

  return result;
}

Lexeme *evalElseIf(Lexeme *tree, Environment *env) {
  Lexeme *cond = eval(getLeft(getLeft(tree)), env);
  if (cond->integer) {
    // fprintf(stderr, "Else If condition evaluated to true\n");
    return eval(getRight(getLeft(tree)), env);
  } else if (getRight(tree) != NULL) {
    return evalElseIf(getRight(tree), env);
  } else {
    // fprintf(stderr, "Else if condition is false or another else if case not present\n");
    return NULL;
  }
}

Lexeme *evalElse(Lexeme *tree, Environment *env) {
  return eval(tree, env);
}

Lexeme *evalWhileLoop(Lexeme *tree, Environment *env) {
  Lexeme *cond = eval(getLeft(tree), env);
  Lexeme *result = NULL;

  while (cond->integer) {
    result = eval(getRight(tree), env);
    cond = eval(getLeft(tree), env);
  }
  return result;
}

Lexeme *evalFunctionDef(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "evalFunctionDef, var is: %s\n", displayLexeme(getLeft(tree)));

  Lexeme *var = getLeft(tree);
  Lexeme *closure = getRight(tree);
  closure->type = CLOSURE;
  closure->env = env;
  return insertVal(var, closure, env);
}

Lexeme *evalLambda(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "evalFunctionDef, var is: %s\n", displayLexeme(getLeft(tree)));
  Lexeme *closure = tree;
  closure->type = CLOSURE;
  closure->env = env;
  return closure;
}

Lexeme *evalVariableDef(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "Evaluating varDef, variable is %s, tree is %s, right is %s\n", displayLexeme(getLeft(tree)), displayLexeme(tree), displayLexeme(getRight(tree)));
  Lexeme *value = eval(getRight(tree), env);
  return insertVal(getLeft(tree), value, env);
}

Lexeme *evalArrayDef(Lexeme *tree, Environment *env) {
  Lexeme *size = eval(getRight(tree), env);

  if (size->string != NULL) {
    fprintf(stderr, "Attempted to create an array with size: \"%s\", size must be an integer\n", size->string);
    exit(1);
  }

  // fprintf(stderr, "Size is %d\n", size->integer);
  Lexeme* result = lexeme(ARRAY);
  result->integer = size->integer;
  result->array = malloc(sizeof(int *) * size->integer);

  return insertVal(getLeft(tree), result, env);
}

Lexeme *evalArrayIndex(Lexeme *tree, Environment *env) {
  Lexeme *arr = lookupVar(getLeft(tree), env);
  Lexeme *index = eval(getRight(tree), env);

  if (index->integer >= arr->integer) {
    fprintf(stderr, "Index out of bounds. Size of array is %d index is %d\n", arr->integer, index->integer);
    exit(1);
  }

  Lexeme *result = lexeme(INTEGER);
  result->integer = arr->array[index->integer];
  return result;
}

Lexeme *evalOptExpressionList(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "evalOptExpressionList called\n");
  if (tree != NULL) {
    //TODO - shouldn't have to break abstraction barrier here
    tree->left = eval(getLeft(tree), env);
    // fprintf(stderr, "arg is %s\n", displayLexeme(tree->left));
    // displayEnv(env);

    if (getRight(tree) != NULL) {
      tree->right = evalOptExpressionList(getRight(tree), env);
    }
  }
  return tree;
}

Lexeme *evalOPERATOR(Lexeme *tree, Environment *env) {
  Lexeme *left = eval(getLeft(getRight(tree)), env);
  Lexeme *right = eval(getRight(getRight(tree)), env);
  Lexeme *op = getLeft(tree);
  Lexeme *result = malloc(sizeof(Lexeme));
  result->type = INTEGER;
  // fprintf(stderr, "Lexemes are: %s and %s\n", displayLexeme(left), displayLexeme(right));


  if (strcmp(left->type, INTEGER) == 0 && strcmp(right->type, INTEGER) == 0) {
    if (strcmp(op->type, PLUS) == 0) {
      // fprintf(stderr, "%s %s\n", displayLexeme(left), displayLexeme(right));
      result->integer = left->integer + right->integer;
    } else if (strcmp(op->type, MINUS) == 0) {
      result->integer = left->integer - right->integer;
    } else if (strcmp(op->type, MULTIPLY) == 0) {
      result->integer = left->integer * right->integer;
    } else if (strcmp(op->type, DIVIDE) == 0) {
      result->integer = left->integer / right->integer;
    } else if (strcmp(op->type, EXPONENTIATE) == 0) {
      result->integer = pow(left->integer, right->integer);
    } else if (strcmp(op->type, MODULO) == 0) {
      result->integer = left->integer % right->integer;
    } else if (strcmp(op->type, LT) == 0) {
      result->integer = left->integer < right->integer;
    } else if (strcmp(op->type, GT) == 0) {
      result->integer = left->integer > right->integer;
    } else if (strcmp(op->type, LT_EQUAL) == 0) {
      result->integer = left->integer <= right->integer;
    } else if (strcmp(op->type, GT_EQUAL) == 0) {
      result->integer = left->integer >= right->integer;
    } else if (strcmp(op->type, EQUAL) == 0) {
      result->integer = left->integer == right->integer;
    } else if (strcmp(op->type, N_EQUAL) == 0) {
      result->integer = left->integer != right->integer;
    } else if (strcmp(op->type, AND) == 0) {
      result->integer = left->integer && right->integer;
    } else if (strcmp(op->type, OR) == 0) {
      result->integer = left->integer || right->integer;
    } else if (strcmp(op->type, NOT) == 0) {
      result->integer = !(left->integer);
    } else {
      fprintf(stderr, "Operator not found: %s\n", op->type);
    }
  } else {
    fprintf(stderr, "Incompatible types for %s.  Supported types are INTEGER and INTEGER.  Supplied types are %s and %s\n", op->type, left->type, right->type);
    exit(1);
  }

  // fprintf(stderr, "Result is: %d\n", result->integer);
  return result;
}

//A wrapper for displayLexeme
Lexeme* evalPrint(Lexeme *tree, Environment *env) {
  // fprintf(stderr, "Its' working!\n");

  if (tree != NULL && env != NULL) {
    // fprintf(stdout, "%s", displayLexeme(getLeft(tree)));

    Lexeme* l = getLeft(tree);
    if (l == NULL) {
      printf("NULL");

    } else if (strcmp(l->type, INTEGER) == 0) {
      printf("%d", l->integer);

    } else if (strcmp(l->type, ARRAY) == 0) {
      char *string = malloc(sizeof(char) * 128);
      sprintf(string, "%s[%d] [", l->type, l->integer);
      char *digits = malloc(sizeof(char) * 16); //TODO - isn't 16 the max number of digits for lexing an int
      for (int i = 0; i < l->integer; i++) {
        sprintf(digits, "%d,", l->array[i]);
        strcat(string, digits); //Playing with fire here, don't use strcat at home kids
      }
      char *end = "]\0";
      strncat(string, end, 2);
      fprintf(stderr, "%s", string);

    } else if ((strcmp(l->type, STRING) == 0)
         || (strcmp(l->type, VARIABLE) == 0)) {
      printf("%s", l->string);
    }
    else {
      printf("%s", l->type);
    }

  } else {
    fprintf(stdout, "\n");
  }

  return NULL;
}
