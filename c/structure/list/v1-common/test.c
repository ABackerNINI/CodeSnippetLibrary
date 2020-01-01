#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"

void list_foreach_print_int(void **data) { printf("%d ", *(int *)*data); }
void list_elem_destory(void **data) { free(*data); }

void list_push_back_int(List l, int val) {
    int *p;

    p = (int *)malloc(sizeof(int));
    *p = val;
    list_push_back(l, p);
}

void list_push_front_int(List l, int val) {
    int *p;

    p = (int *)malloc(sizeof(int));
    *p = val;
    list_push_front(l, p);
}

void test_1() {
    int i, *val;
    list_t l;
    list_init(&l);
    for (i = 0; i < 100; ++i) {
        val = (int *)malloc(sizeof(int));
        *val = i;
        list_push_back(&l, val);
    }
    list_foreach(&l, list_foreach_print_int);
    printf("\n");
    LIST_FOREACH(&l, iter) { free(list_data(iter)); }
    list_destory(&l);
}

void test_2() {
    int i, *val;
    list_t l;
    list_init(&l);
    for (i = 0; i < 100; ++i) {
        val = (int *)malloc(sizeof(int));
        *val = i;
        list_push_back(&l, val);
    }
    list_foreach(&l, list_foreach_print_int);
    printf("\n");
    list_foreach(&l, list_elem_destory);
    list_destory(&l);
}

void test_3() {
    int i, *val;
    list_t l;
    list_init(&l);
    for (i = 0; i < 100; ++i) {
        val = (int *)malloc(sizeof(int));
        *val = i;
        list_push_back(&l, val);
    }
    list_foreach(&l, list_foreach_print_int);
    printf("\n");
    list_destory2(&l, list_elem_destory);
}

void test_4() {
    int arr[] = {3, 4, 6, 1, 2, 5, 9, 0};

    int i;
    list_t l;
    list_init(&l);
    list_push_back_int(&l, 2);
    list_push_back_int(&l, 5);
    list_push_back_int(&l, 9);
    list_push_front_int(&l, 1);
    list_push_front_int(&l, 6);
    list_push_front_int(&l, 4);
    list_push_back_int(&l, 0);
    list_push_front_int(&l, 3);

    i = 0;
    LIST_FOREACH(&l, iter) { assert(arr[i++] == *(int *)list_data(iter)); }

    list_destory2(&l, list_elem_destory);
}

int main() {
    test_1();
    test_2();
    test_3();
    test_4();

    return 0;
}
