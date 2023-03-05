#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lisp.h"
#include "lexer.c"

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
    a.value.symbol = strdup(name);
    sym_table = cons(a, sym_table);

    return a;
}

// Print recursively. Could in theory run out of stack space.
void print_expr(Atom atom) {
    switch (atom.type) {
        case AtomType_Nil:
            printf("NIL");
            break;
        case AtomType_Pair:
            putchar('(');
            print_expr(car(atom));
            atom = cdr(atom);

            while (!nilp(atom)) {
                if (atom.type == AtomType_Pair) {
                    putchar(' ');
                    print_expr(car(atom));
                    atom = cdr(atom);
                } else {
                    printf(" . ");
                    print_expr(atom);
                    break;
                }
            }
            putchar(')');
            break;
        case AtomType_Symbol:
            printf("%s", atom.value.symbol);
            break;
        case AtomType_Integer:
            printf("%ld", atom.value.integer);
            break;
    }
}

// TODO(austin): put tests somewhere else and build a proper header file
void initial_test() {
    print_expr(create_int(42));
    printf("\n");
    print_expr(create_sym("SYM"));
    printf("\n");
    print_expr(create_sym("SYM"));
    printf("\n");
    print_expr(cons(create_sym("AGE"), create_int(23)));
    printf("\n");

    Atom list = cons(create_int(1), cons(create_int(2), cons(create_int(3), nil)));
    print_expr(list);
    printf("\n");
};

int main(void) {
    initial_test();
    return 0;
};
