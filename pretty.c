//Nick Boltralik, 2017

#include <stdio.h>
#include "parser.h"
#include "lexer.h"
#include "pretty.h"
#include "types.h"

void prettyPrint(Lexeme *tree) {

  if (tree == NULL) {
    printf("NULL");
  } else if (tree->type == STATEMENTLIST) {
    // printf("%s\n", tree->type);
    printf("\n");
    prettyPrint(tree->left);
    // printf("\nStatementList: ");
    if (tree->right != NULL) {
      prettyPrint(tree->right);
    }

  } else if (tree->type == OPTEXPRESSIONLIST) {
    printf("(");
    prettyPrint(tree->left);
    printf(", ");
    if (tree->right != NULL) {
      prettyPrint(tree->right);
    }
    printf(")");

  } else if (tree->type == FUNCTIONCALL) {
    printf("run ");
    prettyPrint(tree->left);
    prettyPrint(tree->right);
    printf("\n");

  } else if (tree->type == EXPRESSION) {
    printf("(");
    prettyPrint(tree->left);
    printf(" ");
    prettyPrint(tree->right->left);
    printf(" ");
    prettyPrint(tree->right->right);
    printf(")");

  } else if (tree->type == IFSTATEMENT) {
    printf("if ");
    prettyPrint(tree->left->left);
    printf(" {");
    prettyPrint(tree->left->right);
    printf("\n}\n");
    prettyPrint(tree->right->left);
    printf("else {");
    prettyPrint(tree->right->right);
    printf("\n}\n");

  } else if (tree->type == ELSEIFSTATEMENT) {
    printf("else if ");
    prettyPrint(tree->left->left);
    printf(" {");
    prettyPrint(tree->left->right);
    printf("\n}\n");
    if (tree->right != NULL) {
      prettyPrint(tree->right);
    }

  } else if (tree->type == WHILELOOP) {
    printf("while ");
    prettyPrint(tree->left);
    printf(" {");
    prettyPrint(tree->right);
    printf("\t}");

  } else if (tree->type == LAMBDA) {
    printf("lambda");
    prettyPrint(tree->left);
    printf(" {");
    prettyPrint(tree->right);
    printf("\n}\n");

  // } else if (tree->type == STATEMENT) {
  //   printf("%s\n", tree->type);
  //   prettyPrint(tree->left);
  //   prettyPrint(tree->right);

  } else if (tree->type == INTEGERDEF || tree->type == FUNCTIONDEF
          || tree->type == NULLDEF) {
    printf("Define ");
    prettyPrint(tree->left);
    printf(" = ");
    prettyPrint(tree->right);
    printf("\n");

  } else if (tree->type == STRINGDEF) {
    printf("Define ");
    prettyPrint(tree->left);
    printf(" = \"");
    prettyPrint(tree->right);
    printf("\"\n");

  } else if (tree->type == ARRAYDEF) {
    printf("Define ");
    prettyPrint(tree->left);
    printf("[");
    prettyPrint(tree->right);
    printf("]\n");

  } else if (tree->type == INTEGER) {
    printf("%d", tree->integer);
  } else if (tree->type == STRING) {
    printf("\"%s\"", tree->string);
  } else if (tree->type == VARIABLE) {
    printf("%s", tree->string);

  } else if (tree->type == ASSIGN_NARRAY) {
    prettyPrint(tree->left);
    printf(" = ");
    prettyPrint(tree->right);
    printf("\n");

  } else if (tree->type == ASSIGN_ARRAY) {
    prettyPrint(tree->left);
    printf("[");
    prettyPrint(tree->right->left);
    printf("] = ");
    prettyPrint(tree->right->right);
    printf("\n");

    //TODO - make OPERATOR the type with string equal to the operator name
  } else if (tree->type == PLUS || tree->type == MINUS || tree->type == MULTIPLY ||
             tree->type == DIVIDE || tree->type == EXPONENTIATE || tree->type == MODULO ||
             tree->type == LT || tree->type == GT || tree->type == LT_EQUAL ||
             tree->type == GT_EQUAL || tree->type == EQUAL || tree->type == N_EQUAL ||
             tree->type == AND || tree->type == OR || tree->type == NOT) {
    printf("%s", tree->type);

    //These should not display in normal use
  } else if (tree->type == GLUE) {
    printf("%s\n", tree->type);
    prettyPrint(tree->left);
    printf(" ");
    prettyPrint(tree->right);
  } else {
    printf("OTHER - %s", tree->type);
  }

}
