#include "lisp.h"
#include <string.h>

Atom create_env(Atom parent) {
    return cons(parent, nil);
}
