/** File: list.c
 *  Tags: c,structure,list
 *
 *  2019/9/13
 *
 *  Compile with: gcc list.c -W -Wall -Werror -o list.out
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*===========================================================================*/

typedef int DATA_TYPE;

typedef struct _list_node {
    DATA_TYPE data;
    struct _list_node *next;
} list_node;

typedef list_node *list_iterator;

typedef struct _list {
    list_node *first;
    list_node *tail;
    size_t size;
} list;

void list_init(list *l);
void list_destory(list *l);
void list_push_front(list *l, DATA_TYPE data);
void list_push_back(list *l, DATA_TYPE data);
void list_pop_front(list *l);
/* Bad choice, 'deque' is recommended if have to use 'pop_back' */
/* void list_pop_back(list *l); */
DATA_TYPE list_front(list *l);
DATA_TYPE list_back(list *l);
DATA_TYPE *list_front_ref(list *l);
DATA_TYPE *list_back_ref(list *l);
size_t list_size(list *l);
int list_empty(list *l);
list_iterator list_iter_begin(list *l);
list_iterator list_iter_end(/* list *l */);
DATA_TYPE list_iter_data(list_iterator iter);
DATA_TYPE *list_iter_data_ref(list_iterator iter);
list_iterator list_iter_next(list_iterator iter);

/*===========================================================================*/

void list_init(list *l) {
    l->first = NULL;
    l->size = 0;
}

void list_destory(list *l) {
    list_node *node, *tmp;

    node = l->first;
    while (node) {
        tmp = node->next;
        free(node);
        node = tmp;
    }
}

void list_push_front(list *l, DATA_TYPE data) {
    list_node *new = (list_node *)malloc(sizeof(list_node));
    new->data = data;
    new->next = l->first;

    if (l->first == NULL) {
        l->first = l->tail = new;
    } else {
        l->first = new;
    }
    ++l->size;
}

void list_push_back(list *l, DATA_TYPE data) {
    list_node *new = (list_node *)malloc(sizeof(list_node));
    new->data = data;
    new->next = NULL;

    if (l->first == NULL) {
        l->first = l->tail = new;
    } else {
        l->tail->next = new;
        l->tail = new;
    }
    ++l->size;
}

void list_pop_front(list *l) {
    list_node *tmp = l->first;
    l->first = l->first->next;
    free(tmp);
    --l->size;
}

/* void list_pop_back(list *l){} */

DATA_TYPE list_front(list *l) { return l->first->data; }

DATA_TYPE list_back(list *l) { return l->tail->data; }

DATA_TYPE *list_front_ref(list *l) { return &l->first->data; }

DATA_TYPE *list_back_ref(list *l) { return &l->tail->data; }

size_t list_size(list *l) { return l->size; }

int list_empty(list *l) { return l->size == 0; }

list_iterator list_iter_begin(list *l) { return l->first; }

list_iterator list_iter_end(/* list *l */) { return NULL; }

DATA_TYPE list_iter_data(list_iterator iter) { return iter->data; }

DATA_TYPE *list_iter_data_ref(list_iterator iter) { return &iter->data; }

list_iterator list_iter_next(list_iterator iter) { return iter->next; }

/*===========================================================================*/

void test_list_print(list *l) {
    int first;
    list_iterator iter;

    printf("List:\n");
    first = 1;
    for (iter = list_iter_begin(l); iter != list_iter_end(l);
         iter = list_iter_next(iter)) {
        if (!first) {
            printf("->");
        } else {
            first = 0;
        }
        printf("%d", *list_iter_data_ref(iter));
    }
    printf("\n");
}

void test_list() {
    list l;
    list_init(&l);

    list_push_back(&l, 1);
    list_push_back(&l, 2);
    list_push_back(&l, 3);
    assert(list_size(&l) == 3);
    test_list_print(&l);

    list_push_front(&l, 4);
    list_push_front(&l, 5);
    list_push_front(&l, 6);
    assert(list_size(&l) == 6);
    test_list_print(&l);

    list_pop_front(&l);
    assert(list_size(&l) == 5);
    test_list_print(&l);

    list_pop_front(&l);
    assert(list_size(&l) == 4);
    test_list_print(&l);

    while (list_size(&l)) {
        list_pop_front(&l);
    }
    test_list_print(&l);

    list_destory(&l);
}

void test_list_iterator() {
    int i;
    list l;
    list_iterator iter;

    list_init(&l);

    for (i = 0; i < 1000; ++i) {
        list_push_back(&l, i);
    }

    for (i = 0, iter = list_iter_begin(&l); iter != list_iter_end();
         ++i, iter = list_iter_next(iter)) {
        assert(i == list_iter_data(iter));
        // printf("%d\n", *list_iter_data_ref(iter));
    }
    list_destory(&l);
}

int main() {
    test_list();
    test_list_iterator();

    return 0;
}
