

#include "parser.h"
#include <string.h>
#include <ctype.h>

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



S_EXP* s_expression(S_EXP_LEX* lexer);

const char* atom_chars = ".?_-\\/¿!¡=%&$@~¬";

static int lexer_atom_char(char c) {
  return strchr(atom_chars, c) != NULL;
}
static int atom_char( char c) {
  return isalnum(c) || lexer_atom_char(c);
}

void rol_lexer_init(S_EXP_LEX* lexer, FILE* stream) {
  lexer->stream = stream;
  memset(lexer->buffer, 0, sizeof(lexer->buffer));
}

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
          if (atom_char(c)) {
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
    s_exp = rol_make_cons(rol_make_atom_from_string("quote"), rol_make_cons(s_exp, NIL));
  } else if (token == TOKEN_ATOM) {
      s_exp = rol_make_atom_from_string(lexer->token_value);
  } else {
    puts("Syntax error");
  }
  return s_exp;
}

S_EXP* parse(FILE* stream) {
    S_EXP_LEX lexer;
    rol_lexer_init(&lexer, stream);
    return s_expression(&lexer);
}