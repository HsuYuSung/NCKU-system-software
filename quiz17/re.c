#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum state_behavior { state_single, state_split };

enum special_chars { any_char };

typedef struct state {
    enum state_behavior type;
    char matching_value;

    struct state *output, *output1;
} state;

typedef struct {
    state *start;

    int num_dangling;
    state ***dangling;
} fragment;

static state *create_single_state(char matching_value)
{
    state *new_state = malloc(sizeof(state));
    new_state->type = state_single;
    new_state->matching_value = matching_value;
    return new_state;
}

static state *create_split_state()
{
    state *new_state = malloc(sizeof(state));
    new_state->type = state_split;
    return new_state;
}

static fragment *create_fragment(state *start,
                                 int num_dangling,
                                 state ***dangling)
{
    fragment *frag = malloc(sizeof(fragment));
    frag->start = start;
    frag->num_dangling = num_dangling, frag->dangling = dangling;
    return frag;
}

typedef struct {
    state **list;
    int capacity;
    int next_index;
} state_list;

#define INITIAL_SIZE 100000
static state_list *list_create()
{
    state_list *new_list = malloc(sizeof(state_list));
    new_list->capacity = INITIAL_SIZE;
    new_list->next_index = 0;
    new_list->list = malloc(sizeof(state *) * INITIAL_SIZE);
    return new_list;
}

static void list_grow(state_list *grow)
{
    grow->list = realloc(grow->list, (sizeof(state **) * grow->capacity) * 2);
    grow->capacity *= 2;
}

static void list_append(state_list *to_append, state *data)
{
    if (to_append->next_index >= to_append->capacity)
        list_grow(to_append);

    to_append->list[to_append->next_index] = data;
    to_append->next_index++;
}

static void list_clear(state_list *to_clear)
{
    to_clear->next_index = 0;
}

#define MAX_STATES 1000000

static int current_index = 0;
static char *regex;

static bool is_match(state_list *list)
{
    for (int i = 0; i < list->next_index; i++) {
        state *check = list->list[i];
        if (!check)
            return true;
        if ((check->type == state_split) && (!check->output || !check->output1))
            return true;
    }
    return false;
}


bool nfa_matches(char *string, state *nfa)
{
    state_list *possible = list_create(), *next_possible = list_create();

    list_append(possible, nfa);

    while (*string != '\0') {
        for (int i = 0; i < possible->next_index; i++) {
            state *current = possible->list[i];

            if (current->type == state_single) {
                if (current->matching_value == *string ||
                    current->matching_value == any_char) {
                    if (!current->output)
                        return true;

                    list_append(next_possible, current->output);
                }
            } else if (current->type == state_split) {
                if (!current->output || !current->output1)
                    return true;

                list_append(possible, current->output);
                list_append(possible, current->output1);
            }
        }

        /* swap new list of next states with old clear the new old */
        state_list *tmp = possible;
        possible = next_possible;
        next_possible = tmp;
        list_clear(next_possible);

        string++;
    }

    return is_match(possible);
}

static inline char peek()
{
    return regex[current_index];
}

static inline bool eat(char c)
{
    if (regex[current_index] == c) {
        current_index++;
        return true;
    }

    return false;
}

static inline char next()
{
    return regex[current_index++];
}

static bool more()
{
    return current_index < strlen(regex);
}

static void connect_dangling(fragment *frag, fragment *output)
{
    for (int i = 0; i < frag->num_dangling; i++)
        *(frag->dangling[i]) = output->start;
}

static fragment *parse_factor();

static fragment *parse_term()
{
    fragment *next = parse_factor();
    fragment *first = next;

    while (more() && peek() != ')' && peek() != '|') {
        fragment *after = parse_factor();
        connect_dangling(next, after);

        first->dangling = after->dangling;
        first->num_dangling = after->num_dangling;

        next = after;
    }
    return first;
}

fragment *parse_regex()
{
    fragment *term = parse_term();

    if (more() && peek() == '|') {
        eat('|');
        fragment *regex = parse_regex();

        state *split = create_split_state();
        split->output = term->start, split->output1 = regex->start;

        state ***dangling = malloc((term->num_dangling + regex->num_dangling) *
                                   sizeof(state **));

        for (int i = 0; i < term->num_dangling; i++)
            dangling[i] = term->dangling[i];

        for (int i = 0; i < regex->num_dangling; i++)
            dangling[term->num_dangling + i] = regex->dangling[i];

        return create_fragment(split, term->num_dangling + regex->num_dangling,
                               dangling);
    }
    return term;
}

static fragment *parse_base()
{
    char matching_value;

    switch (peek()) {
    case '(':
        eat('(');
        fragment *regex = parse_regex();
        eat(')');
        return regex;
    case '.':
        matching_value = any_char;
        next();
        break;
    case '\\':
        eat('\\');
        matching_value = next();
        break;
    default:
        matching_value = next();
        break;
    }

    state *single = create_single_state(matching_value);
    state ***dangling = malloc(1 * sizeof(state **));
    dangling[0] = &single->output;
    single->output = NULL;
    return create_fragment(single, 1, dangling);
}

static fragment *parse_factor()
{
    fragment *base = parse_base();

    if (more() && peek() == '*') {
        eat('*');

        state *next = create_split_state();
        state ***dangling = malloc(1 * sizeof(state **));
        dangling[0] = &next->output1;
        next->output = base->start;
        next->output1 = NULL;

        fragment *connected = create_fragment(next, 1, dangling);
        connect_dangling(base, connected);
        return connected;
    } else if (more() && peek() == '+') {
        eat('+');

        state *next = create_split_state();
        state ***dangling = malloc(1 * sizeof(state **));
        dangling[0] = &next->output1;
        next->output = base->start;
        next->output1 = NULL;

        fragment *connected = create_fragment(next, 1, dangling);
        connect_dangling(base, connected);

        base->dangling = connected->dangling;
        base->num_dangling = connected->num_dangling;
        return base;
    } else if (more() && peek() == '?') {
        eat('?');

        state *next = create_split_state();
        state ***dangling = malloc((1 + base->num_dangling) * sizeof(state **));
        dangling[0] = &next->output1;
        next->output1 = NULL;

        next->output = base->start;

        for (int i = 0; i < base->num_dangling; i++)
            dangling[1 + i] = base->dangling[i];

        fragment *connected =
            create_fragment(next, 1 + base->num_dangling, dangling);
        return connected;
    }
    return base;
}

int main(int argc, char **argv)
{
    if (argc < 3)
        return -1;

    regex = argv[1];
    char *match = argv[2];
    fragment *parsed = parse_regex();

    /* add .* to make all expressions match inside strings, since we have
     * basically got an implicit ^ without it.
     */
    state *star_split = create_split_state();
    state *star = create_single_state(any_char);
    star_split->output = star;
    star_split->output1 = parsed->start;
    star->output = star_split;

    bool matches = nfa_matches(match, star_split);
    printf("Result (%s on %s): %d\n", regex, match, matches);

    return 0;
}

