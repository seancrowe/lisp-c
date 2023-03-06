#include "lisp.h"
#include <stdio.h>
#include <stdlib.h>


#define MAX_BUFFER 128

int main(int argc, char **argv) {

    Atom env;
    char *input;

    // Create the global environment
    env = env_create(nil);

    for (;;) {

        char *input = malloc(MAX_BUFFER);
        if (input == NULL) {
            printf("No Memory\n");
        }

        printf("> ");
        fgets(input, MAX_BUFFER, stdin);

        const char *p = input;
        Error err;
        Atom expr, result;

        err = read_expr(p, &p, &expr);

        if (!err) {
            err = eval_expr(expr, env, &result);
        }

        switch(err) {
            case Error_OK:
                print_expr(result);
                putchar('\n');
                break;
            case Error_Syntax:
                puts("Syntax Error");
                break;
            case Error_Unbound:
                puts("Symbol not bound");
                break;
            case Error_Args:
                puts("Wrong number of arguments");
                break;
            case Error_Type:
                puts("Wrong type");
                break;
        }
       free(input);
    }
}
