

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rol.h"

#define EXPR_SIZE 512
#define ATOM_SIZE 64
#define TOKEN_OPEN_PARENTHESIS '('
#define TOKEN_CLOSE_PARENTHESIS ')'
#define TOKEN_QUOTE '\''

char* next_token(FILE *fp) {
  return NULL;
}

char* peek_token(FILE *fp) {
  return NULL;
}

S_EXP* s_list(fp) {

  char* token = 

  S_EXP* s_exp = s_expression(fp);


}

S_EXP* s_expression(FILE* fp) {
  char token[ATOM_SIZE];
  next_token(&token, fp);
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
    S_EXP* s_expr = NULL;
    printf("rol>");
    s_expr = s_expression(stdin);
    return s_expr;
}

int main(int argc, char** argv) {

    char expr[EXPR_SIZE];

    while (!feof(stdin)) {
        S_EXP* expr = my_read(stdin);
        //print(eval(expr));
    }

    return 0;
}