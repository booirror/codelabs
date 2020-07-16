#ifndef _GLIST_H_
#define _GLIST_H_

typedef struct generic_list
{
    int elem_size;
    int length;
    int capacity;
    char *p_data;
} generic_list_t;


#define INITIAL_CAPACITY 8

void __create_generic_list(int elem_size, int initial_capacity, struct generic_list* glist);

#define generic_list_create(type, glist) \
    __create_generic_list(sizeof(type), INITIAL_CAPACITY, glist)

void generic_list_destroy(struct generic_list *glist);

void generic_list_duplicate(struct generic_list *dst, const struct generic_list *src);

void generic_list_push_back(struct generic_list *glist, const void *elem);

void generic_list_pop_back(struct generic_list *glist);

void *generic_list_back(struct generic_list *glist);

void *generic_list_front(struct generic_list *glist);

void generic_list_clear(struct generic_list *glist);

void *generic_list_find(struct generic_list *glist, const void *elem, int(*cmp)(const void*, const void*));

void generic_list_foreach(struct generic_list *glist, void(*func)(const void*));

int generic_list_add(struct generic_list *glist, const void *elem, int(*cmp)(const void*, const void*));

/* Compare function template for POD types */
#define MAKE_COMPARE_FUNCTION(postfix, type) \
    static int __cmp_##postfix (const void *a_, const void *b_)         \
    {                                                                   \
        type a = *((type*) a_);                                         \
        type b = *((type*) b_);                                         \
        if (a < b) return -1;                                           \
        else if (a > b) return 1;                                       \
        else return 0;                                                  \
    }

#endif