#include "lisp.h"
#include <stdio.h>
#include <stdlib.h>


#define MAX_BUFFER 128

int main(int argc, char **argv) {

    char *input;

    for (;;) {

        char *input = malloc(MAX_BUFFER);
        if (input == NULL) {
            printf("No Memory\n");
        }

        printf("> ");
        fgets(input, MAX_BUFFER, stdin);

        const char *p = input;
        Error err;
        Atom expr;

        err = read_expr(p, &p, &expr);

        switch(err) {
            case Error_OK:
                print_expr(expr);
                putchar('\n');
                break;
            case Error_Syntax:
                puts("Syntax Error");
                break;
        }

    }
}
