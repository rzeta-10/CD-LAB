// CS22B1093 ROHAN G

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PRODUCTIONS 10
#define MAX_STRING_LENGTH 100
#define MAX_DERIVATIONS 1000
#define MAX_RECURSION_DEPTH 10
#define MAX_PATH_LENGTH 1000

struct production {
    char lhs;
    char rhs[MAX_STRING_LENGTH];
};

struct derivation {
    char derived_str[MAX_STRING_LENGTH];
    char derivation_path[MAX_PATH_LENGTH];
};

// Function to generate all possible strings from the grammar up to a certain depth
void generate_strings(struct production grammar[], int n, char start, char *str, int max_depth, int current_depth, struct derivation derivations[], int *derivation_count, char *path) {
    if (current_depth > max_depth || *derivation_count >= MAX_DERIVATIONS) {
        return;
    }

    int is_terminal = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {  // Non-terminal found
            is_terminal = 0;
            break;
        }
    }

    if (is_terminal) {
        // If terminal, store the string and path
        if (*derivation_count < MAX_DERIVATIONS) {
            strncpy(derivations[*derivation_count].derived_str, str, MAX_STRING_LENGTH - 1);
            derivations[*derivation_count].derived_str[MAX_STRING_LENGTH - 1] = '\0';
            strncpy(derivations[*derivation_count].derivation_path, path, MAX_PATH_LENGTH - 1);
            derivations[*derivation_count].derivation_path[MAX_PATH_LENGTH - 1] = '\0';
            (*derivation_count)++;
        }
        return;
    }

    // Apply production rules
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {  // Non-terminal found
            char original = str[i];
            for (int j = 0; j < n; j++) {
                if (grammar[j].lhs == original) {
                    // Apply production and create new string
                    char new_str[MAX_STRING_LENGTH];
                    strncpy(new_str, str, i);
                    new_str[i] = '\0';
                    strncat(new_str, grammar[j].rhs, MAX_STRING_LENGTH - strlen(new_str) - 1);
                    strncat(new_str, &str[i + 1], MAX_STRING_LENGTH - strlen(new_str) - 1);

                    // Update path with applied production
                    char new_path[MAX_PATH_LENGTH];
                    snprintf(new_path, sizeof(new_path), "%s %c->%s", path, grammar[j].lhs, grammar[j].rhs);

                    // Recursively generate further strings
                    generate_strings(grammar, n, start, new_str, max_depth, current_depth + 1, derivations, derivation_count, new_path);
                }
            }
        }
    }
}

// Function to check for structural ambiguity by comparing derivations
int check_ambiguity(struct derivation derivations[], int count) {
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            // If two derivations generate the same string but with different derivation paths
            if (strcmp(derivations[i].derived_str, derivations[j].derived_str) == 0 &&
                strcmp(derivations[i].derivation_path, derivations[j].derivation_path) != 0) {
                printf("Detected different derivations for the same string '%s':\n", derivations[i].derived_str);
                printf("Derivation 1: %s\n", derivations[i].derivation_path);
                printf("Derivation 2: %s\n", derivations[j].derivation_path);
                return 1;  // Ambiguity found
            }
        }
    }
    return 0;  // No ambiguity found
}

int main() {
    int n;
    printf("Enter the number of productions: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_PRODUCTIONS) {
        printf("Invalid number of productions.\n");
        return 1;
    }

    struct production grammar[MAX_PRODUCTIONS];

    for (int i = 0; i < n; i++) {
        char input[MAX_STRING_LENGTH];
        printf("Enter production %d (format S->aB): ", i + 1);
        scanf("%s", input);

        // Split the input into LHS and RHS based on '->'
        char *arrow = strstr(input, "->");
        if (arrow == NULL || arrow == input || *(arrow + 2) == '\0') {
            printf("Invalid production format. Use the format S->aB.\n");
            return 1;
        }

        grammar[i].lhs = input[0];  // LHS is the first character
        strncpy(grammar[i].rhs, arrow + 2, MAX_STRING_LENGTH - 1);  // RHS is everything after '->'
        grammar[i].rhs[MAX_STRING_LENGTH - 1] = '\0';
    }

    char start;
    printf("Enter the start symbol: ");
    scanf(" %c", &start);

    struct derivation *derivations = malloc(MAX_DERIVATIONS * sizeof(struct derivation));
    if (derivations == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    int derivation_count = 0;

    char start_string[2] = {start, '\0'};
    char initial_path[MAX_PATH_LENGTH] = "";

    // Generate all possible derivations with a depth limit
    generate_strings(grammar, n, start, start_string, MAX_RECURSION_DEPTH, 1, derivations, &derivation_count, initial_path);

    // Check for ambiguity
    int ambiguous = check_ambiguity(derivations, derivation_count);
    
    if (ambiguous) {
        printf("The grammar is ambiguous.\n");
    } else {
        printf("The grammar is not ambiguous (within the tested derivations).\n");
    }

    free(derivations);
    return 0;
}
