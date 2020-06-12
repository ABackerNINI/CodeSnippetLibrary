/** File: tokenize.c
 *  Tags: c,string,string processing,tokenize,shell
 *
 *  2020/04/03
 *
 *  Compile with: gcc tokenize.c -W -Wall -o tt.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*===========================================================================*/

/**
 * @brief Tokenize the given string into tokens. If tokens is NULL, tokens will
 * be alloced by capacity. If capacity is 0, the tokens will be alloced by
 * needed.
 *
 * @return
 *      @retval number of tokens if success.
 *      @retval -1 quotes doesn't match.
 *      @retval -2 error when malloc or realloc.
 */
int tokenize(char* s, char*** tokens, int capacity) {
    if (*tokens == NULL) {
        if (capacity == 0) {
            capacity = 16;
        }
        *tokens = (char**)malloc(sizeof(char*) * capacity);
        if (*tokens == NULL) {
            perror("malloc");
            return -2;
        }
    }
    int n_tokens = 0;

    char* p;
    for (p = s; *p; ++p) {
        // skip leading spaces
        while (isspace(*p)) {
            ++p;
        }

        if (*p == '\0') {
            break;
        }

        // increase capacity if needed, reserve 1 for NULL
        if (n_tokens == capacity - 1) {
            capacity += capacity / 2;
            char** new_tokens =
                (char**)realloc(*tokens, sizeof(char*) * capacity);
            if (new_tokens == NULL) {
                perror("realloc");
                return -2;
            } else {
                *tokens = new_tokens;
            }
        }

        if (*p == '\'' || *p == '"') {
            char quote = *p;
            *(p++) = '\0';
            (*tokens)[n_tokens++] = p;
            while (*p && *p != quote) {
                ++p;
            }
            if (*p == quote) {
                *(p++) = '\0';
            } else {
                // error
                return -1;  // quotes doesn't match
            }
        } else {
            (*tokens)[n_tokens++] = p;
            while (*p && !isspace(*p) && !(*p == '\'' || *p == '"')) {
                ++p;
            }
            if (isspace(*p)) {
                *p = '\0';
            }
        }
    }
    (*tokens)[n_tokens] = NULL;

    return n_tokens;
}

/*===========================================================================*/

int check(const char* _s, int expected_n_tokens) {
    char* s = strdup(_s);
    char** tokens = NULL;
    int n_tokens = tokenize(s, &tokens, 0);
    if (n_tokens > 0) {
        int i;
        for (i = 0; i < n_tokens; ++i) {
            printf("%d: |%s|\n", i + 1, tokens[i]);
        }
        assert(tokens[i] == NULL);
    }
    free(tokens);
    free(s);
    return n_tokens == expected_n_tokens;
}

void test_tokenize1() {
    assert(check("ls ", 1));
    assert(check("ls \n ", 1));
    assert(check(" ls \n ", 1));
    assert(check(" \t ls \n ", 1));
    assert(check(" \t ls \n", 1));

    assert(check("ls ' xx xx xx ' \" xx x xx\" xx x  x ", 6));
}

void test_tokenize2() {
    char s[BUFSIZ];
    char** tokens = NULL;
    while (fgets(s, BUFSIZ, stdin)) {
        int n_tokens = tokenize(s, &tokens, 0);
        printf("%d\n", n_tokens);
        if (n_tokens > 0) {
            int i;
            for (i = 0; i < n_tokens; ++i) {
                printf("%d: |%s|\n", i + 1, tokens[i]);
            }
            assert(tokens[i] == NULL);
        }
        free(tokens);
    }
}

/*===========================================================================*/

int main() {
    test_tokenize1();
    test_tokenize2();
    return 0;
}