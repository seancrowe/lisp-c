#include "lisp.h"

int builtin_car(Atom args, Atom *result) {
    if (nilp(args) || !nilp(cdr(args))) {
        return Error_Args;
    }

    if (nilp(car(args))) {
        *result = nil;
    } else if (car(args).type != AtomType_Pair) {
        return Error_Type;
    } else {
        *result = car(car(args));
    }

    return Error_OK;
}

int builtin_cdr(Atom args, Atom *result) {
    if (nilp(args) || !nilp(cdr(args))) {
        return Error_Args;
    }

    if (nilp(car(args))) {
        *result = nil;
    } else if (car(args).type != AtomType_Pair) {
        return Error_Type;
    } else {
        *result = cdr(car(args));
    }

    return Error_OK;
}

int builtin_cons(Atom args, Atom *result) {
    if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
        return Error_Args;
    }

    *result = cons(car(args), car(cdr(args)));

    return Error_OK;
}
