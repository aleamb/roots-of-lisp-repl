/*
    The Roots of Lisp interpreter

    Parser

    BNF from http://cuiwww.unige.ch/isi/bnf/LISP/BNFlisp.html

    s_expression = atomic_symbol \
                / "(" s_expression "."s_expression ")" \
                / list 

    list = "(" s_expression < s_expression > ")"

    atomic_symbol = letter atom_part

    atom_part = empty / letter atom_part / number atom_part

    letter = "a" / "b" / " ..." / "z"

    number = "1" / "2" / " ..." / "9"

    empty = " "

*/

#include "buffer.h"

void s_expression(BUFFER* buffer) {

    int ch = peek_char(buffer);

}