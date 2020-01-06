#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define _LIST_ENABLE_UDEDF 1 /* USER_DEF_ELEM_DESTORY_FUNC */
#define _LIST_ENABLE_ITERATOR 1

/***** LIST ELEMENT TYPE *****/

// typedef void *ListElemType;
/* Use define rather than typedef here so that you can define it for each object
 * file by adding `-DListElemType='type'` to your compiler flags. */
#ifndef ListElemType
#define ListElemType void *
#endif

/***** LIST ELEMENT DESTORY FUNC TYPE *****/

#if (_LIST_ENABLE_UDEDF)
typedef void (*list_elem_destory_func)(ListElemType *);
#endif

/***** LIST NODE TYPE *****/

typedef struct _list_node_t_ {
    ListElemType data;
    struct _list_node_t_ *next;
} list_node_t;

typedef list_node_t *ListNode;

/***** LIST TYPE *****/

typedef struct _list_t_ {
    ListNode first;
    ListNode last;
    size_t size;
} list_t;

typedef list_t *List;

/***** INSERT COMPARE FUNC TYPE *****/

typedef int (*list_insert_cmp_func)(ListElemType *, ListElemType *);

/***** FOREACH FUNC TYPE *****/

typedef void (*list_foreach_func)(ListElemType *);
typedef void (*list_foreach_index_func)(ListElemType *, size_t);
typedef void (*list_foreach_extra_func)(ListElemType *, size_t, void *);

/***** ITERATOR TYPE *****/

#if (_LIST_ENABLE_ITERATOR)
typedef list_node_t *list_iterator;
#endif

/* ======================================================================== */

/***** INIT-DESTORY FUNC *****/

/* Initialize the List, call list_destory() to clean up. */
void list_init(List l);
/* Destory the List 'l', free all nodes. */
void list_destory(List l);

/***** NEW-DELETE FUNC *****/

/* Return an initialized List, call list_delete() to clean up. */
List list_new();
/* Destory the List 'l', free all nodes and free the memory of the List. */
void list_delete(List l);

/***** MODIFIER FUNC *****/

/* Push element 'val' to the front of the List 'l'. */
void list_push_front(List l, ListElemType val);
/* Push element 'val' to the back of the List 'l'. */
void list_push_back(List l, ListElemType val);
/* Pop one element from the front of the List 'l'. */
void list_pop_front(List l);
/* Insert element 'val' into the List 'l', and keep the List in decsending
 * order.
 * Return the position that the element inserted into.
 */
size_t list_sort_insert(List l, ListElemType val, list_insert_cmp_func func);

/***** ACCESS FUNC *****/

/* Return the first element in the List 'l'. */
ListElemType list_front(List l);
/* Return the last element in the List 'l'. */
ListElemType list_back(List l);
/* Return a pointer of the first element in the List 'l'. */
ListElemType *list_front_ref(List l);
/* Return a pointer of the last element in the List 'l'. */
ListElemType *list_back_ref(List l);
/* Return number of elements in List 'l'. */
size_t list_size(List l);
/* Return:
    --- true: if List 'l' is empty.
    --- false: if not. */
bool list_empty(List l);
/* For each element in List 'l', call 'func' with element. */
void list_foreach(List l, list_foreach_func func);
/* For each element in List 'l', call 'func' with element and its index. */
void list_foreach_index(List l, list_foreach_index_func func);
/* For each element in List 'l', call 'func' with element, its index and
 * 'extra'. */
void list_foreach_extra(List l, list_foreach_extra_func func, void *extra);
#if (_LIST_ENABLE_ITERATOR == 0)
/* Macro version of list_foreach().
 * Some version of C compilers force all variables putting at the front of a
 * function. */
#define _LIST_FOREACH(list, node_name)                 \
    for (node_name = (list)->first; node_name != NULL; \
         node_name = node_name->next)
/* Macro version of list_foreach(). */
#define LIST_FOREACH(list, node_name) \
    ListNode node_name;               \
    _LIST_FOREACH(list, node_name)
#endif

/***** ITERATOR FUNC *****/

#if (_LIST_ENABLE_ITERATOR)
/* Return iterator of the first element in the List 'l'. */
list_iterator list_begin(List l);
/* Return iterator indicates the end of List. */
list_iterator list_end(/* List l */);
/* Return data of the iterator 'iter'. */
ListElemType list_data(list_iterator iter);
/* Return a pointer of the data of the iterator 'iter'. */
ListElemType *list_data_ref(list_iterator iter);
/* Return iterator points to the next of iterator the iterator 'iter'. */
list_iterator list_next(list_iterator iter);
/* Insert one element into the List after 'iter'.
 * If 'iter' is `NULL`, insert the element to the front of the List. */
void list_insert_after(List l, list_iterator iter, ListElemType val);
#ifndef _LIST_FOREACH
/* Macro version of list_foreach().
 * Some version of C compilers force all variables putting at the front of a
 * function. */
#define _LIST_FOREACH(list, iter_name)                          \
    for (iter_name = list_begin(list); iter_name != list_end(); \
         iter_name = list_next(iter_name))
/* Macro version of list_foreach() */
#define LIST_FOREACH(list, iter_name) \
    ListNode iter_name;               \
    _LIST_FOREACH(list, iter_name)
#endif
#endif

/***** UDEDF FUNC *****/

#if (_LIST_ENABLE_UDEDF)
/* Destory the List 'l', free all nodes. */
void list_destory2(List l, list_elem_destory_func func);

/* Destory the List 'l', free all nodes and free the memory of the List. */
void list_delete2(List l, list_elem_destory_func func);

/* Pop one element from the front of the List 'l'. */
void list_pop_front2(List l, list_elem_destory_func func);
#endif

/***** NOT-RECOMMENDED FUNC *****/

/* Pop one element from the back of the List 'l'.
 * !!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'pop_back' frequently. */
void list_pop_back(List l);
/* Insert one element into the List at position 'pos'.
 * !!!Bad choice!!! Use list_insert_after() instead if list iterator is enabled.
 */
void list_insert(List l, size_t pos, ListElemType val);
/* Return the element at position 'pos' in the List 'l'.
 * !!!Bad choice!!! Use list iterator instead if list iterator is enabled. */
ListElemType list_at(List l, size_t pos);

/* Return pointer of the element at position 'pos' in the List 'l'.
 * !!!Bad choice!!! Use list iterator instead if list iterator is enabled. */
ListElemType *list_at_ref(List l, size_t pos);
#if (_LIST_ENABLE_ITERATOR)
/* Insert one element into the List 'l' before 'iter'.
 * If 'iter' is `NULL`, insert the element to the back of the List.
 * !!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'insert_before'. */
void list_insert_before(List l, list_iterator iter, ListElemType val);
#endif
#if (_LIST_ENABLE_UDEDF)
/* Pop one element from the back of the List 'l'.
 * !!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'pop_back'. */
void list_pop_back2(List l, list_elem_destory_func func);
#endif

/* ======================================================================== */

/***** INIT-DESTORY FUNC *****/

inline void list_init(List l) {
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
}

inline void list_destory(List l) {
    ListNode node, next;

    node = l->first;
    while (node) {
        next = node->next;
        free(node);
        node = next;
    }
}

/***** NEW-DELETE FUNC *****/

inline List list_new() {
    List l;

    l = (List)malloc(sizeof(list_t));
    list_init(l);

    return l;
}

inline void list_delete(List l) {
    list_destory(l);
    free(l);
}

/***** MODIFIER FUNC *****/

inline void list_push_front(List l, ListElemType val) {
    ListNode node;

    node = (ListNode)malloc(sizeof(list_node_t));
    node->data = val;
    node->next = l->first;

    if (l->first == NULL) {
        l->first = l->last = node;
    } else {
        l->first = node;
    }
    ++l->size;
}

inline void list_push_back(List l, ListElemType val) {
    ListNode node;

    node = (ListNode)malloc(sizeof(list_node_t));
    node->data = val;
    node->next = NULL;

    if (l->first == NULL) {
        l->first = l->last = node;
    } else {
        l->last->next = node;
        l->last = node;
    }
    ++l->size;
}

inline void list_pop_front(List l) {
    ListNode node;

    node = l->first;
    l->first = l->first->next;
    if (l->first == NULL) {
        l->last = NULL;
    }
    free(node);
    --l->size;
}

inline size_t list_sort_insert(List l, ListElemType val,
                               list_insert_cmp_func func) {
    size_t pos;
    ListNode node, new_node;

    if (l->first == NULL || func(&l->first->data, &val) <= 0) {
        list_push_front(l, val);
        return 0;
    } else if (func(&l->last->data, &val) > 0) {
        list_push_back(l, val);
        return l->size - 1;
    }

    new_node = (ListNode)malloc(sizeof(list_node_t));
    new_node->data = val;

    pos = 1;
    node = l->first;
    while (node->next) {
        if (func(&node->next->data, &val) <= 0) {
            new_node->next = node->next;
            node->next = new_node;
            ++l->size;
            return pos;
        }
        ++pos;
        node = node->next;
    }

    assert(0); /* Control should never reach hear */
    return l->size + 1;
}

/***** ACCESS FUNC *****/

inline ListElemType list_front(List l) { return l->first->data; }

inline ListElemType list_back(List l) { return l->last->data; }

inline ListElemType *list_front_ref(List l) { return &l->first->data; }

inline ListElemType *list_back_ref(List l) { return &l->last->data; }

inline size_t list_size(List l) { return l->size; }

inline bool list_empty(List l) { return list_size(l) == 0; }

inline void list_foreach(List l, list_foreach_func func) {
    ListNode node;

    node = l->first;
    while (node) {
        func(&node->data);
        node = node->next;
    }
}

inline void list_foreach_index(List l, list_foreach_index_func func) {
    size_t index;
    ListNode node;

    index = 0;
    node = l->first;
    while (node) {
        func(&node->data, index++);
        node = node->next;
    }
}

inline void list_foreach_extra(List l, list_foreach_extra_func func,
                               void *extra) {
    size_t index;
    ListNode node;

    index = 0;
    node = l->first;
    while (node) {
        func(&node->data, index++, extra);
        node = node->next;
    }
}

/***** ITERATOR FUNC *****/

#if (_LIST_ENABLE_ITERATOR)
inline list_iterator list_begin(List l) { return l->first; }

inline list_iterator list_end(/* List l */) { return NULL; }

inline ListElemType list_data(list_iterator iter) { return iter->data; }

inline ListElemType *list_data_ref(list_iterator iter) { return &iter->data; }

inline list_iterator list_next(list_iterator iter) { return iter->next; }

inline void list_insert_after(List l, list_iterator iter, ListElemType val) {
    ListNode new_node;

    if (iter == NULL) {
        list_push_front(l, val);
    } else if (iter == l->last) {
        list_push_back(l, val);
    } else {
        new_node = (ListNode)malloc(sizeof(list_node_t));
        new_node->data = val;
        new_node->next = iter->next;
        iter->next = new_node;
        ++l->size;
    }
}
#endif

/***** UDEDF FUNC *****/

#if (_LIST_ENABLE_UDEDF)
inline void list_destory2(List l, list_elem_destory_func func) {
    ListNode node, next;

    if (func) {
        node = l->first;
        while (node) {
            next = node->next;
            func(&node->data);
            free(node);
            node = next;
        }
    } else {
        node = l->first;
        while (node) {
            next = node->next;
            free(node);
            node = next;
        }
    }
}

inline void list_delete2(List l, list_elem_destory_func func) {
    list_delete2(l, func);
    free(l);
}

inline void list_pop_front2(List l, list_elem_destory_func func) {
    func(&l->first->data);
    list_pop_front(l);
}
#endif

/***** NOT-RECOMMENDED FUNC *****/

inline void list_pop_back(List l) {
    ListNode node;

    if (l->first == l->last) {
        list_pop_front(l);
    } else {
        node = l->first;
        while (node->next != l->last) {
            node = node->next;
        }
        free(l->last);
        node->next = NULL;
        l->last = node;
        --l->size;
    }
}

inline void list_insert(List l, size_t pos, ListElemType val) {
    ListNode node, new_node;

    if (pos == 0) {
        list_push_front(l, val);
    } else if (pos == list_size(l)) {
        list_push_back(l, val);
    } else {
        node = l->first;
        while (--pos) {
            node = node->next;
        }

        new_node = (ListNode)malloc(sizeof(list_node_t));
        new_node->data = val;
        new_node->next = node->next;
        node->next = new_node;
        ++l->size;
    }
}

inline ListElemType list_at(List l, size_t pos) {
    ListNode node;

    node = l->first;
    while (pos--) {
        node = node->next;
    }
    return node->data;
}

inline ListElemType *list_at_ref(List l, size_t pos) {
    ListNode node;

    node = l->first;
    while (pos--) {
        node = node->next;
    }
    return &node->data;
}

#if (_LIST_ENABLE_ITERATOR)
inline void list_insert_before(List l, list_iterator iter, ListElemType val) {
    ListNode node, new_node;

    if (iter == NULL) {
        list_push_back(l, val);
    } else if (iter == l->first) {
        list_push_front(l, val);
    } else {
        new_node = (ListNode)malloc(sizeof(list_node_t));
        new_node->data = val;

        node = l->first;
        while (node->next != iter) {
            node = node->next;
        }

        new_node->next = node->next;
        node->next = new_node;
        ++l->size;
    }
}
#endif

#if (_LIST_ENABLE_UDEDF)
inline void list_pop_back2(List l, list_elem_destory_func func) {
    func(&l->last->data);
    list_pop_back(l);
}
#endif

#endif  //_LIST_H_
