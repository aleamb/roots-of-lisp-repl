

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "rol.h"

#define EXPR_SIZE 512
#define ATOM_SIZE 64

typedef enum {
  TOKEN_LIST_OPEN,
  TOKEN_LIST_CLOSE,
  TOKEN_QUOTE,
  TOKEN_ATOM
} TOKEN;


typedef struct {
  FILE* stream;
  char buffer[EXPR_SIZE];
  char token_value[ATOM_SIZE];
  int buffer_index;
  TOKEN token;
} S_EXP_LEX;

void rol_lexer_init(S_EXP_LEX* lexer, FILE* stream) {
  lexer->stream = stream;
  memset(lexer->buffer, 0, sizeof(lexer->buffer));
}

S_EXP* s_expression(S_EXP_LEX* lexer);

TOKEN next_token(S_EXP_LEX* lexer, int peek) {
  
  char c = '\0';
  int ti = 0;
  int token_available = 0;
  int prior_buffer_pos = lexer->buffer_index;
  while (!token_available) {
    if (!(lexer->buffer[lexer->buffer_index])) {
      fgets(lexer->buffer, EXPR_SIZE, lexer->stream);
      lexer->buffer_index = 0;
      prior_buffer_pos = 0;
    }
    c = lexer->buffer[lexer->buffer_index++];
    if (c != '\n' && c != '\t' && c != ' ' && c != '\r') {
      switch (c) {
        case '(':
          if (ti > 0) {
            lexer->token = TOKEN_ATOM;
            lexer->buffer_index -= 1;
          } else {
            lexer->token = TOKEN_LIST_OPEN;
          }
          token_available = 1;
          break;
        case ')':
          if (ti > 0) {
            lexer->token = TOKEN_ATOM;
            lexer->buffer_index -= 1;
          } else {
            lexer->token = TOKEN_LIST_CLOSE;
          }
          token_available = 1;
          break;
        case '\'':
         if (ti > 0) {
            lexer->token = TOKEN_ATOM;
            lexer->buffer_index -= 1;
          } else {
            lexer->token = TOKEN_QUOTE;
          }
          token_available = 1;
          break;
        default:
          if (isalnum(c)) {
            lexer->token_value[ti++] = c;
          }
      }
    } else {
      if (ti > 0) {
        lexer->token = TOKEN_ATOM;
        token_available = 1;
      }
    }
  }
  lexer->token_value[ti] = 0;
  if (peek) {
    lexer->buffer_index = prior_buffer_pos;
  }
  return lexer->token;
}

TOKEN peek_token(S_EXP_LEX* lexer) {
  return next_token(lexer, 1);
}

S_EXP* s_list(S_EXP_LEX* lexer) {
  S_EXP* current = NULL;
  S_EXP *list = NULL;
  TOKEN token;
  while ((token = peek_token(lexer)) != TOKEN_LIST_CLOSE) {
    S_EXP* expr  = rol_make_cons(s_expression(lexer), NULL);
    if (current != NULL) {
      rol_set_cdr(current, expr);
    } else {
      list = expr;
    }
    current = expr;
  }
  next_token(lexer, 0);

  return list ? list : rol_make_cons(NULL, NULL);
}

S_EXP* s_expression(S_EXP_LEX* lexer) {
  TOKEN token = next_token(lexer, 0);
  S_EXP* s_exp = NULL;
  if (token == TOKEN_LIST_OPEN) {
    s_exp = s_list(lexer);
  } else if (token == TOKEN_QUOTE) {
    s_exp = s_expression(lexer);
    s_exp = rol_make_cons(rol_make_atom_from_string("quote"), s_exp);
  } else if (token == TOKEN_ATOM) {
    s_exp = rol_make_atom_from_string(lexer->token_value);
  } else {
    puts("Syntax error");
  }
  return s_exp;
}

S_EXP* my_read(FILE* stream) {
    printf("rol>");
    S_EXP_LEX lexer;
    rol_lexer_init(&lexer, stdin);
    return s_expression(&lexer);
}

void _print(S_EXP* s_exp, int level) {
  if (s_exp == NULL) { printf(")"); return; }
  if (s_exp->type == ATOM) {
    char *symbol = ((TATOM*)(s_exp->expr))->name;
    printf("%s", symbol);
  } else {
    S_EXP* car = ((TCONS*)(s_exp->expr))->car;
    S_EXP* cdr = ((TCONS*)(s_exp->expr))->cdr;
    if (!car && !cdr) {
      printf("NIL");
    } else {
      if (level == 0 || car->type == CONS)
        printf("(");
      _print(car, level + 1 );
      if (cdr != NULL)
        printf(" ");
      _print(cdr,  level + 1 );
     
    }
  }
}

void print(S_EXP* s_exp) {
  _print(s_exp, 0);
  printf("\n");
}

int main(int argc, char** argv) {

    while (!feof(stdin)) {
        S_EXP* expr = my_read(stdin);
        print(expr);
        //print(eval(expr));
    }

    return 0;
}