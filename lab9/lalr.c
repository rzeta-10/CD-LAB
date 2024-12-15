#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 20
#define MAX_STR 20

// Structure to represent a production rule
struct Prod {
    char lhs;
    char rhs[MAX_STR];
};

// Structure to represent an item in the LR(0) item set
struct Item {
    char lhs;
    char rhs[MAX_STR];
    int dot;
    char lookahead[MAX_STR];
};

// Structure to represent an item set
struct ItemSet {
    struct Item item[MAX_PROD];
    int size;
    int index;
    int merged;
};

// Structure to represent a transition
struct Trans {
    int from;
    char symbol;
    int to;
};

// Structure to represent an action in the parsing table
struct Act {
    char action[10];
    int next;
};

struct Prod prod[MAX_PROD];
struct ItemSet items[MAX_PROD];
struct Trans trans[MAX_PROD * 5];
struct Act action[MAX_PROD][MAX_PROD];
char terms[MAX_PROD];

int num_prod;
int num_items = 0;
int num_trans = 0;

// Function to check if a character is a non-terminal
int is_nonterm(char c) {
    return isupper(c);
}

// Function to add a character to a set
void add_to_set(char *set, char c) {
    int i;
    for (i = 0; set[i] != '\0'; i++) {
        if (set[i] == c) {
            return;
        }
    }
    set[i] = c;
    set[i + 1] = '\0';
}

// Function to compute the first set of a character
char *first(char *set, char c) {
    char temp[10];
    temp[0] = '\0';
    if (!is_nonterm(c)) {
        add_to_set(set, c);
        return set;
    }
    for (int i = 0; i <= num_prod; i++) {
        if (prod[i].lhs == c) {
            if (prod[i].rhs[0] == prod[i].lhs) continue;
            first(temp, prod[i].rhs[0]);
            for (int k = 0; temp[k] != '\0'; k++) {
                add_to_set(set, temp[k]);
            }
        }
    }
    return set;
}

// Function to print the items
void print_items() {
    printf("Number of items: %d\n", num_items);
    for (int i = 0; i < num_items; i++) {
        if (items[i].merged) continue;
        printf("Item %d - %d\n", i, items[i].index);
        printf("Size: %d\n", items[i].size);
        for (int j = 0; j < items[i].size; j++) {
            printf("%c->%s, %s - %d\n", items[i].item[j].lhs, items[i].item[j].rhs, items[i].item[j].lookahead, items[i].item[j].dot);
        }
        printf("\n");
    }
}

// Function to print a single item set
void print_item(struct ItemSet *set) {
    printf("Item %d\n", set->index);
    printf("Size: %d\n", set->size);
    for (int j = 0; j < set->size; j++) {
        printf("%c->%s, %s - %d\n", set->item[j].lhs, set->item[j].rhs, set->item[j].lookahead, set->item[j].dot);
    }
    printf("\n");
}

// Function to print the transitions
void print_trans() {
    printf("Number of transitions: %d\n", num_trans);
    for (int i = 0; i < num_trans; i++) {
        printf("%d, %c -> %d\n", trans[i].from, trans[i].symbol, trans[i].to);
    }
}

// Function to print the productions
void print_prods() {
    int i;
    for (i = 0; i <= num_prod; i++) {
        printf("%c->%s\n", prod[i].lhs, prod[i].rhs);
    }
}

// Function to check if two item sets have the same core
int are_cores_eq(struct ItemSet *set1, struct ItemSet *set2) {
    if (set1->size != set2->size) return 0;
    for (int i = 0; i < set1->size; i++) {
        if (set1->item[i].lhs != set2->item[i].lhs ||
            strcmp(set1->item[i].rhs, set2->item[i].rhs) != 0 ||
            set1->item[i].dot != set2->item[i].dot) {
            return 0;
        }
    }
    return 1;
}

// Function to merge lookaheads of two item sets
void merge_lookaheads(struct ItemSet *dst, struct ItemSet *src) {
    for (int i = 0; i < src->size; i++) {
        for (int j = 0; j < strlen(src->item[i].lookahead); j++) {
            char lookahead = src->item[i].lookahead[j];
            if (!strchr(dst->item[i].lookahead, lookahead)) {
                strncat(dst->item[i].lookahead, &lookahead, 1);
            }
        }
    }
}

// Function to compute the closure of an item set
void closure(struct ItemSet *temp) {
    int i = 0;
    while (i < temp->size) {
        if (is_nonterm(temp->item[i].rhs[temp->item[i].dot])) {
            for (int j = 0; j <= num_prod; j++) {
                if (prod[j].lhs == temp->item[i].rhs[temp->item[i].dot]) {
                    struct Item t;
                    t.lhs = prod[j].lhs;
                    strcpy(t.rhs, prod[j].rhs);
                    t.dot = 0;

                    char set[10];
                    set[0] = '\0';
                    if (temp->item[i].dot + 1 >= strlen(temp->item[i].rhs)) {
                        strcpy(t.lookahead, temp->item[i].lookahead);
                    } else {
                        first(set, temp->item[i].rhs[temp->item[i].dot + 1]);
                        strcpy(t.lookahead, set);
                    }

                    int found = 0;
                    for (int k = 0; k < temp->size; k++) {
                        if (temp->item[k].lhs == t.lhs &&
                            strcmp(temp->item[k].rhs, t.rhs) == 0 &&
                            temp->item[k].dot == t.dot) {
                            for (int m = 0; t.lookahead[m] != '\0'; m++) {
                                if (!strchr(temp->item[k].lookahead, t.lookahead[m])) {
                                    strncat(temp->item[k].lookahead, &t.lookahead[m], 1);
                                }
                            }
                            found = 1;
                            break;
                        }
                    }

                    if (!found) {
                        temp->item[temp->size++] = t;
                    }
                }
            }
        }
        i++;
    }

    temp->merged = 0;
    temp->index = num_items;
    items[num_items] = *temp;
    num_items++;
}

// Function to merge states with the same core
void merge_states() {
    for (int i = 0; i < num_items; i++) {
        for (int j = i + 1; j < num_items; j++) {
            if (are_cores_eq(&items[i], &items[j])) {
                merge_lookaheads(&items[i], &items[j]);

                for (int k = 0; k < num_trans; k++) {
                    if (trans[k].to == items[j].index) {
                        trans[k].to = items[i].index;
                    }
                }

                items[j].merged = 1;
            }
        }
    }
}

// Function to compute the goto of an item set
void go_to(struct ItemSet* src, char symbol) {
    struct ItemSet dst;
    dst.size = 0;

    for (int i = 0; i < src->size; i++) {
        if (src->item[i].rhs[src->item[i].dot] == symbol) {
            struct Item new_item;
            new_item.lhs = src->item[i].lhs;
            strcpy(new_item.rhs, src->item[i].rhs);
            strcpy(new_item.lookahead, src->item[i].lookahead);
            new_item.dot = src->item[i].dot + 1;

            dst.item[dst.size++] = new_item;
        }
    }

    if (dst.size > 0) {
        closure(&dst);

        trans[num_trans].from = src->index;
        trans[num_trans].symbol = symbol;
        trans[num_trans].to = dst.index;
        num_trans++;
        merge_states();
    }
}

// Function to initialize the item sets
void init() {
    struct ItemSet temp;
    temp.item[0].lhs = 'Z';
    strcpy(temp.item[0].rhs, prod[0].rhs);
    temp.item[0].dot = 0;
    strcpy(temp.item[0].lookahead, "$");
    temp.size = 1;

    closure(&temp);
    for (int i = 0; i < num_items; i++) {
        for (int j = 0; j < items[i].size; j++) {
            if (items[i].item[j].dot < strlen(items[i].item[j].rhs) && items[i].merged == 0) {
                char next = items[i].item[j].rhs[items[i].item[j].dot];
                go_to(&items[i], next);
            }
        }
    }
}

// Function to update the transitions after merging states
void update_trans() {
    for (int i = 0; i < num_trans; i++) {
        for (int j = 0; j < num_items; j++) {
            if (trans[i].from == items[j].index) {
                trans[i].from = j;
            }
            if (trans[i].to == items[j].index) {
                trans[i].to = j;
            }
        }
    }
}

// Function to remove merged items
void remove_merged() {
    struct ItemSet temp[MAX_PROD];
    int j = 0;
    for (int i = 0; i < num_items; i++) {
        if (items[i].merged == 0) {
            temp[j++] = items[i];
        }
    }
    num_items = j;
    memset(items, 0, sizeof(items));
    for (int i = 0; i < num_items; i++) {
        items[i] = temp[i];
    }
    update_trans();
}

// Function to get terminals and non-terminals
void get_terms() {
    terms[0] = '\0';
    for (int i = 1; i <= num_prod; i++) {
        for (int k = 0; prod[i].rhs[k] != '\0'; k++) {
            if (!is_nonterm(prod[i].rhs[k])) {
                add_to_set(terms, prod[i].rhs[k]);
            }
        }
    }
    add_to_set(terms, '$');
    for (int i = 1; i <= num_prod; i++) {
        add_to_set(terms, prod[i].lhs);
    }
}

// Function to get the index of a character in the terminals array
int get_index(char c) {
    for (int i = 0; terms[i] != '\0'; i++) {
        if (terms[i] == c) {
            return i;
        }
    }
    return -1;
}

// Function to get the index of a production
int get_prod_index(struct Item item) {
    for (int i = 1; i <= num_prod; i++) {
        if (prod[i].lhs == item.lhs && strcmp(prod[i].rhs, item.rhs) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to print the action table
void print_act_table(char *terms, int num_states) {
    printf("\nAction Table:\n");

    printf("State\t");
    for (int i = 0; terms[i] != '\0'; i++) {
        printf("%c\t", terms[i]);
    }
    printf("\n");

    for (int i = 0; i <= strlen(terms); i++) {
        printf("-------");
    }
    printf("\n");

    for (int state = 0; state < num_states; state++) {
        printf("%d\t", state);

        for (int i = 0; terms[i] != '\0'; i++) {
            if (strcmp(action[state][i].action, "") == 0) {
                printf(" \t");
            } else if (is_nonterm(terms[i])) {
                printf("%d\t", action[state][i].next);
            } else if (strcmp(action[state][i].action, "shift") == 0) {
                printf("S%d\t", action[state][i].next);
            } else if (strcmp(action[state][i].action, "reduce") == 0) {
                printf("r%d\t", action[state][i].next);
            } else if (strcmp(action[state][i].action, "accept") == 0) {
                printf("acc\t");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Function to create the action table
void create_act_table() {
    get_terms();

    memset(action, 0, sizeof(action));

    for (int i = 0; i < num_trans; i++) {
        struct Act a;
        int index = get_index(trans[i].symbol);
        a.next = trans[i].to;
        strcpy(a.action, "shift");
        action[trans[i].from][index] = a;
    }
    for (int i = 0; i < num_items; i++) {
        if (items[i].size == 1 && items[i].item[0].dot == strlen(items[i].item[0].rhs)) {
            struct Item item = items[i].item[0];
            for (int j = 0; j < strlen(item.lookahead); j++) {
                int index = get_index(item.lookahead[j]);
                struct Act a;
                strcpy(a.action, "reduce");
                if (item.lhs == 'Z') {
                    strcpy(a.action, "accept");
                } else {
                    int p = get_prod_index(item);
                    a.next = p;
                }
                action[i][index] = a;
            }
        }
    }
    print_act_table(terms, num_items);
}

// Function to print the current parsing state
void print_parse_state(char *stack, int top, char *str, int lookahead) {
    printf("Stack: ");
    for (int i = 0; i <= top; i++) {
        printf("%c", stack[i]);
    }
    printf("\tInput: ");
    for (int i = lookahead; str[i] != '\0'; i++) {
        printf("%c", str[i]);
    }
    printf("\tAction: ");
    int index = get_index(str[lookahead]);
    printf("%s %d", action[stack[top] - '0'][index].action, action[stack[top] - '0'][index].next);

    printf("\n");
}

// Function to parse the input string
int parse(char *str) {
    char stack[MAX_STR];
    int top = 0;
    int lookahead = 0;
    int len = strlen(str);
    str[len++] = '$';
    str[len] = '\0';
    stack[top] = '$';
    stack[++top] = '0';

    printf("Parsing:\n");
    while (lookahead < len) {
        print_parse_state(stack, top, str, lookahead);
        char t = stack[top];
        char symbol = str[lookahead];
        struct Act a;
        if (isdigit(t)) {
            int index = get_index(symbol);
            a = action[t - '0'][index];
            if (strcmp(a.action, "shift") == 0) {
                stack[++top] = str[lookahead];
                stack[++top] = a.next + '0';
                lookahead++;
            } else if (strcmp(a.action, "reduce") == 0) {
                int pop_len = 2 * strlen(prod[a.next].rhs);
                for (int i = 0; i < pop_len; i++) {
                    top--;
                }
                stack[++top] = prod[a.next].lhs;
            } else if (strcmp(a.action, "accept") == 0) {
                return 1;
            } else {
                return 0;
            }
        } else if (is_nonterm(t)) {
            int prev = top - 1;
            int index = get_index(t);
            stack[++top] = action[stack[prev] - '0'][index].next + '0';
        }
    }
    return 0;
}

// Function to perform LALR parsing
void lalr(char *str) {
    init();
    remove_merged();
    create_act_table();
    if (parse(str)) {
        printf("\nString accepted\n");
    } else {
        printf("\nString not accepted\n");
    }
}

int main() {
    printf("Enter the number of productions : ");
    scanf("%d", &num_prod);
    printf("Enter the productions in the form S->AB\n");
    for (int i = 1; i <= num_prod; i++) {
        printf("Enter the production %d : ", i);
        scanf(" %c->%s", &prod[i].lhs, prod[i].rhs);
    }

    prod[0].lhs = 'Z';
    prod[0].rhs[0] = prod[1].lhs;
    prod[0].rhs[1] = '\0';

    char str[MAX_STR];
    printf("Enter the string to parse: ");
    scanf("%s", str);

    lalr(str);
    return 0;
}