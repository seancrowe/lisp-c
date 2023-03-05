#include "lisp.h"
#include "core.c"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int lex(const char *str, const char **start, const char **end) {
    const char *ws = " \t\n";
    const char *delim = "() \t\n";
    const char *prefix = "()";

    // increment the pointer to the first position without a space, tab, or newline if possible
    str += strspn(str, ws);

    if (str[0] == '\0') {
        *start = *end = NULL;
        return Error_Syntax;
    }

    // Store the address for the start of the token
    *start = str;

    // Check to see if a ( or a ) exist at the first position of the string
    if (strchr(prefix, str[0]) != NULL) {
        // if they are not, then set the end address to mark the end of a token
        *end = str + 1;
    } else {
        *end = str + strspn(str, delim);
    }

    return Error_OK;
}

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
