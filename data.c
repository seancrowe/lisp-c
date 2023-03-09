#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lisp.h"

// Create pair atom on the heap; will leak memory at the moment
Atom cons(Atom car_val, Atom cdr_val) {
    Atom p;

    p.type = AtomType_Pair;
    p.value.pair = malloc(sizeof(struct Pair));

    car(p) = car_val;
    cdr(p) = cdr_val;

    return p;
}

// Create an integer atom
Atom create_int(long x) {
    Atom a;
    a.type = AtomType_Integer;
    a.value.integer = x;

    return a;
}

// Create a symbol atom
Atom create_sym(const char *name) {
    Atom a, p;

    // Check to see if symbol exsits on the symbol table, if it does then return it
    p = sym_table;
    while (!nilp(p)) {
        a = car(p);
        if (strcmp(a.value.symbol, name) == 0)
            //printf("Returning symbol from table!\n");
            return a;
        p = cdr(p);
    }

    // Symbol does not exist, so we will create one and add it to the symbol table
    a.type = AtomType_Symbol;
    a.value.symbol = _strdup(name);
    sym_table = cons(a, sym_table);

    return a;
}

Atom create_builtin(Builtin fn) {
    Atom a;
    a.type = AtomType_Builtin;
    a.value.builtin = fn;
    return a;
}

// Check if expression is a list
int listp(Atom expr) {
    while (!nilp(expr)) {
        if(expr.type != AtomType_Pair) {
            return 0;
        }
        expr = cdr(expr);
    }
    return 1;
}

Atom copy_list(Atom list) {
    Atom a, p;

    if (nilp(list)){
        return nil;
    }

    a = cons(car(list), nil);
    p = a;
    list = cdr(list);

    while (!nilp(list)) {
        cdr(p) = cons(car(list), nil);
        p = cdr(p);
        list = cdr(list);
    }

    return a;
}
