cmake_minimum_required(VERSION 3.5)

project(rol)

add_executable(rol sexp.c parser.c rol.c print.c rol-repl.c memory.c)
