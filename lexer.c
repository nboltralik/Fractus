//Nick Boltralik, 2017

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "types.h"

int isWhiteSpace(int);
void skipWhiteSpace(Parser*);
int getNextCharacter(Parser*);
char* resize(char*, int*);

Lexeme* lexNumber(Parser*, char);
Lexeme* lexString(Parser*);
Lexeme* lexKeywordOrVar(Parser*, char);
Lexeme* badLexeme(char);

Lexeme *lex(Parser *p) {
  int c;

  skipWhiteSpace(p);
  c = getNextCharacter(p);

  if(c == EOF) {
    return lexeme(END_OF_INPUT);
  }

  switch(c) {
    case '(':
      return lexeme(OPEN_PAREN);
    case ')':
      return lexeme(CLOSE_PAREN);
    case '{':
      return lexeme(OPEN_CURL);
    case '}':
      return lexeme(CLOSE_CURL);
    case '[':
      return lexeme(OPEN_SQUARE);
    case ']':
      return lexeme(CLOSE_SQUARE);
    case ',':
      return lexeme(COMMA);
    case '+':
      return lexeme(PLUS);
    case '-':
      return lexeme(MINUS);
    case '*':
      return lexeme(MULTIPLY);
    case '/':
      return lexeme(DIVIDE);
    case '^':
      return lexeme(EXPONENTIATE);
    case '%':
      return lexeme(MODULO);
    case '<':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(LT_EQUAL);
      }
      ungetc(c, p->fp);
      return lexeme(LT);
    case '>':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(GT_EQUAL);
      }
      ungetc(c, p->fp);
      return lexeme(GT);
    case '=':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(EQUAL);
      }
      ungetc(c, p->fp);
      return lexeme(ASSIGN);
    case ';':
      return lexeme(SEMICOLON);
    case '!':
      c = getNextCharacter(p);
      if(c == '=') {
        return lexeme(N_EQUAL);
      }
      ungetc(c, p->fp);
      return lexeme(NOT);
    case '&':
      return lexeme(AND);
    case '|':
      return lexeme(OR);

    default:
      if (isdigit(c)) {
        return lexNumber(p, c);
      }
      else if (isalpha(c)) {
        return lexKeywordOrVar(p, c);
      }
      else if (c == '\"') {
        return lexString(p);
      }
      else {
        return badLexeme(c);
      }
  }
}

Lexeme* lexeme(char* s) {
  Lexeme *lex = malloc(sizeof(Lexeme));
  // fprintf(stderr, "type passed to lexeme is: %s\n", s);
  lex->type = s;
  lex->string = NULL;
  lex->integer = 0;
  lex->array = NULL;
  lex->function = NULL;
  lex->env = NULL;

  lex->left = NULL;
  lex->right = NULL;

  return lex;
}

Lexeme *lexNumber(Parser *p, char c) {
  int index = 1, size = 16;
  int n;
  char *num = malloc(sizeof(char) * size);
  Lexeme *lex = lexeme(INTEGER);
  num[0] = c;
  n = getNextCharacter(p);

  while(isdigit(n) && !isWhiteSpace(n) && n != EOF) {
    if(index >= size) {
      num = resize(num,&size);
    }
    num[index++] = n;
    n = getNextCharacter(p);
  }
  ungetc(n,p->fp);
  num[index++] = '\0';
  //TODO - don't use atoi
  lex->integer = atoi(num);
  // lex->string = num;

  return lex;
}

Lexeme *lexKeywordOrVar(Parser *p, char c) {
  int index = 1, size = 16;
  int n;
  Lexeme *lex = lexeme(KEYWORD);
  char *name = malloc(sizeof(char) * size);
  name[0] = c;
  n = getNextCharacter(p);

  while((isalpha(n) || isdigit(n)) && !isWhiteSpace(n) && n != EOF) {
    if(index >= size) {
      name = resize(name, &size);
    }
    name[index++] = n;
    n = getNextCharacter(p);
  }

  //TODO - change these to Latin
  ungetc(n, p->fp);
  name[index++] = '\0';

  if(strcmp(name, "incognita") == 0) {
    lex->type = LAMBDA;
  } else if(strcmp(name, "si") == 0) {
    lex->type = IF;
  } else if(strcmp(name, "tandem") == 0) {
    lex->type = ELSE;
  } else if(strcmp(name, "aliud") == 0) {
    lex->type = ELSEIF;
  } else if(strcmp(name, "praenomino") == 0) {
    lex->type = DEFINE;
  } else if(strcmp(name, "dum") == 0) {
    lex->type = WHILE;
  // } else if(strcmp(name, "nullum") == 0) { //TODO - do I ever use this?
  //   lex->type = "NULL";
  } else if(strcmp(name,"exequor") == 0) {
    lex->type = RUN;
  } else {
    lex = lexeme(VARIABLE);
    lex->string = name;
  }
  return lex;
}

Lexeme *lexString(Parser *p) {
  int index = 0, size = 32;
  int n;
  Lexeme *lex = lexeme(STRING);
  char *string = malloc(sizeof(char) * size);
  n = getNextCharacter(p);

  while(n != '\"') {
    if(n == EOF) {
      fprintf(stderr,"FATAL ERROR: STRING NOT CLOSED\n");
      exit(1);
    }
    if(index >= size) {
      string = resize(string,&size);
    }
    if(n != '\\') {
      string[index++] = n;
    } else {
      n = getNextCharacter(p);
      if(n == 'n') {
        string[index++] = '\n';
      } else if(n == 't') {
        string[index++] = '\t';
      } else if(n == 'r') {
        string[index++] = '\r';
      } else {
        string[index++] = n;
      }
    }
    n = getNextCharacter(p);
  }
  string[index++] = '\0';
  lex->string = string;
  lex->integer = index;

  return lex;
}

Lexeme* badLexeme(char c) {
  Lexeme *lex = lexeme(BAD_LEX);
  char *bad_char = malloc(sizeof(char));
  bad_char[0] = c;
  lex->string = bad_char;
  return lex;
}

char* displayLexeme(Lexeme *l) {
  char *string = malloc(sizeof(char) * 128);
  // fprintf(stderr, "%s\n", l->type);
  if (l == NULL) {
    return "NULL Lexeme";

  } else if (strcmp(l->type, INTEGER) == 0) {
    sprintf(string, "%s %d", l->type, l->integer);

  } else if (strcmp(l->type, ARRAY) == 0) {
    sprintf(string, "%s[%d] [", l->type, l->integer);
    char *digits = malloc(sizeof(char) * 16); //TODO - isn't 16 the max number of digits for lexing an int
    for (int i = 0; i < l->integer; i++) {
      sprintf(digits, "%d,", l->array[i]);
      strcat(string, digits); //Playing with fire here, don't use strcat at home kids
    }
    char *end = "]\0";
    strncat(string, end, 2);

  // } else if (strcmp(l->type, CLOSURE) == 0) {
  //   sprintf(string, "%s", CLOSURE);

  } else if ((strcmp(l->type, STRING) == 0)
       || (strcmp(l->type, VARIABLE) == 0)
       || (strcmp(l->type, BAD_LEX) == 0)) {
    sprintf(string, "%s \"%s\"", l->type, l->string);
  }
  else {
    sprintf(string, "%s", l->type);
  }
  return string;
}

void skipWhiteSpace(Parser *p) {
  int c = getNextCharacter(p);

  while(!feof(p->fp) && isWhiteSpace(c)) {
    if(c == '\n') {
      p->line++;
    }
    c = getNextCharacter(p);
  }
  ungetc(c, p->fp);
}

int isWhiteSpace(int c) {
  return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

int getNextCharacter(Parser *p) {
  int c = fgetc(p->fp);

  // comments start with !!
  if(c == '!') {
    int s = fgetc(p->fp);
    if(s == '!') {
      while(!feof(p->fp) && c != '\n') {
        c = fgetc(p->fp);
      }
    } else {
      ungetc(s,p->fp);
    }
  }

  return c;
}

//wrapper for realloc
char *resize(char *original, int *size) {
  *size *= 2;
  return realloc(original, sizeof(char) * (*size));
}

Lexeme* newNode(char *type, Lexeme *left, Lexeme *right) {
  Lexeme *lex = lexeme(type);
  lex->left = left;
  lex->right = right;

  return lex;
}

Lexeme* getLeft(Lexeme *l) {
  return l->left;
}

Lexeme* getRight(Lexeme *l) {
  return l->right;
}

void setLeft(Lexeme *l, Lexeme *left) {
  l->left = left;
}

void setRight(Lexeme *l, Lexeme *right) {
  l->right = right;
}
