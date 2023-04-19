#ifndef LISP_H_
#define LISP_H_

// DO THIS IN JAVA!!!!!!!!!!!!!!

struct Atom;

typedef int (*Builtin)(struct Atom args, struct Atom *result);

struct Atom {
    enum {
        AtomType_Nil,
        AtomType_Pair,
        AtomType_Symbol,
        AtomType_Integer,
        AtomType_Builtin
    } type;

    union {
        struct Pair *pair;
        const char *symbol;
        long integer;
        Builtin builtin;
    } value;
};

struct Pair {
    struct Atom atom[2];
};

typedef enum {
    Error_OK = 0,
    Error_Syntax,
    Error_Unbound,
    Error_Args,
    Error_Type
} Error;

typedef struct Atom Atom;

#define car(p) ((p).value.pair->atom[0])
#define cdr(p) ((p).value.pair->atom[1])
// nilp = nil predicate; Basically nil?
#define nilp(atom) ((atom).type == AtomType_Nil)

static const Atom nil = { AtomType_Nil };
static Atom sym_table = { AtomType_Nil };

/* Eval */
Atom env_create(Atom parent);
int env_get(Atom env, Atom symbol, Atom *result);
int env_set(Atom env, Atom symbol, Atom value);
int eval_expr(Atom expr, Atom env, Atom *result);

/* Data */
Atom cons(Atom car_val, Atom cdr_val);
Atom create_int(long x);
Atom create_sym(const char *name);
Atom create_builtin(Builtin fn);
int listp(Atom expr);
Atom copy_list(Atom list);

/* Builtins */
int builtin_car(Atom args, Atom *result);
int builtin_cdr(Atom args, Atom *result);
int builtin_cons(Atom args, Atom *result);

/* Lexer */
int lex(const char *str, const char **start, const char **end);

/* Parser */
int read_expr(const char *input, const char **end, Atom *result);

/* Printer */
void print_expr(Atom atom);

#endif // LISP_H_
