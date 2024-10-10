#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_RULES 10
#define MAX_SYMBOLS 26
#define MAX_RULE_LENGTH 50
#define MAX_STRING_LENGTH 100

struct Rule {
    char left;
    char right[MAX_RULE_LENGTH];
};

struct Rule grammar[MAX_RULES];
int num_rules = 0;
bool first[MAX_SYMBOLS][MAX_SYMBOLS];
bool follow[MAX_SYMBOLS][MAX_SYMBOLS];
int parse_table[MAX_SYMBOLS][MAX_SYMBOLS];

void add_to_first(int left, int symbol) {
    if (!first[left][symbol]) {
        first[left][symbol] = true;
    }
}

void add_to_follow(int left, int symbol) {
    if (!follow[left][symbol]) {
        follow[left][symbol] = true;
    }
}

void compute_first_sets() {
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < num_rules; i++) {
            char left = grammar[i].left - 'A';
            char *right = grammar[i].right;

            if (right[0] == 'e') {
                if (!first[left]['e' - 'A']) {
                    first[left]['e' - 'A'] = true;
                    changed = true;
                }
            } else {
                for (int j = 0; right[j] != '\0'; j++) {
                    if (isupper(right[j])) {
                        for (int k = 0; k < MAX_SYMBOLS; k++) {
                            if (first[right[j] - 'A'][k] && k != 'e' - 'A' && !first[left][k]) {
                                first[left][k] = true;
                                changed = true;
                            }
                        }
                        if (!first[right[j] - 'A']['e' - 'A']) break;
                    } else {
                        if (!first[left][right[j] - 'a']) {
                            first[left][right[j] - 'a'] = true;
                            changed = true;
                        }
                        break;
                    }
                }
            }
        }
    } while (changed);
}

void compute_follow_sets() {
    follow[grammar[0].left - 'A']['$' - 'A'] = true;

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < num_rules; i++) {
            char left = grammar[i].left - 'A';
            char *right = grammar[i].right;

            for (int j = 0; right[j] != '\0'; j++) {
                if (isupper(right[j])) {
                    int next = j + 1;
                    bool added_follow = false;

                    while (right[next] != '\0') {
                        if (isupper(right[next])) {
                            for (int k = 0; k < MAX_SYMBOLS; k++) {
                                if (first[right[next] - 'A'][k] && k != 'e' - 'A' && !follow[right[j] - 'A'][k]) {
                                    follow[right[j] - 'A'][k] = true;
                                    changed = true;
                                }
                            }
                            if (!first[right[next] - 'A']['e' - 'A']) {
                                added_follow = true;
                                break;
                            }
                        } else {
                            if (!follow[right[j] - 'A'][right[next] - 'a']) {
                                follow[right[j] - 'A'][right[next] - 'a'] = true;
                                changed = true;
                            }
                            added_follow = true;
                            break;
                        }
                        next++;
                    }

                    if (!added_follow) {
                        for (int k = 0; k < MAX_SYMBOLS; k++) {
                            if (follow[left][k] && !follow[right[j] - 'A'][k]) {
                                follow[right[j] - 'A'][k] = true;
                                changed = true;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);
}

void build_parse_table() {
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            parse_table[i][j] = -1;
        }
    }

    for (int i = 0; i < num_rules; i++) {
        char left = grammar[i].left - 'A';
        char *right = grammar[i].right;

        if (right[0] == 'e') {
            for (int j = 0; j < MAX_SYMBOLS; j++) {
                if (follow[left][j]) {
                    parse_table[left][j] = i;
                }
            }
        } else {
            for (int j = 0; j < MAX_SYMBOLS; j++) {
                if (isupper(right[0])) {
                    if (first[right[0] - 'A'][j] && j != 'e' - 'A') {
                        parse_table[left][j] = i;
                    }
                } else {
                    if (right[0] - 'a' == j) {
                        parse_table[left][j] = i;
                    }
                }
            }

            if (isupper(right[0]) && first[right[0] - 'A']['e' - 'A']) {
                for (int j = 0; j < MAX_SYMBOLS; j++) {
                    if (follow[left][j]) {
                        parse_table[left][j] = i;
                    }
                }
            }
        }
    }
}

bool parse(char *input) {
    char stack[MAX_STRING_LENGTH];
    int top = 0;
    int input_pos = 0;

    stack[top++] = '$';
    stack[top++] = grammar[0].left;

    while (top > 0) {
        if (stack[top - 1] == '$' && input[input_pos] == '\0') {
            return true;
        }

        if (stack[top - 1] == 'e') {
            top--;
            continue;
        }

        if (!isupper(stack[top - 1])) {
            if (stack[top - 1] == input[input_pos]) {
                top--;
                input_pos++;
            } else {
                return false;
            }
        } else {
            int rule = parse_table[stack[top - 1] - 'A'][input[input_pos] - 'a'];
            if (rule == -1) {
                return false;
            }
            top--;
            char *right = grammar[rule].right;
            if (strcmp(right, "e") != 0) {
                for (int i = strlen(right) - 1; i >= 0; i--) {
                    stack[top++] = right[i];
                }
            }
        }
    }

    return input[input_pos] == '\0';
}

int main() {
    printf("Enter the number of grammar rules: ");
    scanf("%d", &num_rules);
    getchar(); // Consume newline

    printf("Enter the grammar rules (e.g., S->aSbS):\n");
    for (int i = 0; i < num_rules; i++) {
        char rule[MAX_RULE_LENGTH];
        fgets(rule, sizeof(rule), stdin);
        rule[strcspn(rule, "\n")] = 0; // Remove newline

        grammar[i].left = rule[0];
        strcpy(grammar[i].right, rule + 3);
    }

    compute_first_sets();
    compute_follow_sets();
    build_parse_table();

    char input[MAX_STRING_LENGTH];
    printf("Enter the input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline

    if (parse(input)) {
        printf("The string is accepted.\n");
    } else {
        printf("The string is rejected.\n");
    }

    return 0;
}