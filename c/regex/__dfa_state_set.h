#ifndef _dfa_state_set_h_
#define _dfa_state_set_h_

#include "dfa.h"
#include "nfa.h"


MAKE_COMPARE_FUNCTION(addr, struct DFA_state*)
MAKE_COMPARE_FUNCTION(char, char)

struct __DFA_state_set
{
    struct __DFA_state_set *prev;
    struct __DFA_state_set *next;

    struct generic_list dfa_states;
    struct DFA_state *merged_state;
};

static struct __DFA_state_set *__alloc_stateset_node(void)
{
    struct __DFA_state_set *new_node = (struct __DFA_state_set*)malloc(sizeof(struct __DFA_state_set));
    new_node->prev = new_node->next = NULL;
    new_node->dfa_states.length = 0;
    new_node->merged_state = NULL;
    return new_node;
}

static struct __DFA_state_set *__create_empty_stateset_list(void)
{
    struct __DFA_state_set *head = __alloc_stateset_node();
    head->prev = head->next = head;
    return head;
}

static void __free_DFA_state_set(struct __DFA_state_set *state_set)
{
    generic_list_destroy(&state_set->dfa_states);
    free(state_set);
}

static void __destroy_DFA_stateset_list(struct __DFA_state_set *head)
{
    struct __DFA_state_set *cur = head->next, *next;
    free(head);
    for (; cur != head; cur = next)
    {
        next = cur->next;
        __free_DFA_state_set(cur);
    }
}

static struct __DFA_state_set *__find_state_set(struct __DFA_state_set *ll_head, const struct DFA_state *state)
{
    struct __DFA_state_set *cur = ll_head->next;
    for (; cur != ll_head; cur = cur->next)
    {
        if (generic_list_find(&cur->dfa_states, &state, __cmp_addr) != NULL) {
            return cur;
        }
    }
    return NULL;
}

static void __insert_DFA_state_set_after(struct __DFA_state_set *e, struct __DFA_state_set *pivot)
{
    e->prev = pivot;
    e->next = pivot->next;
    pivot->next->prev = e;
    pivot->next = e;
}

static void __insert_states_after(const struct generic_list *states, struct __DFA_state_set *pivot)
{
    struct __DFA_state_set *new_node = __alloc_stateset_node();
    new_node->dfa_states = *states;
    __insert_DFA_state_set_after(new_node, pivot);
}

static void __remove_DFA_state_set(struct __DFA_state_set *state_set)
{
    state_set->prev->next = state_set->next;
    state_set->next->prev = state_set->prev;
    __free_DFA_state_set(state_set);
}

#endif