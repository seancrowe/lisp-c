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

/* Data */
Atom cons(Atom car_val, Atom cdr_val);
Atom create_int(long x);
Atom create_sym(const char *name);

/* Lexer */
int lex(const char *str, const char **start, const char **end);

/* Parser */
int read_expr(const char *input, const char **end, Atom *result);

/* Printer */
void print_expr(Atom atom);

#endif // LISP_H_
