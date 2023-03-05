#include "lisp.h"
#include "core.c"
#include "lexer.c"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int parse_simple(const char *start, const char *end, Atom *result) {
    char *buf, *p;

    // Check if it is an integer
    long val = strtol(start, &p, 10);
    if (p == end) {
        result->type = AtomType_Integer;
        result->value.integer = val;
        return Error_OK;
    }

    // Check if NIL or symbol
    buf = malloc(end - start + 1);
    p = buf;
    // Copy over the uppercase version of string to a buffer. Makes a case-insensitve LISP per original
    while (start != end) {
        *p++ = toupper(*start), ++start;
    }
    *p = '\0';

    if(strcmp(buf, "NIL") == 0) {
        *result = nil;
    } else {
        *result = create_sym(buf);
    }

    free(buf);
    return Error_OK;
}

int read_expr(const char *input, const char **end, Atom *result);

int read_list(const char *start, const char **end, Atom *result) {
    Atom p;

    *end = start;
    p = *result = nil;

    for (;;) {
        const char *token;
        Atom item;
        Error err;

        err = lex(*end, &token, end);
        if (err) {
            return err;
        }

        if (token[0] == ')') {
            return Error_OK;

        }

        if (token[0] == '.' && *end - token == 1) {
            // Improper list
            if (nilp(p)) {
                return Error_Syntax;
            }

            err = read_expr(*end, end, &item);
            if (err) {
                return err;
            }

            cdr(p) = item;

            // Read the closing )
            err = lex(*end, &token, end);
            if (!err && token[0] != ')') {
                err = Error_Syntax;
            }

            return err;
        }

        err = read_expr(token, end, &item);
        if (err) {
            return err;
        }

        if (nilp(p)) {
            // First item
            *result = cons(item, nil);
            p = *result;
        } else {
            cdr(p) = cons(item, nil);
            p = cdr(p);
        }
    }
}

int read_expr(const char *input, const char **end, Atom *result) {
    const char *token;
    Error err;

    err = lex(input, &token, end);
    if (err) {
        return err;
    }

    if(token[0] == '(') {
        return read_list(*end, end, result);
    } else if (token[0] == ')') {
        return Error_Syntax;
    } else {
        return parse_simple(token, *end, result);
    }
}
