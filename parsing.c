#include "mpc.h"
#include <stdio.h>
#include <stdlib.h>

/* If we are compiling on Windows */
#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Fake readline function */
char* readline(char* prompt) {
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char* cpy = malloc(strlen(buffer)+1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy)-1] = '\0';
    return cpy;
}

/* Fake add_history function */
void add_history(char* unused) {}

/* Otherwise include the editline headers */
#else
#include <editline/readline.h>
#endif

/* declare a buffer for user input of size 2048 */
int main(int argc, char** argv) {

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");

    mpca_lang(MPCA_LANG_DEFAULT,
      "                                                    \
        number   : /-?[0-9]+/ ;                            \
        operator : '+' | '-' | '*' | '/' ;                 \
        expr     : <number> | '(' <operator> <expr>+ ')' ; \
        lispy    : /^/ <operator> <expr>+ /$/ ;            \
      ",
      Number, Operator, Expr, Lispy);

    /* Print version and exit information */
    puts("Lispy Version 0.0.0.0.i");
    puts("Copywrite 2020");
    puts("Press Ctrl+c to Exit\n");

    /* In a never ending loop */
    while (1) {

        /* Output our prompt */
        char * input = readline("lispy> ");

        /* Read a line of user input of maximum size 2048 */
        add_history(input);
        
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r)) {
            /* On Success print and delete the AST */
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        } else {
            /* Otherwise print and delete the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
        
        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}

