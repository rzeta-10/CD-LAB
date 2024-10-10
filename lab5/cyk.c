#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PRODUCTIONS 100
#define MAX_RHS_LENGTH 10
#define MAX_STRING_LENGTH 20

typedef struct {
    char lhs;
    char rhs[MAX_RHS_LENGTH];
} Production;

typedef struct {
    Production productions[MAX_PRODUCTIONS];
    int num_productions;
    char start_symbol;
} Grammar;

int count_parse_trees(Grammar* grammar, const char* input, int input_index, char current_symbol) {
    if (current_symbol == 'e' && input[input_index] == '\0') {
        return 1;
    }
    
    if (input[input_index] == '\0' || current_symbol == '\0') {
        return 0;
    }
    
    if (current_symbol >= 'a' && current_symbol <= 'z') {
        if (input[input_index] == current_symbol) {
            return count_parse_trees(grammar, input, input_index + 1, '\0');
        }
        return 0;
    }
    
    int total_trees = 0;
    for (int i = 0; i < grammar->num_productions; i++) {
        if (grammar->productions[i].lhs == current_symbol) {
            int trees_from_this_production = 1;
            for (int j = 0; grammar->productions[i].rhs[j] != '\0'; j++) {
                int trees_for_symbol = count_parse_trees(grammar, input, input_index, grammar->productions[i].rhs[j]);
                if (trees_for_symbol == 0) {
                    trees_from_this_production = 0;
                    break;
                }
                input_index += strlen(input) - input_index - (trees_for_symbol > 0);
                trees_from_this_production *= trees_for_symbol;
            }
            total_trees += trees_from_this_production;
        }
    }
    
    return total_trees;
}

bool is_ambiguous(Grammar* grammar) {
    char input[MAX_STRING_LENGTH];
    
    // Generate all possible strings up to a certain length
    for (int len = 0; len <= 10; len++) {
        input[len] = '\0';
        for (int i = 0; i < len; i++) {
            input[i] = 'a';
            for (int j = 0; j < (1 << len); j++) {
                int parse_trees = count_parse_trees(grammar, input, 0, grammar->start_symbol);
                if (parse_trees > 1) {
                    printf("Ambiguous string found: %s (has %d parse trees)\n", input, parse_trees);
                    return true;  // Ambiguity detected
                }
                
                // Generate next string
                int k = 0;
                while (k < len && input[k] == 'b') {
                    input[k] = 'a';
                    k++;
                }
                if (k < len) input[k] = 'b';
            }
        }
    }
    
    return false;  // No ambiguity detected within the search space
}

int main() {
    Grammar grammar;
    printf("Enter the number of productions: ");
    scanf("%d", &grammar.num_productions);
    
    printf("Enter the start symbol: ");
    scanf(" %c", &grammar.start_symbol);
    
    printf("Enter the productions in the format 'LHS->RHS' (use 'e' for epsilon):\n");
    for (int i = 0; i < grammar.num_productions; i++) {
        char input[20];
        scanf("%s", input);
        grammar.productions[i].lhs = input[0];
        strcpy(grammar.productions[i].rhs, input + 3);
    }
    
    if (is_ambiguous(&grammar)) {
        printf("The given grammar is ambiguous.\n");
    } else {
        printf("The given grammar is not ambiguous (within the tested string length).\n");
    }
    
    return 0;
}