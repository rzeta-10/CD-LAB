//CS22B1093 Rohan G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct to store production
struct production { 
    char lhs;
    char rhs[10];
};

// struct to store first and follow sets
struct firsts_follows {
    char non_terminal;
    char first[10];
    char follow[10];
};

// add a symbol to a set (first or follow)
void addToSet(char* set, char symbol) {
    if (!strchr(set, symbol)) {
        strncat(set, &symbol, 1);
    }
}

// calculate first set of a grammar
void first(struct production* p, struct firsts_follows* ff, char* non_terminal, char* terminal, int productions, int non_terminals) {
    int updated;

    do {
        updated = 0; 
        // iterate through all productions
        for (int i = 0; i < productions; i++) {
            int lhs_index = 0;
            for (int j = 0; j < non_terminals; j++) {
                if (ff[j].non_terminal == p[i].lhs) {
                    lhs_index = j;
                    break;
                }
            }
            // if first symbol of rhs is terminal, add it to first set
            int k = 0;
            while (p[i].rhs[k] != '\0') {
                int isTerminal = 0;
                for (int m = 0; m < strlen(terminal); m++) {
                    if (p[i].rhs[k] == terminal[m]) {
                        if (!strchr(ff[lhs_index].first, p[i].rhs[k])) {
                            addToSet(ff[lhs_index].first, p[i].rhs[k]);
                            updated = 1;
                        }
                        isTerminal = 1;
                        break;
                    }
                }

                if (isTerminal) {
                    break;
                }
                // if first symbol of rhs is non-terminal, add its first set to first set
                int rhs_index = 0;
                for (int n = 0; n < non_terminals; n++) {
                    if (ff[n].non_terminal == p[i].rhs[k]) {
                        rhs_index = n;
                        break;
                    }
                }
                // add first set of rhs to first set of lhs
                for (int j = 0; j < strlen(ff[rhs_index].first); j++) {
                    if (ff[rhs_index].first[j] != 'e') {
                        if (!strchr(ff[lhs_index].first, ff[rhs_index].first[j])) {
                            addToSet(ff[lhs_index].first, ff[rhs_index].first[j]);
                            updated = 1;
                        }
                    }
                }
                
                // if first symbol of rhs is epsilon, add epsilon to first set of lhs
                if (strchr(ff[rhs_index].first, 'e')) {
                    k++;
                    if (p[i].rhs[k] == '\0') {
                        if (!strchr(ff[lhs_index].first, 'e')) {
                            addToSet(ff[lhs_index].first, 'e');
                            updated = 1;
                        }
                    }
                } else {
                    break;
                }
            }
        }
    } while (updated);
}

// calculate follow set of a grammar
void follow(struct production* p, struct firsts_follows* ff, int productions, int non_terminals) {
    int updated;

    // Initialize Follow(start_symbol) with '$'
    addToSet(ff[0].follow, '$');

    do {
        updated = 0; 
        for (int i = 0; i < productions; i++) {
            for (int k = 0; k < strlen(p[i].rhs); k++) {
                // Check if the symbol is a non-terminal
                for (int j = 0; j < non_terminals; j++) {
                    if (p[i].rhs[k] == ff[j].non_terminal) {
                        int non_terminal_index = j;
                        // Look at the symbol right after the current non-terminal
                        if (p[i].rhs[k + 1] != '\0') {
                            int next_symbol_index = -1;
                            int isTerminal = 0;
                            for (int m = 0; m < non_terminals; m++) {
                                if (p[i].rhs[k + 1] == ff[m].non_terminal) {
                                    next_symbol_index = m;
                                    break;
                                }
                            }
                            if (next_symbol_index == -1) {
                                isTerminal = 1;
                            }
                            // If the next symbol is a terminal, add it to the follow set
                            if (isTerminal) {
                                if (!strchr(ff[non_terminal_index].follow, p[i].rhs[k + 1])) {
                                    addToSet(ff[non_terminal_index].follow, p[i].rhs[k + 1]);
                                    updated = 1;
                                }
                            } else {
                                // If the next symbol is a non-terminal, add First(next_symbol) to Follow(current non-terminal)
                                for (int n = 0; n < strlen(ff[next_symbol_index].first); n++) {
                                    if (ff[next_symbol_index].first[n] != 'e') {
                                        if (!strchr(ff[non_terminal_index].follow, ff[next_symbol_index].first[n])) {
                                            addToSet(ff[non_terminal_index].follow, ff[next_symbol_index].first[n]);
                                            updated = 1;
                                        }
                                    }
                                }
                                // If First(next_symbol) contains epsilon, add Follow(LHS) to Follow(current non-terminal)
                                if (strchr(ff[next_symbol_index].first, 'e')) {
                                    for (int n = 0; n < strlen(ff[i].follow); n++) {
                                        if (!strchr(ff[non_terminal_index].follow, ff[i].follow[n])) {
                                            addToSet(ff[non_terminal_index].follow, ff[i].follow[n]);
                                            updated = 1;
                                        }
                                    }
                                }
                            }
                        } else {
                            // If the current non-terminal is the last symbol in RHS, add Follow(LHS) to Follow(current non-terminal)
                            int lhs_index = -1;
                            for (int n = 0; n < non_terminals; n++) {
                                if (ff[n].non_terminal == p[i].lhs) {
                                    lhs_index = n;
                                    break;
                                }
                            }
                            for (int n = 0; n < strlen(ff[lhs_index].follow); n++) {
                                if (!strchr(ff[non_terminal_index].follow, ff[lhs_index].follow[n])) {
                                    addToSet(ff[non_terminal_index].follow, ff[lhs_index].follow[n]);
                                    updated = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    } while (updated);
}

int main() {
    int non_terminals, terminals, productions;
    printf("Enter the number of non-terminals: ");
    scanf("%d", &non_terminals);
    printf("Enter the number of terminals: ");
    scanf("%d", &terminals);

    char non_terminal[non_terminals];
    char terminal[terminals];

    for (int i = 0; i < non_terminals; i++) {
        printf("Enter the non-terminal %d: ", i + 1);
        scanf(" %c", &non_terminal[i]);
    }

    for (int i = 0; i < terminals; i++) {
        printf("Enter the terminal %d: ", i + 1);
        scanf(" %c", &terminal[i]);
    }

    printf("Enter the number of productions: ");
    scanf("%d", &productions);

    struct production p[productions];
    struct firsts_follows ff[non_terminals];

    for (int i = 0; i < non_terminals; i++) {
        ff[i].non_terminal = non_terminal[i];
        ff[i].first[0] = '\0';
        ff[i].follow[0] = '\0';
    }

    printf("Enter the productions in the form A->aB\n");
    for (int i = 0; i < productions; i++) {
        printf("Enter the production %d: ", i + 1);
        scanf(" %c->%s", &p[i].lhs, p[i].rhs);
    }

    first(p, ff, non_terminal, terminal, productions, non_terminals);
    follow(p, ff, productions, non_terminals);

    printf("First sets:\n");
    for (int i = 0; i < non_terminals; i++) {
        printf("First(%c) = { ", ff[i].non_terminal);
        for (int j = 0; j < strlen(ff[i].first); j++) {
            printf("%c ", ff[i].first[j]);
        }
        printf("}\n");
    }

    printf("Follow sets:\n");
    for (int i = 0; i < non_terminals; i++) {
        printf("Follow(%c) = { ", ff[i].non_terminal);
        for (int j = 0; j < strlen(ff[i].follow); j++) {
            printf("%c ", ff[i].follow[j]);
        }
        printf("}\n");
    }

    return 0;
}
