#ifndef LISP_H_
#define LISP_H_

struct Atom {
    enum {
        AtomType_Nil,
        AtomType_Pair,
        AtomType_Symbol,
        AtomType_Integer
    } type;

    union {
        struct Pair *pair;
        const char *symbol;
        long integer;
    } value;
};

struct Pair {
    struct Atom atom[2];
};

typedef enum {
    Error_OK = 0,
    Error_Syntax
} Error;

typedef struct Atom Atom;

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
// nilp = nil predicate; Basically nil?
#define nilp(atom) ((atom).type == AtomType_Nil)

static const Atom nil = { AtomType_Nil };
static Atom sym_table = { AtomType_Nil };

#endif // LISP_H_
