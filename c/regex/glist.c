#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "glist.h"

void __create_generic_list(int elem_size, int initial_capacity, struct generic_list* glist)
{
    assert(initial_capacity != 0);
    glist->elem_size = elem_size;
    glist->capacity = initial_capacity;
    glist->length = 0;
    glist->p_data = (char*)malloc(elem_size*initial_capacity);
}

void generic_list_destroy(struct generic_list* glist)
{
    free(glist->p_data);
}

void generic_list_duplicate(struct generic_list *dst, const struct generic_list *src)
{
    __create_generic_list(src->elem_size, src->capacity, dst);
    dst->length = src->length;
    memcpy(dst->p_data, src->p_data, src->length * src->elem_size);
}

void generic_list_push_back(struct generic_list *glist, const void *elem)
{
    if (glist->capacity == glist->length)
    {
        glist->capacity *= 2;
        glist->p_data = (char*)realloc(glist->p_data, glist->elem_size*glist->capacity);
    }
    memcpy(glist->p_data + glist->elem_size*glist->length++, elem, glist->elem_size);
}

void generic_list_pop_back(struct generic_list *glist)
{
    assert(glist->length != 0);
    glist->length--;
}

void *generic_list_back(struct generic_list *glist)
{
    assert(glist->length != 0);
    return glist->p_data + glist->elem_size * (glist->length-1);
}

void *generic_list_front(struct generic_list *glist)
{
    assert(glist->length != 0);
    return glist->p_data;
}

void generic_list_clear(struct generic_list *glist)
{
    glist->length = 0;
}

void *generic_list_find(struct generic_list *glist, const void *elem, int(*cmp)(const void*, const void*))
{
    char *cur = glist->p_data;
    for (int i = 0; i < glist->length; i++, cur += glist->elem_size)
    {
        if (cmp((void*)cur, elem) == 0) return cur;
    }
    return NULL;
}

void generic_list_foreach(struct generic_list *glist, void(*func)(const void*))
{
    char *cur = glist->p_data;
    for (int i = 0; i < glist->length; i++, cur += glist->elem_size)
    {
        func((void*)cur);
    }
}

int generic_list_add(struct generic_list *glist, const void *elem, int(*cmp)(const void*, const void*))
{
    void *e = generic_list_find(glist, elem, cmp);
    if (e == NULL) {
        generic_list_push_back(glist, elem);
        return 1;
    }
    else {
        return 0;
    }
}