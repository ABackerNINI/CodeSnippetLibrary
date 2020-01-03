#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdbool.h>

#define _LIST_HIDE_NODE_TYPE 1
#define _LIST_HIDE_LIST_TYPE 0
#define _LIST_ENABLE_UDEDF 1 /* USER_DEF_ELEM_DESTORY_FUNC */
#define _LIST_ENABLE_ITERATOR 1

/***** LIST ELEMENT TYPE *****/

typedef void *ListElemType;

/***** LIST ELEMENT DESTORY FUNC TYPE *****/

#if (_LIST_ENABLE_UDEDF)
typedef void (*list_elem_destory_func)(ListElemType *);
#endif

/***** LIST NODE TYPE *****/

#if (_LIST_HIDE_NODE_TYPE == 0)
typedef struct _list_node_t_ {
    ListElemType data;
    struct _list_node_t_ *next;
} list_node_t;
#else
struct _list_node_t_;
typedef struct _list_node_t_ list_node_t;
#endif

typedef list_node_t *ListNode;

/***** LIST TYPE *****/

#if (_LIST_HIDE_LIST_TYPE == 0)
typedef struct _list_t_ {
    ListNode first;
    ListNode last;
    size_t size;
} list_t;
#else
struct _list_t_;
typedef struct _list_t_ list_t;
#endif

typedef list_t *List;

/***** ELEMENTS COMPARE FUNC TYPE *****/

typedef int (*list_elem_cmp_func)(ListElemType *, ListElemType *);

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

/* @Return a pointer of an initialized List, call list_delete() to clean up. */
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
/* Append the List 'source' to the back of the List 'dest'.
 * All elements in 'source' will be removed after merging. */
void list_merge(List dest, List source);
/* Sort the List 'l' in decsending order.
 * @Time complexity: O(n^2) */
void list_sort(List l, list_elem_cmp_func func);
/* Insert element 'val' into the List 'l', and keep the List in decsending
 * order.
 * @Return the position that the element inserted into.
 * @Note: the new element will be inserted before elements equal to it.
 * @See also: list_sort_insert2(), list_sort_insert_single().
 */
size_t list_sort_insert(List l, ListElemType val, list_elem_cmp_func func);
/* Insert element 'val' into the List 'l', and keep the List in decsending
 * order.
 * @Return the position that the element inserted into.
 * @Note: the new element will be inserted after elements equal to it.
 * @See also: list_sort_insert(), list_sort_insert_single().
 */
size_t list_sort_insert2(List l, ListElemType val, list_elem_cmp_func func);
/* Merge the List 'source' to the List 'dest' and keep it in decsending order.
 * All elements in 'source' will be removed after merging.
 * @Note: the two Lists should be sorted in decsending order before calling this
 * function.
 * @Time complexity: O(n1+n2). */
void list_sort_merge(List dest, List source, list_elem_cmp_func func);

/***** ACCESS FUNC *****/

/* @Return the first element in the List 'l'. */
ListElemType list_front(List l);
/* @Return the last element in the List 'l'. */
ListElemType list_back(List l);
/* @Return a pointer of the first element in the List 'l'. */
ListElemType *list_front_ref(List l);
/* @Return a pointer of the last element in the List 'l'. */
ListElemType *list_back_ref(List l);
/* @Return number of elements in List 'l'. */
size_t list_size(List l);
/* @Return:
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
#if (_LIST_HIDE_NODE_TYPE == 0 && _LIST_ENABLE_ITERATOR == 0)
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
/* @Return iterator of the first element in the List 'l'. */
list_iterator list_begin(List l);
/* @Return iterator indicates the end of List. */
list_iterator list_end(/* List l */);
/* @Return data of the iterator 'iter'. */
ListElemType list_data(list_iterator iter);
/* @Return a pointer of the data of the iterator 'iter'. */
ListElemType *list_data_ref(list_iterator iter);
/* @Return iterator points to the next of iterator the iterator 'iter'. */
list_iterator list_next(list_iterator iter);
/* @Return:
 * --- iterator points to the first element equals to 'val' in the List 'l': if
 * found.
 * --- NULL: if not found.
 * @See also: list_find2(). */
list_iterator list_find(List l, ListElemType val, list_elem_cmp_func func);
/* @Return:
 * --- iterator points to the first element equals to 'val' in the List at or
 * after 'iter': if found.
 * --- NULL: if not found.
 * @See also: list_find(). */
list_iterator list_find2(list_iterator iter, ListElemType val,
                         list_elem_cmp_func func);
/* Swap two elements in the List.
 * @Note: since this is single-linked list, this function swaps two elements
 * instead of swaping two nodes. You may use dlist if the size of elem is too
 * large. Function list_swap2()/list_swap_pos() do swap nodes but it is not time
 * efficient.
 * @See also: list_swap2(), list_swap_pos(). */
void list_swap(list_iterator iter1, list_iterator iter2);
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
 * @!!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'pop_back' frequently.
 * @Time complexity: O(n). */
void list_pop_back(List l);
/* Insert one element into the List at position 'pos'.
 * @!!!Bad choice!!! Use list_insert_after() instead if list iterator is
 * enabled.
 * @Time complexity: O(pos). */
void list_insert(List l, size_t pos, ListElemType val);
/* @Return the element at position 'pos' in the List 'l'.
 * @!!!Bad choice!!! Use list iterator instead if list iterator is enabled.
 * @Time complexity: O(pos). */
ListElemType list_at(List l, size_t pos);
/* @Return pointer of the element at position 'pos' in the List 'l'.
 * @!!!Bad choice!!! Use list iterator instead if list iterator is enabled.
 * @Time complexity: O(pos). */
ListElemType *list_at_ref(List l, size_t pos);
/* Swap two elements in the List 'l'.
 * @!!!Bad choice!!! Use list_swap() instead if list iterator is enabled.
 * @Time complexity: O(pos1+pos2). */
void list_swap_pos(List l, size_t pos1, size_t pos2);
#if (_LIST_ENABLE_ITERATOR)
/* Swap two elements in the List 'l'.
 * @!!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'list_swap2' frequently.
 * @Time complexity: O(pos(iter1)+pos(iter2)). */
void list_swap2(List l, list_iterator iter1, list_iterator iter2);
/* Insert one element into the List 'l' before 'iter'.
 * If 'iter' is `NULL`, insert the element to the back of the List.
 * @!!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'insert_before' frequently.
 * @Time complexity: O(pos(iter)). */
void list_insert_before(List l, list_iterator iter, ListElemType val);
#endif
#if (_LIST_ENABLE_UDEDF)
/* Pop one element from the back of the List 'l'.
 * @!!!Bad choice!!! 'dlist' (double linked list) is recommended if you have to
 * use 'pop_back' frequently.
 * @Time complexity: O(n). */
void list_pop_back2(List l, list_elem_destory_func func);
#endif

#endif  //_LIST_H_
