#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "dlist.h"

void dlist_foreach_print_int(DListElemType *data) {
    printf("%d ", *(int *)*data);
}
void dlist_elem_destory(DListElemType *data) { free(*data); }

void dlist_push_back_int(DList l, int val) {
    int *p;

    p = (int *)malloc(sizeof(int));
    *p = val;
    dlist_push_back(l, p);
}

void dlist_push_front_int(DList l, int val) {
    int *p;

    p = (int *)malloc(sizeof(int));
    *p = val;
    dlist_push_front(l, p);
}

void test_1() {
    int i, *val;
    dlist_t l;
    dlist_init(&l);
    for (i = 0; i < 100; ++i) {
        val = (int *)malloc(sizeof(int));
        *val = i;
        dlist_push_back(&l, val);
    }
    dlist_foreach(&l, dlist_foreach_print_int);
    printf("\n");
    // DLIST_FOREACH(&l, iter) { free(dlist_data(iter)); }
    dlist_destory(&l);
}

void test_2() {
    int i, *val;
    dlist_t l;
    dlist_init(&l);
    for (i = 0; i < 100; ++i) {
        val = (int *)malloc(sizeof(int));
        *val = i;
        dlist_push_back(&l, val);
    }
    dlist_foreach(&l, dlist_foreach_print_int);
    printf("\n");
    dlist_foreach(&l, dlist_elem_destory);
    dlist_destory(&l);
}

void test_3() {
    int i, *val;
    dlist_t l;
    dlist_init(&l);
    for (i = 0; i < 100; ++i) {
        val = (int *)malloc(sizeof(int));
        *val = i;
        dlist_push_back(&l, val);
    }
    dlist_foreach(&l, dlist_foreach_print_int);
    printf("\n");
    dlist_destory2(&l, dlist_elem_destory);
}

void test_4() {
    int arr[] = {3, 4, 6, 1, 2, 5, 9, 0};

    int i;
    dlist_t l;
    dlist_init(&l);
    dlist_push_back_int(&l, 2);
    dlist_push_back_int(&l, 5);
    dlist_push_back_int(&l, 9);
    dlist_push_front_int(&l, 1);
    dlist_push_front_int(&l, 6);
    dlist_push_front_int(&l, 4);
    dlist_push_back_int(&l, 0);
    dlist_push_front_int(&l, 3);

    i = 0;
    // DLIST_FOREACH(&l, iter) { assert(arr[i++] == *(int *)dlist_data(iter)); }

    dlist_destory2(&l, dlist_elem_destory);
}

int main() {
    test_1();
    test_2();
    test_3();
    test_4();

    return 0;
}
