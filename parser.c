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

const char* atom_chars = ".?_-\\/¿!¡=&$@~¬+-*~<>%";
const char* ignored_chars = " \n\t\r";

S_EXP s_expression(S_EXP_PARSER_CONTEXT* lexer);

static int atom_valid_char(char c) {
  return isalnum(c) || strchr(atom_chars, c) != NULL;
}

static int is_ignore_char(char c) {
  return strchr(ignored_chars, c) != NULL;
}

static void lexer_init(S_EXP_PARSER_CONTEXT* context, FILE* stream) {
  context->stream = stream;
  memset(context->buffer, 0, sizeof(context->buffer));
  context->buffer_index = 0;
  context->line = 0;
}

static char lexer_getchar(S_EXP_PARSER_CONTEXT* context) {
  if (context->buffer[context->buffer_index] == 0) {
    memset(context->token_value, 0, ATOM_SIZE);
    if (!fgets(context->buffer, EXPR_SIZE, context->stream))
      return EOF;
    context->buffer_index = 0;
    context->position = 0;
    context->line++;
    context->status = 0;
  }
  context->position++;
  return context->buffer[context->buffer_index++];
}

TOKEN next_token(S_EXP_PARSER_CONTEXT* context, int peek) {
  char c = 0;
  int bp = context->buffer_index;
  context->status = 0;
  while (is_ignore_char( (c = lexer_getchar(context)) ));
  if (atom_valid_char(c)) {
    int n = 0;
    do {
      context->token_value[n++] = c;
      c = lexer_getchar(context);
    } while(atom_valid_char(c));
    context->token = TOKEN_ATOM;
  } else {
      if (c == '\'') context->token = TOKEN_QUOTE;
      else if (c == '(') context->token = TOKEN_LIST_OPEN;
      else if (c == ')') context->token = TOKEN_LIST_CLOSE;
      else {
        context->token = TOKEN_INVALID; 
        context->token_value[0] = c;
      }
  }
  return context->token;
}

TOKEN peek_token(S_EXP_PARSER_CONTEXT* context) {
  return next_token(context, 1);
}

S_EXP s_list(S_EXP_PARSER_CONTEXT* context) {
  S_EXP current = NULL;
  S_EXP list = NULL;
  TOKEN token;
  while ((token = peek_token(context)) != TOKEN_LIST_CLOSE) {
    S_EXP expr  = s_exp_create_cons(s_expression(context), NULL);
    if (current != NULL) {
      s_exp_set_cdr(current, expr);
    } else {
      list = expr;
    }
    current = expr;
  }
  next_token(context, 0);

  return list ? list : s_exp_create_cons(NULL, NULL);
}

S_EXP s_expression(S_EXP_PARSER_CONTEXT* context) {
  TOKEN token = next_token(context, 0);
  S_EXP sexp = NULL;
  if (token == TOKEN_INVALID) {
    fprintf(stderr, "Lexical error in line %d, position %d - Character %c not valid\n", context->line, context->buffer_index);
  }
  else if (token == TOKEN_LIST_OPEN) {
    sexp = s_list(context);
  } else if (token == TOKEN_QUOTE) {
    sexp = s_expression(context);
    sexp = s_exp_create_cons(s_exp_create_atom("quote"), s_exp_create_cons(sexp, NULL));
  } else if (token == TOKEN_ATOM) {
      sexp = s_exp_create_atom(context->token_value);
  }
  return sexp;
}

S_EXP parse(S_EXP_PARSER_CONTEXT* context) {
  S_EXP result = NULL;
  result = s_expression(context);
  return result;
}

S_EXP_PARSER_CONTEXT* init_parser(FILE* stream) {
  S_EXP_PARSER_CONTEXT* context = (S_EXP_PARSER_CONTEXT*)malloc(sizeof(S_EXP_PARSER_CONTEXT));
  lexer_init(context , stream);
  return context;
}
