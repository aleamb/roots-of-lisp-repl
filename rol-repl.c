

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rol.h"

#define EXPR_SIZE 512
#define ATOM_SIZE 64
#define TOKEN_OPEN_PARENTHESIS '('
#define TOKEN_CLOSE_PARENTHESIS ')'
#define TOKEN_QUOTE '\''

S_EXP* s_expression(FILE* fp);

char* next_token(char* token, FILE *fp) {
  return NULL;
}

char* peek_token(char* token, FILE *fp) {
  return NULL;
}

S_EXP* s_list(FILE* fp) {
  char token[ATOM_SIZE];
  peek_token(token, fp);
  S_EXP* s_expr = NULL;
  while (token[0] != TOKEN_CLOSE_PARENTHESIS) {
      if (!s_expr) {
        s_expr = rol_make_cons(s_expression(fp), NULL);
      } else {
          S_EXP *local_expr = rol_make_cons(s_expr, NULL);
          rol_set_cdr(s_expr, local_expr);
          s_expr = local_expr;
      }
      peek_token(token, fp);
  }
  return s_expr;
}

S_EXP* s_expression(FILE* fp) {
  char token[ATOM_SIZE];
  next_token(token, fp);
  S_EXP* s_exp = NULL;
  if (token[0] == TOKEN_OPEN_PARENTHESIS) {
    s_exp = s_list(fp);
  } else if (token[0] == TOKEN_QUOTE) {
    s_exp = s_expression(fp);
    s_exp = rol_make_cons(rol_make_atom_from_string("quote"), s_exp);
  } else if (isalnum(token[0])) {
    s_exp = rol_make_atom_from_string(token);
  } else {
    puts("Syntax error");
  }
  return s_exp;
}

S_EXP* my_read(FILE* stream) {
    printf("rol>");
    return s_expression(stdin);
}

int main(int argc, char** argv) {

    char expr[EXPR_SIZE];

    while (!feof(stdin)) {
        S_EXP* expr = my_read(stdin);
        //print(eval(expr));
    }

    return 0;
}