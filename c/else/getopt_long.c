/** File: getopt_long.c
 *  Tags: c,else,linux,getopt_long,parse_args;
 *
 *  2020/06/12
 *
 *  Compile with:
 *      gcc getopt_long.c -W -Wall -o getopt_long.out
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
 * @brief Parse arguments using getopt_long().
 *
 * @return
 *      @retval 0 if no errors.
 *      @retval -1 if error.
 */
int parse_args(int argc, char **argv) {
    /* short options:
        'a' has no argument,
        'b' has mandatory argument,
        'c' has optional argument.
     */
    const char *optstr = "ab:c::";
    /* long options */
    const struct option long_opts[15] = {{"a_long", no_argument, NULL, 'a'},
                                         {"b_long", no_argument, NULL, 'b'},
                                         {"c_long", no_argument, NULL, 'c'},
                                         {"d_long", no_argument, NULL, 'd'},
                                         {NULL, 0, NULL, 0}};

    int i, ch, long_ind;

    opterr = 0; /* if set to 1, error msg will be printed to stderr */
    while ((ch = getopt_long(argc, argv, optstr, long_opts, &long_ind)) != -1) {
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
            case 'd':
                printf("option: -d, arg: |%s|\n", optarg);
                break;
            default:
                if (optopt == 'a' || optopt == 'b' || optopt == 'c' ||
                    optopt == 'd') {
                    fprintf(stderr, "missing argument option: %c\n",
                            (char)optopt);
                } else {
                    fprintf(stderr, "unknown option: %c\n", (char)optopt);
                }
                break;
        }
    }

    /* getopt_long will change the order of argv, non-options will be put behind
     */
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
