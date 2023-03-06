#include "lisp.h"
#include <string.h>

/*
 * Evaluation rules for common LISP
 *
 * A literal will evaluate to itself
 * The environment allows us to determine a value for an identifier(/symbol).
 * Attempting to evaluation an identifier for which no binding exists is an error.
 *
 * A list expression with one of the following operators is called a special form:
 *
 * QUOTE
 *      - The result of evaluation (QUOTE EXPR) is EXPR, returned without evaluation
 * DEFINE
 *      - Evaluation (DEFINE SYMBOL EXPR) creates a binding for SYMBOL in the evaluation environemnt
 *        SYMBOL is bound to the value obtained by evaluation EXPR. Final result is SYMBOL
 *
 * Anything else, including list expressions with any other operator is invalid.
 */

Atom env_create(Atom parent) {
    return cons(parent, nil);
}

int env_get(Atom env, Atom symbol, Atom *result) {
    Atom parent = car(env);
    Atom bs = cdr(env);

    while (!nilp(bs)) {
        Atom b = car(bs);

        // Don't need to use strcmp since we don't allow duplicate symbols
        if (car(b).value.symbol == symbol.value.symbol) {
            *result = cdr(b);
            return Error_OK;
        }

        bs = cdr(bs);
    }

    // If we are at the top level (parent is NIL) then throw unbound symbol
    if (nilp(parent)) {
        return Error_Unbound;
    }

    // Recursively check if the parent of this env happens to have the symbol
    return env_get(parent, symbol, result);
}

int env_set(Atom env, Atom symbol, Atom value) {
    Atom bs = cdr(env);
    Atom b = nil;

    // Attempt to find the symbol in the environment, set it if found
    while(!nilp(bs)) {
        b = car(bs);
        // Don't need to use strcmp since we don't allow duplicate symbols
        if (car(b).value.symbol == symbol.value.symbol) {
           cdr(b) = value;
           return Error_OK;
        }
        bs = cdr(bs);
    }

    // Symbol not found in environment, create a pair and append it to the end of env
    b = cons(symbol, value);
    cdr(env) = cons(b, cdr(env));

    return Error_OK;
}

int eval_expr(Atom expr, Atom env, Atom *result) {
    Atom op, args;
    Error err;

    // If expression is a symbol, return the lookup for it
    if (expr.type == AtomType_Symbol) {
        return env_get(env, expr, result);

    } else if (expr.type != AtomType_Pair) {
        // It's a literal, return
        *result = expr;
        return Error_OK;
    }

    // Check to make sure it's a valid list structure
    if (!listp(expr)) {
        return Error_Syntax;
    }

    op = car(expr);
    args = cdr(expr);

    if (op.type == AtomType_Symbol) {
        if (strcmp(op.value.symbol, "QUOTE") == 0) {
            if (nilp(args) || !nilp(cdr(args))) {
                return Error_Args;
            }

            *result = car(args);
            return Error_OK;
        } else if (strcmp(op.value.symbol, "DEFINE") == 0) {
            Atom sym, val;

            // Make sure we have 2 arguments
            if (nilp(args) || nilp(cdr(args)) || !nilp(cdr(cdr(args)))) {
                return Error_Args;
            }

            // Make sure the first argument is a symbol
            sym = car(args);
            if (sym.type != AtomType_Symbol) {
                return Error_Type;
            }

            // Evaluate the second argument, set env symbol as result if it did not error
            err = eval_expr(car(cdr(args)), env, &val);
            if (err) {
                return err;
            }

            *result = sym;
            return env_set(env, sym, val);

        }
    }
    return Error_Syntax;
}
