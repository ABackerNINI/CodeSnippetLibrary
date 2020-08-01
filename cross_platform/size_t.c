#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN64
#define ZU "%I64u"
#elif defined(_WIN32)
#define ZU "%I32u"
#else
#define ZU "%zu"
#endif

int main(int argc, char **argv) {
    size_t a = 1;
    printf("a: " ZU "\n", a);

    return 0;
}
