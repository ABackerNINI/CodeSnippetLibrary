/** File: to_lower,to_upper.c
 *  Tags: c,string,string processing,to_lower,to_upper
 *
 *  2019/9/13
 *
 *  Compile with: gcc to_lower,to_upper.c -Wall -Werror -o tt.out
 */

#include <stdio.h>
#include <ctype.h> /* tolower,toupper */
#include <assert.h>

/* Note:
        Functions 'tolower' and 'toupper' is implemented in 'ctype.h'.
*/

/*===========================================================================*/

/* Convert 'c' to lowercase. */
char my_to_lower(char c);

/* Convert 'c' to uppercase. */
char my_to_upper(char c);

/*===========================================================================*/

char my_to_lower(char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }

char my_to_upper(char c) { return (c >= 'a' && c <= 'z') ? c - 32 : c; }

/*===========================================================================*/

void test_to_lower() {
    char c;
    for (c = 'A'; c <= 'Z'; ++c) {
        assert(tolower(c) == my_to_lower(c));
    }
    for (c = 'a'; c <= 'z'; ++c) {
        assert(tolower(c) == my_to_lower(c));
    }
    for (c = (char)1; c <= (char)255; ++c) {
        assert(tolower(c) == my_to_lower(c));
    }
}

void test_to_upper() {
    char c;
    for (c = 'A'; c <= 'Z'; ++c) {
        assert(toupper(c) == my_to_upper(c));
    }
    for (c = 'a'; c <= 'z'; ++c) {
        assert(toupper(c) == my_to_upper(c));
    }
    for (c = (char)0; c <= (char)255; ++c) {
        assert(toupper(c) == my_to_upper(c));
    }
}

int main() {
    test_to_lower();
    test_to_upper();

    return 0;
}