#infdef _NFA_H_
#define _NFA_H_

#include <stdlib.h>
#include <stdio.h>

enum NFA_transition_type {
    NFATT_NONE,
    NFATT_CHARACTER,
    NFATT_EPSILON,
};

struct NFA_stransition
{
    enum NFA_stransition_type trans_type;
    char trans_char;
};

struct NFA_state
{
    struct NFA_state *to[2];
    struct NFA_transition transition[2];
};

struct NFA
{
    struct NFA_state *start;
    struct NFA_state *terminate;
};

struct NFA_state *alloc_NFA_state(void);

void free_NFA_state(struct NFA_state *state);

int NFA_state_transition_num(const struct NFA_state *state);

int NFA_state_add_transition(struct NFA_state *state,
enum NFA_stransition_type trans_type, char trans_char,
struct NFA_state *to_state);

int NFA_epsilon_move(struct NFA_state *from, struct NFA_state *to);


struct NFA NFA_create_atomic(char c);

struct NFA NFA_concatenate(const struct NFA *a, const struct NFA *b);
struct NFA NFA_alternate(const struct NFA *a, const struct NFA *b);
struct NFA NFA_optional(const struct NFA *a);
struct NFA NFA_kleene_closure(const struct NFA *a);
struct NFA NFA_positive_closure(const struct NFA *a);

void NFA_dispose(struct NFA *nfa);

int NFA_pattern_match(const struct NFA *nfa, const char *str);
#endif