#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdbool.h>

typedef void *ListElemType;

typedef void (*list_elem_destory_func)(ListElemType *);
typedef void (*list_foreach_func)(ListElemType *);
typedef void (*list_foreach_index_func)(ListElemType *, size_t);

typedef struct _list_node_t_ {
    ListElemType data;
    struct _list_node_t_ *next;
} list_node_t;

typedef list_node_t *ListNode;
typedef list_node_t *list_iterator;

typedef struct {
    ListNode first;
    ListNode last;
    size_t size;
} list_t;

typedef list_t *List;

void list_init(List l);
void list_destory(List l);
void list_destory2(List l, list_elem_destory_func func);

void list_push_front(List l, ListElemType val);
void list_push_back(List l, ListElemType val);
void list_pop_front(List l);
void list_pop_front2(List l, list_elem_destory_func func);
/* Bad choice, 'dlist' (double linked list) is recommended if you have to use
 * 'pop_back' */
void list_pop_back(List l);
/* Bad choice, 'dlist' (double linked list) is recommended if you have to use
 * 'pop_back' */
void list_pop_back2(List l, list_elem_destory_func func);

ListElemType list_front(List l);
ListElemType list_back(List l);
ListElemType *list_front_ref(List l);
ListElemType *list_back_ref(List l);
size_t list_size(List l);
bool list_empty(List l);

list_iterator list_begin(List l);
list_iterator list_end(/* List l */);
ListElemType list_data(list_iterator iter);
ListElemType *list_data_ref(list_iterator iter);
list_iterator list_next(list_iterator iter);

void list_foreach(List l, list_foreach_func func);
void list_foreach_index(List l, list_foreach_index_func func);

#define _LIST_FOREACH(list, iter_name)                          \
    for (iter_name = list_begin(list); iter_name != list_end(); \
         iter_name = list_next(iter_name))
#define LIST_FOREACH(list, iter_name) \
    ListNode iter_name;               \
    _LIST_FOREACH(list, iter_name)

#endif  //_LIST_H_