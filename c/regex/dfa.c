#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glist.h"
#include "dfa.h"

MAKE_COMPARE_FUNCTION(addr, struct DFA_state *);

struct DFA_state *alloc_DFA_state(void)
{
    struct DFA_state *state = (struct DFA_state*)malloc(sizeof(struct DFA_state));
    state->_capacity = 4;
    state->n_transitions = 0;
    state->is_acceptable = 0;
    state->trans = (struct DFA_transition*)malloc(state->_capacity*sizeof(struct DFA_transition));
    return state;
}

void free_DFA_state(struct DFA_state *state)
{
    free(state->trans);
    free(state);
}

void DFA_traverse(struct DFA_state *state, struct generic_list *visited)
{
    int i_trans = 0;
    int n_trans = state->n_transitions;
    for (; i_trans < n_trans; i_trans++)
    {
        if (generic_list_add(visited, &state->trans[i_trans].to, __cmp_addr) != 0) {
            DFA_traverse(state->trans[i_trans].to, visited);
        }
    }
}

void DFA_dispose(struct DFA_state *start)
{
    struct generic_list state_list;
    struct DFA_state **curr;
    int i_state = 0;

    generic_list_create(struct DFA_state *, &state_list);
    generic_list_push_back(&state_list, &start);
    DFA_traverse(start, &state_list);

    for (curr = (struct DFA_state **)state_list.p_data; i_state < state_list.length; i_state++, curr++)
    {
        free_DFA_state(*curr);
    }
    generic_list_destroy(&state_list);
}

void DFA_make_acceptable(struct DFA_state *state)
{
    state->is_acceptable = 1;
}

void DFA_add_transition(struct DFA_state *from, struct DFA_state *to, char trans_char)
{
    if (from->n_transitions == from->_capacity)
    {
        from->_capacity *= 2;
        from->trans = (struct DFA_transition*)realloc(from->trans, from->capacity*sizeof(struct DFA_transition));
    }
    from->trans[from->n_transitions].to = to;
    from->trans[from->n_transitions].trans_char = trans_char;
    from->n_transitions++;
}

struct DFA_state *NFA_target_of_trans(struct DFA_state *state, char trans_char)
{
    int i_trans = 0;
    int n_trans = state->n_transitions;
    for (; i_trans < n_trans; i_trans++)
    {
        if (state->trans[i_trans].trans_char == trans_char) {
            return state->trans[i_trans].to;
        }
    }
    return NULL;
}

