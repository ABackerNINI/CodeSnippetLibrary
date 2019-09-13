/** File: strcmp,stricmp.c
 *  Tags: c,string,string processing,strcmp,stricmp,to_lower
 *
 *  2019/9/13
 *
 *  Compile with: gcc strcmp,stricmp.c -W -Wall -Werror -o ss.out
 */

#include <stdio.h>
#include <string.h> /* strcmp */
#include <ctype.h>  /* tolower */
#include <assert.h>

/* Note:
        Function 'stricmp' is implemented in windows.
        In linux, it's 'strcasecmp'.
*/

/*===========================================================================*/

/* Compare two strings. Return 0 if identical */
int my_strcmp(const char *s1, const char *s2);

/* Convert 'c' to lowercase. */
char my_to_lower(char c);

/* Compare two strings and ignore differences in case when comparing. */
int my_stricmp(const char *s1, const char *s2);

/*===========================================================================*/

int my_strcmp(const char *s1, const char *s2) {
    int i;
    for (i = 0; s1[i] && s2[i]; ++i) {
        if (s1[i] != s2[i]) {
            break;
        }
    }
    return s1[i] - s2[i];
}

/*---------------------------------------------------------------------------*/

char my_to_lower(char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }

int my_stricmp(const char *s1, const char *s2) {
    int i;
    for (i = 0; s1[i] && s2[i]; ++i) {
        if (my_to_lower(s1[i]) != my_to_lower(s2[i])) {
            break;
        }
    }
    return my_to_lower(s1[i]) - my_to_lower(s2[i]);
}

/*===========================================================================*/

void test_strcmp() {
    assert(my_strcmp("", "") == 0);
    assert(my_strcmp("a", "") > 0);
    assert(my_strcmp("", "a") < 0);
    assert(my_strcmp("a", "a") == 0);
    assert(my_strcmp("aa", "a") > 0);
    assert(my_strcmp("a", "aa") < 0);
    assert(my_strcmp("abcdef", "abcdef") == 0);
}

void test_to_lower() {
    char c;
    for (c = 'A'; c <= 'Z'; ++c) {
        assert(tolower(c) == my_to_lower(c));
    }
    for (c = 'a'; c <= 'z'; ++c) {
        assert(tolower(c) == my_to_lower(c));
    }
    for (c = (char)0; c <= (char)255; ++c) {
        assert(tolower(c) == my_to_lower(c));
    }
}

void test_stricmp() {
    test_to_lower();

    assert(my_stricmp("", "") == 0);
    assert(my_stricmp("a", "") > 0);
    assert(my_stricmp("", "a") < 0);
    assert(my_stricmp("a", "a") == 0);
    assert(my_stricmp("aa", "a") > 0);
    assert(my_stricmp("a", "aa") < 0);
    assert(my_stricmp("abcdef", "abcdef") == 0);

    assert(my_stricmp("a", "A") == 0);
    assert(my_stricmp("Aa", "a") > 0);
    assert(my_stricmp("A", "aa") < 0);
    assert(my_stricmp("abCdef", "abcdef") == 0);
}

int main() {
    test_strcmp();
    test_stricmp();

    return 0;
}