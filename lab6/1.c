#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void add_sym(char *, char);
void find_first(char *, char);
void find_follow(char *, char);
void show_first();
void show_follow();
int create_table();
void show_table(int);

int top = 0;
int t_cnt, nt_cnt, choice, prod_cnt, f_cnt;
char first[100][100], follow[100][100];
char term[100], nterm[100], prod[100][100], stack[100];
int table[100][100];

// Helper function to find index of non-terminal
int find_nt(char ch) {
    for (int i = 0; i < nt_cnt; i++) {
        if (nterm[i] == ch) {
            return i;
        }
    }
    return -1;
}

void main() {
    int i, j, nt_flag, t_flag, input;
    printf("Enter production rules of grammar in the form A->B\n\n");
    nt_flag = 1;
    t_flag = 1;
    
    while (nt_flag == 1) {
        printf("\n1) Insert Production Rules\n2) Show Grammar\n3) Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &input);
        switch (input) {
        case 1:
            printf("Enter rule %d of grammar: ", prod_cnt + 1);
            scanf("%s", prod[prod_cnt++]);
            for (i = 0; i < nt_cnt && t_flag == 1; i++) {
                if (nterm[i] == prod[prod_cnt - 1][0])
                    t_flag = 0;
            }
            if (t_flag == 1)
                nterm[nt_cnt++] = prod[prod_cnt - 1][0];
            t_flag = 1;
            for (i = 3; prod[prod_cnt - 1][i] != '\0'; i++) {
                if (!isupper(prod[prod_cnt - 1][i]) && prod[prod_cnt - 1][i] != '!') {
                    t_flag = 1; // Reset flag
                    for (j = 0; j < t_cnt && t_flag == 1; j++) {
                        if (term[j] == prod[prod_cnt - 1][i])
                            t_flag = 0;
                    }
                    if (t_flag == 1)
                        term[t_cnt++] = prod[prod_cnt - 1][i];
                    t_flag = 1;
                }
            }
            break;

        case 2:
            if (prod_cnt > 0) {
                printf("\nGrammar");
                printf("\nStarting symbol is: %c", prod[0][0]);
                printf("\nNon-terminals: ");
                for (i = 0; i < nt_cnt; i++)
                    printf("%c  ", nterm[i]);
                printf("\nTerminals: ");
                for (i = 0; i < t_cnt; i++)
                    printf("%c  ", term[i]);
                printf("\nProduction rules: ");
                for (i = 0; i < prod_cnt; i++)
                    printf("%s  ", prod[i]);
                printf("\n");
            } else {
                printf("!Enter at least one production rule");
            }
            break;

        case 3:
            nt_flag = 0;
        }
    }

    // Display the FIRST and FOLLOW sets
    show_first();
    show_follow();

    // Adding end marker '$'
    term[t_cnt++] = '$';
    term[t_cnt] = '\0';

    // Creating LL(1) Parsing Table
    nt_flag = create_table();
    show_table(nt_flag);
}

void show_first() {
    int i, j;
    char arr[100];
    for (i = 0; i < nt_cnt; i++) {
        arr[0] = '\0';
        find_first(arr, nterm[i]);
        for (j = 0; arr[j] != '\0'; j++) {
            first[i][j] = arr[j];
        }
        first[i][j] = '\0';
        f_cnt = 0;
    }
    printf("\nFIRST:\n\n");
    for (i = 0; i < nt_cnt; i++) {
        printf("FIRST( %c ): { ", nterm[i]);
        for (j = 0; first[i][j + 1] != '\0'; j++)
            printf(" %c,", first[i][j]);
        printf(" %c }", first[i][j]);
        printf("\n");
    }
}

void show_follow() {
    int i, j;
    char arr[100];
    for (i = 0; i < nt_cnt; i++) {
        f_cnt = 0;
        arr[0] = '\0';
        find_follow(arr, nterm[i]);
        for (j = 0; arr[j] != '\0'; j++) {
            follow[i][j] = arr[j];
        }
        follow[i][j] = '\0';
    }
    printf("\nFOLLOW:\n\n");
    for (i = 0; i < nt_cnt; i++) {
        printf("FOLLOW( %c ): { ", nterm[i]);
        for (j = 0; follow[i][j + 1] != '\0'; j++)
            printf(" %c,", follow[i][j]);
        printf(" %c }", follow[i][j]);
        printf("\n");
    }
}

void show_table(int flag) {
    int i, j;
    if (flag == 0) {
        printf("\n\nPredictive Parsing Table:\n\n\t");
        for (j = 0; j < t_cnt; j++) {
            printf("\t%c\t", term[j]);
        }
        printf("\n----------------------------------------------------------------------------------------");
        printf("\n\n");
        for (i = 0; i < nt_cnt; i++) {
            printf("%c\t|\t", nterm[i]);
            for (j = 0; j < t_cnt; j++) {
                if (table[i][j] != 0)
                    printf("%s\t\t", prod[table[i][j] - 1]);
                else
                    printf("%c\t\t", '_');
            }
            printf("\n\n");
        }
    }
}

void find_first(char *arr, char ch) {
    int i;
    if (!isupper(ch))
        add_sym(arr, ch);
    else {
        for (i = 0; i < prod_cnt; i++) {
            if (ch == prod[i][0]) {
                if (prod[i][3] == '!') {
                    add_sym(arr, prod[i][3]);
                } else {
                    find_first(arr, prod[i][3]);
                }
            }
        }
    }
}

void find_follow(char arr[], char ch) {
    int i, j, k, l, is_new = 1;
    if (ch == prod[0][0])
        add_sym(arr, '$');
    for (i = 0; i < prod_cnt; i++) {
        char* rhs = prod[i] + 3; // Skip the non-terminal symbol and "->"
        int len = strlen(rhs);

        // Loop through the production to find the current non-terminal
        for (j = 0; j < len; j++) {
            if (rhs[j] == ch) {
                // If there is a terminal or epsilon after the non-terminal
                if (j + 1 < len) {
                    if (islower(rhs[j + 1]) || !isalpha(rhs[j + 1])) {
                        add_sym(arr, rhs[j + 1]);
                    } else {
                        // Handle the case where the next symbol is a non-terminal
                        int next_nt = find_nt(rhs[j + 1]);
                        if (next_nt != -1) {
                            // Add FIRST(next_nt) to FOLLOW(ch), except epsilon
                            for (k = 0; k < strlen(first[next_nt]); k++) {
                                if (first[next_nt][k] != '!') {
                                    add_sym(arr, first[next_nt][k]);
                                }
                            }

                            // If epsilon is in FIRST(next_nt), consider subsequent symbols
                            if (strchr(first[next_nt], '!') != NULL) {
                                int m = j + 2;
                                while (m < len) {
                                    if (islower(rhs[m]) || !isalpha(rhs[m])) {
                                        add_sym(arr, rhs[m]);
                                        break;
                                    } else {
                                        int next_next_nt = find_nt(rhs[m]);
                                        if (next_next_nt != -1) {
                                            // Add FIRST(next_next_nt) to FOLLOW(ch), except epsilon
                                            for (l = 0; l < strlen(first[next_next_nt]); l++) {
                                                if (first[next_next_nt][l] != '!') {
                                                    add_sym(arr, first[next_next_nt][l]);
                                                }
                                            }
                                            if (strchr(first[next_next_nt], '!') == NULL) {
                                                break;
                                            }
                                        }
                                    }
                                    m++;
                                }
                                // If end of production and epsilon, add FOLLOW(i) to FOLLOW(ch)
                                if (m == len && i != find_nt(ch)) {
                                    for (l = 0; l < strlen(follow[find_nt(prod[i][0])]); l++) {
                                        if (follow[find_nt(prod[i][0])][l] != '!') {
                                            add_sym(arr, follow[find_nt(prod[i][0])][l]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else {
                    if (i != find_nt(ch)) {
                        // Add FOLLOW(i) to FOLLOW(ch)
                        for (k = 0; k < strlen(follow[find_nt(prod[i][0])]); k++) {
                            if (follow[find_nt(prod[i][0])][k] != '!') {
                                add_sym(arr, follow[find_nt(prod[i][0])][k]);
                            }
                        }
                    }
                }
            }
        }
    }
}

void add_sym(char arr[], char ch) {
    int i;
    for (i = 0; arr[i] != '\0'; i++) {
        if (arr[i] == ch)
            return; // Avoid duplicates
    }
    arr[i] = ch;
    arr[i + 1] = '\0';
}

int create_table() {
    int i, j, k, l;

    // Initialize the parsing table with zeros (meaning no entry)
    for (i = 0; i < nt_cnt; i++) {
        for (j = 0; j < t_cnt; j++) {
            table[i][j] = 0;
        }
    }

    // Fill the parsing table based on productions
    for (i = 0; i < prod_cnt; i++) {
        int nt_idx = find_nt(prod[i][0]);  // Find index of non-terminal on LHS
        char first_sym = prod[i][3];        // First symbol in production (RHS)

        if (first_sym == '!') {
            // If the production leads to epsilon, add this rule under FOLLOW of the non-terminal
            int follow_nt_idx = find_nt(prod[i][0]);
            for (j = 0; j < strlen(follow[follow_nt_idx]); j++) {
                for (k = 0; k < t_cnt; k++) {
                    if (term[k] == follow[follow_nt_idx][j]) {
                        table[nt_idx][k] = i + 1;   // Store the rule number in the parsing table
                    }
                }
            }
        } else if (islower(first_sym) || !isalpha(first_sym)) {
            // If the first symbol is a terminal, place it in the corresponding terminal column
            for (j = 0; j < t_cnt; j++) {
                if (term[j] == first_sym) {
                    table[nt_idx][j] = i + 1;       // Store the rule number in the parsing table
                }
            }
        } else {
            // If the first symbol is a non-terminal, use its FIRST set
            int first_idx = find_nt(first_sym);
            for (j = 0; j < strlen(first[first_idx]); j++) {
                for (k = 0; k < t_cnt; k++) {
                    if (term[k] == first[first_idx][j] && first[first_idx][j] != '!') {
                        table[nt_idx][k] = i + 1;   // Store the rule number in the parsing table
                    }
                }
            }

            // If epsilon is in the FIRST set of the next non-terminal, use FOLLOW of the current non-terminal
            if (strchr(first[first_idx], '!') != NULL) {
                int follow_nt_idx = find_nt(prod[i][0]);
                for (j = 0; j < strlen(follow[follow_nt_idx]); j++) {
                    for (k = 0; k < t_cnt; k++) {
                        if (term[k] == follow[follow_nt_idx][j]) {
                            table[nt_idx][k] = i + 1;  // Store the rule number in the parsing table
                        }
                    }
                }
            }
        }
    }
    return 0;  // No error
}
