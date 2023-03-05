#include "lisp.h"
#include <string.h>

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

