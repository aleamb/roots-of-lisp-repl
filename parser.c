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

S_EXP s_expression(S_EXP_PARSER_CONTEXT* lexer);

static int atom_valid_char( char c) {
  return isalnum(c) || strchr(atom_chars, c) != NULL;
}

static void lexer_init(S_EXP_PARSER_CONTEXT* context, FILE* stream) {
  context->stream = stream;
  memset(context->buffer, 0, sizeof(context->buffer));
  context->buffer_index = 0;
  context->line = 1;
  context->position = 0;
  context->status = 0;
  memset(context->token_value, 0, ATOM_SIZE);
}

TOKEN next_token(S_EXP_PARSER_CONTEXT* context, int peek) {
  
  char c = '\0';
  int ti = 0;
  int token_available = 0;
  int prior_buffer_pos = context->buffer_index;
  context->status = 0;
  while (!token_available) {
    if (!(context->buffer[context->buffer_index])) {
      fgets(context->buffer, EXPR_SIZE, context->stream);
      if (feof(context->stream)) {
        context->status = 1;
        return context->token;
      }
      context->status = 0;
      context->buffer_index = 0;
      prior_buffer_pos = 0;
    }
    c = context->buffer[context->buffer_index++];
    if (!peek) context->position++;
    if (c != '\n' && c != '\t' && c != ' ' && c != '\r') {
      switch (c) {
        case '(':
          if (ti > 0) {
            context->token = TOKEN_ATOM;
            context->buffer_index -= 1;
          } else {
            context->token = TOKEN_LIST_OPEN;
          }
          token_available = 1;
          break;
        case ')':
          if (ti > 0) {
            context->token = TOKEN_ATOM;
            context->buffer_index -= 1;
          } else {
            context->token = TOKEN_LIST_CLOSE;
          }
          token_available = 1;
          break;
        case '\'':
         if (ti > 0) {
            context->token = TOKEN_ATOM;
            context->buffer_index -= 1;
          } else {
            context->token = TOKEN_QUOTE;
          }
          token_available = 1;
          break;
        default:
          if (atom_valid_char(c)) {
            context->token_value[ti++] = c;
          } else {
             fprintf(stderr, "Not valid atom character %c at position %d in line %d\n", c, context->position, context->line);
             return context->token;
          }
      }
    } else {
      if (ti > 0) {
        context->token = TOKEN_ATOM;
        token_available = 1;
      }
    }
    if (c == '\n') {
      context->line++;
      context->position = 0;
    }
  }
  context->token_value[ti] = 0;
  if (peek) {
    context->buffer_index = prior_buffer_pos;
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
  if (context->status)
    return NULL;
  S_EXP sexp = NULL;
  if (token == TOKEN_LIST_OPEN) {
    sexp = s_list(context);
  } else if (token == TOKEN_QUOTE) {
    sexp = s_expression(context);
    sexp = s_exp_create_cons(s_exp_create_atom("quote"), s_exp_create_cons(sexp, NULL));
  } else if (token == TOKEN_ATOM) {
      sexp = s_exp_create_atom(context->token_value);
  } else {
    fprintf(stderr, "Syntax error in line %d, position %d\n", context->line, context->buffer_index);
    return NULL;
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
