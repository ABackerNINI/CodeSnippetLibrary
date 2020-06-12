/** File: getopt.c
 *  Tags: c,else,linux,getopt,parse_args;
 *
 *  2020/06/12
 *
 *  Compile with:
 *      gcc getopt.c -W -Wall -o getopt.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>

/*===========================================================================*/

/**
 * @brief Parse arguments using getopt().
 *
 * @return
 *      @retval index to the first non-options.
 */
int parse_args(int argc, char **argv) {
    /* option 'a' has no argument,
        'b' has mandatory argument,
        'c' has optional argument.
     */
    const char *optstr = "ab:c::";

    int i, ch;

    opterr = 0; /* if set to 1, error msg will be printed to stderr */
    while ((ch = getopt(argc, argv, optstr)) != -1) {
        switch (ch) {
            case 'a':
                printf("option: -a, arg: |%s|\n", optarg);
                break;
            case 'b':
                printf("option: -b, arg: |%s|\n", optarg);
                break;
            case 'c':
                printf("option: -c, arg: |%s|\n", optarg);
                break;
            default:
                if (optopt == 'a' || optopt == 'b' || optopt == 'c') {
                    fprintf(stderr, "missing argument option: %c\n",
                            (char)optopt);
                } else {
                    fprintf(stderr, "unknown option: %c\n", (char)optopt);
                }
                break;
        }
    }

    /* getopt will change the order of argv, non-options will be put behind */
    printf("Remain argv:\n");
    for (i = optind; i < argc; ++i) {
        printf("|%s|\n", argv[i]);
    }

    return optind;
}

/*===========================================================================*/

void test_1(int argc, char **argv) { parse_args(argc, argv); }

/*===========================================================================*/

int main(int argc, char **argv) {
    test_1(argc, argv);

    return 0;
}
