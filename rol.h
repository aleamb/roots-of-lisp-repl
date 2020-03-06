#ifndef _TYPES_H
#define _TYPES_H_

typedef enum _s_exp_type {
    ATOM,
    CONS
} S_EXP_TYPE;

typedef struct _s_exp {
    S_EXP_TYPE type;
    void* expr;
} S_EXP;

typedef struct _atom {
    char *name;
} TATOM;

typedef struct _cons {
    S_EXP* car;
    S_EXP* cdr;
} TCONS;


S_EXP* rol_make_atom_from_string(const char*);

S_EXP* rol_make_cons(const S_EXP*, const S_EXP*);


#endif /* _TYPES_H_ */
