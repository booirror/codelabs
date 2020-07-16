#ifndef _DFA_H_
#define _DFA_H_

#include <stdio.h>
#include <stdlib.h>

#include "glist.h"

struct DFA_state;

struct DFA_transition
{
    struct DFA_state *to;
    char trans_char;
}

struct DFA_state
{
    int is_acceptable;
    struct DFA_transition *trans;
    int n_transitions;
    int _capacity;
}

struct DFA_state *alloc_NFA_state(void);

void free_DFA_state(struct DFA_state *state);

void DFA_dispose(struct DFA_state *start);

void DFA_make_acceptable(struct DFA_state *state);

void DFA_add_transition(struct DFA_state *from, struct DFA_state *to, char trans_char);

struct DFA_state *DFA_target_of_trans(struct DFA_state *state, char trans_char);

void DFA_traverse(struct DFA_state *state, struct generic_list *visited);

#endif