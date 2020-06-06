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
S_EXP s_list(S_EXP_PARSER_CONTEXT* lexer);

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
  }
  context->position++;
  return context->buffer[context->buffer_index++];
}

static S_EXP s_expression_analyze(S_EXP_PARSER_CONTEXT* context, TOKEN token) {
  S_EXP sexp = NULL;
  if (token == TOKEN_EOF) return NULL;
  if (token == TOKEN_INVALID) {
    fprintf(stderr, "Lexical error at line %d, position %d - Token %s not expected\n", context->line, context->position, context->token_value);
  } else if (token == TOKEN_LIST_OPEN) {
    sexp = s_list(context);
  } else if (token == TOKEN_QUOTE) {
    sexp = s_expression(context);
    sexp = s_exp_create_cons(s_exp_create_atom("quote"), s_exp_create_cons(sexp, NULL));
  } else if (token == TOKEN_ATOM) {
      sexp = s_exp_create_atom(context->token_value);
  } else {
     fprintf(stderr, "Syntax error at line %d, position %d\n", context->line, context->position);
  }
  return sexp;
}

TOKEN next_token(S_EXP_PARSER_CONTEXT* context) {
  char c = 0;
  while (is_ignore_char( (c = lexer_getchar(context)) ));
  if (atom_valid_char(c)) {
    int n = 0;
    do {
      context->token_value[n++] = c;
      c = lexer_getchar(context);
    } while(atom_valid_char(c));
	context->token_value[n] = 0;
	context->buffer_index--;
    context->token = TOKEN_ATOM;
  } else {
      if (c == '\'') context->token = TOKEN_QUOTE;
      else if (c == '(') context->token = TOKEN_LIST_OPEN;
      else if (c == ')') context->token = TOKEN_LIST_CLOSE;
      else if (c == EOF) context->token = TOKEN_EOF; 
      else { 
        context->token_value[0] = c;
        context->token = TOKEN_INVALID;
      }
  }
  return context->token;
}

S_EXP s_list(S_EXP_PARSER_CONTEXT* context) {
  TOKEN token = next_token(context);
  if (token == TOKEN_EOF) {
    fprintf(stderr, "EOF found at line %d, position %d\n", context->line, context->position);
    return NULL;
  }
  if (token == TOKEN_LIST_CLOSE) return NULL;
  return s_exp_create_cons(s_expression_analyze(context, token), s_list(context));
}

S_EXP s_expression(S_EXP_PARSER_CONTEXT* context) {
  TOKEN token = next_token(context);
  return s_expression_analyze(context, token);
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
