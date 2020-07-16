struct NFA_state* alloc_NFA_state(void)
{
    struct NFA_state *state = (struct NFA_state*)malloc(sizeof(struct NFA_state));
    struct NFA_transition null_transition = {NFATT_NONE, 0};
    state->to[0] = state->to[1] = NULL;
    state->transition[0] = state->transition[1] = null_transition;
    return state;
}

void free_NFA_state(struct NFA_state* state)
{
    free(state);
}

int NFA_state_transition_num(const struct NFA_state * state)
{
    if (state->transition[1].trans_type != NFATT_NONE) return 2;
    if (state->transition[0].trans_type != NFATT_NONE) return 1;
    return 0;
}

/**
    return 0 on success
    return -1, no empty slot avaliable
*/
int NFA_state_add_transition(struct NFA_state *state, enum NFA_transition_type trans_type, char trans_char, struct NFA_state *to_state)
{
    int tnum = NFA_state_transition_num(state);
    if (tnum >= 2) return -1;
    else {
        state->transition[tnum].trans_type = trans_type;
        state->transition[tnum].trans_char = trans_char;
        state->to[tnum] = to_state;
        return 0;
    }
}

int NFA_epsilon_move(struct NFA_state *from, struct NFA_state *to)
{
    NFA_state_add_transition(from, NFATT_EPSILON, 0, to);
}

struct NFA NFA_create_atomic(char c)
{
    assert(c != '\0');
    struct NFA nfa;
    nfa.start = alloc_NFA_state();
    nfa.terminate = alloc_NFA_state();
    NFA_state_add_transition(nfa.start, NFATT_CHARACTER, c, nfa.terminate);
    return nfa;
}

// c = ab
struct NFA NFA_concatenate(const struct NFA *a, const struct NFA *b)
{
    struct NFA nfa;
    nfa.start = a->start;
    nfa.terminate = b->terminate;
    NFA_epsilon_move(a->terminate, b->start);
    return nfa;
}

// c = a|b
struct NFA NFA_alternate(const struct NFA *a, const struct NFA *b)
{
    struct NFA nfa;
    nfa.start = alloc_NFA_state();
    nfa.terminate = alloc_NFA_state();
    NFA_epsilon_move(nfa->terminate, a->start);
    NFA_epsilon_move(nfa->terminate, b->start);
    NFA_epsilon_move(a.terminate, nfa.terminate);
    NFA_epsilon_move(b.terminate, nfa.terminate);
    return nfa;
}

// c =a? = a | epsilon
struct NFA NFA_optional(const struct NFA *a)
{
    struct NFA c;
    c.start = alloc_NFA_state();
    c.terminate = a->terminate;
    NFA_epsilon_move(c.start, a->start);
    NFA_epsilon_move(c.start, c.terminate);
    return c;
}

// c = a*
struct NFA NFA_kleene_closure(const struct NFA *a)
{
    struct NFA c;
    c.start = alloc_NFA_state();
    c.terminate = a->terminate;
    NFA_epsilon_move(c.start, a->start);
    NFA_epsilon_move(c.start, c.terminate);
    NFA_epsilon_move(c.terminate, a->start);
    return c;
}

// c= a+ = aa*
struct NFA NFA_positive_closure(const struct NFA *a)
{
    struct NFA c;
    c.start = alloc_NFA_state();
    c.terminate = alloc_NFA_state();
    NFA_epsilon_move(c.start, a->start);
    NFA_epsilon_move(a->terminate, c.terminate);
    NFA_epsilon_move(a->terminate, c.start);
    return c;    
}

MAKE_COMPARE_FUNCTION(addr, int*)

static void __NFA_traverse(struct NFA_state *state, struct generic_list *visited)
{
    int i = 0;
    int num = NFA_state_transition_num(state);
    for (; i < num; i++)
    {
        if (generic_list_add(visited, &state->to[i], __cmp_addr) != 0)
        {
            __NFA_traverse(state->to[i], visited);
        }
    }
}

void NFA_dispose(struct NFA *nfa)
{
    struct generic_list visited;
    struct NFA_state **curr;
    int i_state = 0;
    generic_list_create(struct NFA_state*, &visited);
    generic_list_push_back(&visited, &nfa->start);
    __NFA_traverse(nfa->start, &visited);
    for (curr = (struct NFA_state**)visited.p_data; i_state < visited.length; i_state++, curr++)
    {
        free_NFA_state(*curr);
    }
    generic_list_destroy(&visited);
}

/* Match the given substring in a recursive fasion */
static int __NFA_is_substate_match(
    const struct NFA_state *state, const char *str)
{
    char c = str[0];  /* transition to match */
    int i_trans = 0, n_trans = NFA_state_transition_num(state);
    int is_matched = 0;

    /* If we reached the terminate state while consumed the entire string*/
    if (c == '\0' && n_trans == 0)   return 1;   /* str matched the nfa */

    for ( ; i_trans < n_trans; i_trans++)
    {
        /* if it is an epsilon move, we can take this way instantly */
        if (state->transition[i_trans].trans_type == NFATT_EPSILON) {
            is_matched = __NFA_is_substate_match(state->to[i_trans], str);
        }
        /* or it must be a character transition, check if we can take it */
        else if (state->transition[i_trans].trans_char == c) {
            is_matched = __NFA_is_substate_match(state->to[i_trans], str + 1);
        }

        if (is_matched) return 1;
    }

    return 0;  /* not matched */
}

/* Check if the string matches the pattern implied by the nfa */
int NFA_pattern_match(const struct NFA *nfa, const char *str)
{
    /* find a sequence of transitions recursively */
    return __NFA_is_substate_match(nfa->start, str);
}