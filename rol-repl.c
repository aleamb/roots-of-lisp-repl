

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "rol.h"

#define EXPR_SIZE 512

S_EXP* parse_s_expr(char *buffer, S_EXP* parent_s_expr, int* cread, int* p) {
    int token_active = 0;
    char c;
    S_EXP* local_s_expr = NULL;
    while ((c = *buffer++)) {
        *cread++;
        switch (c) {
            case '\'': {
                int r = 0;
                p++;
                local_s_expr = rol_make_cons(rol_make_atom_from_string("quote"), parse_s_expr(buffer, parent_s_expr, &r, &p));
                *cread += r;
                return local_s_expr;
            }
            case '(': {
                int r = 0;
                p++;
                S_EXP* car = parse_s_expr(buffer, s_expr, &r, &p);
                buffer += r;
                S_EXP* cdr = parse_s_expr(buffer, s_expr, &r, &p);
                local_s_expr = rol_make_cons(car, cdr);
                return local_s_expr;
            }
            case ')':
                p--;
                return local_s_expr;
            case ' ':
            case '\n':
            case '\t':
                if (token_active) {
                    return local_s_expr;
                }
                break;
            default:
                token_active = 1;
                if (!local_s_expr) {
                    local_s_expr = rol_make_atom(c);
                } else {
                    rol_add_atom_char(local_s_expr, c);
                }
            break;
        }
    }
    return local_s_expr;
}

S_EXP* my_read(FILE* stream) {
    char buffer[BUFSIZ];
    S_EXP* s_expr = NULL;
    int readed = 0;
    int paren = 0;
    int need_tokens = 1;
    while (need_tokens && fgets(buffer, EXPR_SIZE, stream) != NULL) {
        s_expr = parse_s_expr(buffer, s_expr, &readed, &paren);
        need_tokens = (paren != 0);
    }
    return s_expr;
}

int main(int argc, char** argv) {

    char expr[EXPR_SIZE];

    while (!feof(stdin)) {
        S_EXP* expr = my_read(stdin);
        //eval(expr);
        //print();
    }

    return 0;
}