/* 
  MIT License

  Copyright (c) 2020 Alejandro Ambroa

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE. 
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

typedef enum {
  TOKEN_LIST_OPEN,
  TOKEN_LIST_CLOSE,
  TOKEN_QUOTE,
  TOKEN_ATOM,
  TOKEN_NOT_VALID
} TOKEN;

typedef struct {
  FILE* stream;
  char buffer[EXPR_SIZE];
  char token_value[ATOM_SIZE];
  int buffer_index;
  int line;
  int position;
  TOKEN token;
} S_EXP_LEX;

const char* atom_chars = ".?_-\\/¿!¡=%&$@~¬";

S_EXP s_expression(S_EXP_LEX* lexer);

static int atom_valid_char( char c) {
  return isalnum(c) || strchr(atom_chars, c) != NULL;
}

static void lexer_init(S_EXP_LEX* lexer, FILE* stream) {
  lexer->stream = stream;
  memset(lexer->buffer, 0, sizeof(lexer->buffer));
  lexer->buffer_index = 0;
  lexer->line = 1;
  lexer->position = 0;
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
    if (!peek) lexer->position++;
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
          if (atom_valid_char(c)) {
            lexer->token_value[ti++] = c;
          } else {
             fprintf(stderr, "Not valid atom character %c at position %d in line %d\n", c, lexer->position, lexer->line);
             return TOKEN_NOT_VALID;
          }
      }
    } else {
      if (ti > 0) {
        lexer->token = TOKEN_ATOM;
        token_available = 1;
      }
    }
    if (c == '\n') {
      lexer->line++;
      lexer->position = 0;
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

S_EXP s_list(S_EXP_LEX* lexer) {
  S_EXP current = NULL;
  S_EXP list = NULL;
  TOKEN token;
  while ((token = peek_token(lexer)) != TOKEN_LIST_CLOSE) {
    S_EXP expr  = s_exp_create_cons(s_expression(lexer), NULL);
    if (current != NULL) {
      s_exp_set_cdr(current, expr);
    } else {
      list = expr;
    }
    current = expr;
  }
  next_token(lexer, 0);

  return list ? list : s_exp_create_cons(NULL, NULL);
}

S_EXP s_expression(S_EXP_LEX* lexer) {
  TOKEN token = next_token(lexer, 0);
  S_EXP sexp = NULL;
  if (token == TOKEN_LIST_OPEN) {
    sexp = s_list(lexer);
  } else if (token == TOKEN_QUOTE) {
    sexp = s_expression(lexer);
    sexp = s_exp_create_cons(s_exp_create_atom("quote"), s_exp_create_cons(sexp, NULL));
  } else if (token == TOKEN_ATOM) {
      sexp = s_exp_create_atom(lexer->token_value);
  } else {
    fprintf(stderr, "Syntax error in line %d, position %d\n", lexer->line, lexer->buffer_index);
  }
  return sexp;
}

S_EXP parse(FILE* stream) {
  S_EXP result = NULL;
  S_EXP_LEX* lexer = (S_EXP_LEX*)malloc(sizeof(S_EXP_LEX));
  lexer_init(lexer, stream);
  result = s_expression(lexer);
  free(lexer);
  return result;
}
