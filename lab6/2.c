//CS22B1093 ROHAN G
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
int t_cnt, nt_cnt, ch, prod_cnt, count;
char FIRST[100][100], FOLLOW[100][100];
char T[100], NT[100], G[100][100], STACK[100];
int LL1[100][100];

int find_nt(char ch) {
    for (int i = 0; i < nt_cnt; i++) {
        if (NT[i] == ch) {
            return i;
        }
    }
    return -1;
}

void main() {
    int i, j, flag, fl, ch1;
    printf("Enter production rules of grammar in the form A->B\n\n");
    flag = 1;
    fl = 1;
    while (flag == 1) {
        printf("\n1) Enter Production Rules\n2) Show all the Grammar\n3) Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &ch1);
        switch (ch1) {
        case 1:
            printf("Enter number %d rules of grammar: ", prod_cnt + 1);
            scanf("%s", G[prod_cnt++]);
            for (i = 0; i < nt_cnt && fl == 1; i++) {
                if (NT[i] == G[prod_cnt - 1][0])
                    fl = 0;
            }
            if (fl == 1)
                NT[nt_cnt++] = G[prod_cnt - 1][0];
            fl = 1;
            for (i = 3; G[prod_cnt - 1][i] != '\0'; i++) {
                if (!isupper(G[prod_cnt - 1][i]) && G[prod_cnt - 1][i] != '#') {
                    for (j = 0; j < t_cnt && fl == 1; j++) {
                        if (T[j] == G[prod_cnt - 1][i])
                            fl = 0;
                    }
                    if (fl == 1)
                        T[t_cnt++] = G[prod_cnt - 1][i];
                    fl = 1;
                }
            }
            break;

        case 2:
            if (prod_cnt > 0) {
                printf("\nGrammar");
                printf("\nStarting symbol is: %c", G[0][0]);
                printf("\nNon-terminal symbol is: ");
                for (i = 0; i < nt_cnt; i++)
                    printf("%c  ", NT[i]);
                printf("\nTerminal symbol is: ");
                for (i = 0; i < t_cnt; i++)
                    printf("%c  ", T[i]);
                printf("\nProduction rules: ");
                for (i = 0; i < prod_cnt; i++)
                    printf("%s  ", G[i]);
                printf("\n");
            } else {
                printf("!Enter at least one production rule");
            }
            break;

        case 3:
            flag = 0;
        }
    }

    // Display the FIRST and FOLLOW sets
    show_first();
    show_follow();

    // Adding end marker '$'
    T[t_cnt++] = '$';
    T[t_cnt] = '\0';

    // Creating LL(1) Parsing Table
    flag = create_table();
    show_table(flag);
}

void show_first() {
    int i, j;
    char arr[100];
    for (i = 0; i < nt_cnt; i++) {
        arr[0] = '\0';
        find_first(arr, NT[i]);
        for (j = 0; arr[j] != '\0'; j++) {
            FIRST[i][j] = arr[j];
        }
        FIRST[i][j] = '\0';
        count = 0;
    }
    printf("\nFIRST:\n\n");
    for (i = 0; i < nt_cnt; i++) {
        printf("FIRST( %c ): { ", NT[i]);
        for (j = 0; FIRST[i][j + 1] != '\0'; j++)
            printf(" %c,", FIRST[i][j]);
        printf(" %c }", FIRST[i][j]);
        printf("\n");
    }
}

void show_follow() {
    int i, j;
    char arr[100];
    for (i = 0; i < nt_cnt; i++) {
        count = 0;
        arr[0] = '\0';
        find_follow(arr, NT[i]);
        for (j = 0; arr[j] != '\0'; j++) {
            FOLLOW[i][j] = arr[j];
        }
        FOLLOW[i][j] = '\0';
    }
    printf("\nFOLLOW:\n\n");
    for (i = 0; i < nt_cnt; i++) {
        printf("FOLLOW( %c ): { ", NT[i]);
        for (j = 0; FOLLOW[i][j + 1] != '\0'; j++)
            printf(" %c,", FOLLOW[i][j]);
        printf(" %c }", FOLLOW[i][j]);
        printf("\n");
    }
}

void show_table(int flag) {
    int i, j;
    if (flag == 0) {
        printf("\n\nPredictive Parsing Table:\n\n\t");
        for (j = 0; j < t_cnt; j++) {
            printf("\t%c\t", T[j]);
        }
        printf("\n----------------------------------------------------------------------------------------");
        printf("\n\n");
        for (i = 0; i < nt_cnt; i++) {
            printf("%c\t|\t", NT[i]);
            for (j = 0; j < t_cnt; j++) {
                if (LL1[i][j] != 0)
                    printf("%s\t\t", G[LL1[i][j] - 1]);
                else
                    printf("%c\t\t", '_');
            }
            printf("\n\n");
        }
    }
}

void find_first(char *arr, char ch) {
    int i, j;
    if (!isupper(ch)) {
        add_sym(arr, ch);
    } else {
        for (i = 0; i < prod_cnt; i++) {
            if (ch == G[i][0]) {
                char* prod = G[i] + 3; 
                int all_null = 1;

                for (j = 0; prod[j] != '\0'; j++) {
                    if (prod[j] == '#') {
                        add_sym(arr, '#'); 
                        break; 
                    } else if (islower(prod[j])) {
                        add_sym(arr, prod[j]);
                        all_null = 0;
                        break; 
                    } else {
                        int next_nt = find_nt(prod[j]);
                        if (next_nt != -1) {
                            char next_first[100];
                            next_first[0] = '\0';
                            find_first(next_first, prod[j]);

                            for (int k = 0; next_first[k] != '\0'; k++) {
                                if (next_first[k] != '#') {
                                    add_sym(arr, next_first[k]);
                                }
                            }
                            if (strchr(next_first, '#') == NULL) {
                                all_null = 0; 
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void find_follow(char arr[], char ch) {
    int i, j, k;

    if (ch == G[0][0])  
        add_sym(arr, '$');

    for (i = 0; i < prod_cnt; i++) {
        char* prod = G[i] + 3; 
        for (j = 0; prod[j] != '\0'; j++) {
            if (prod[j] == ch) {
                if (prod[j + 1] != '\0') {
                    if (islower(prod[j + 1])) {
                        add_sym(arr, prod[j + 1]);
                    } else {
                        int next_nt_idx = find_nt(prod[j + 1]);
                        if (next_nt_idx != -1) {
                            for (k = 0; FIRST[next_nt_idx][k] != '\0'; k++) {
                                if (FIRST[next_nt_idx][k] != '#') {
                                    add_sym(arr, FIRST[next_nt_idx][k]);
                                }
                            }
                            if (strchr(FIRST[next_nt_idx], '#') != NULL) {
                                // Handle subsequent symbols
                                for (k = j + 2; prod[k] != '\0'; k++) {
                                    if (islower(prod[k])) {
                                        add_sym(arr, prod[k]);
                                        break;
                                    }
                                    int next_next_nt = find_nt(prod[k]);
                                    if (next_next_nt != -1) {
                                        for (int m = 0; FIRST[next_next_nt][m] != '\0'; m++) {
                                            if (FIRST[next_next_nt][m] != '#') {
                                                add_sym(arr, FIRST[next_next_nt][m]);
                                            }
                                        }
                                        if (strchr(FIRST[next_next_nt], '#') == NULL) {
                                            break;
                                        }
                                    }
                                }
                                if (prod[k] == '\0' && i != find_nt(ch)) {
                                    int follow_idx = find_nt(G[i][0]);
                                    for (k = 0; FOLLOW[follow_idx][k] != '\0'; k++) {
                                        add_sym(arr, FOLLOW[follow_idx][k]);
                                    }
                                }
                            }
                        }
                    }
                } else if (i != find_nt(ch)) {
                    int follow_idx = find_nt(G[i][0]);
                    for (k = 0; FOLLOW[follow_idx][k] != '\0'; k++) {
                        add_sym(arr, FOLLOW[follow_idx][k]);
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
            return;
    }
    arr[i] = ch;
    arr[i + 1] = '\0';
}

int create_table() {
    int i, j, k;

    for (i = 0; i < nt_cnt; i++) {
        for (j = 0; j < t_cnt; j++) {
            LL1[i][j] = 0;
        }
    }

    for (i = 0; i < prod_cnt; i++) {
        int nt_idx = find_nt(G[i][0]);  
        char* prod = G[i] + 3;           

        int first_found = 0;
        for (j = 0; prod[j] != '\0'; j++) {
            if (prod[j] == '#') { 
                first_found = 1; 
                break;
            } else if (islower(prod[j])) { 
                for (k = 0; k < t_cnt; k++) {
                    if (T[k] == prod[j]) {
                        LL1[nt_idx][k] = i + 1; 
                    }
                }
                first_found = 1; 
                break; 
            } else { 
                int first_nt_idx = find_nt(prod[j]);
                for (k = 0; FIRST[first_nt_idx][k] != '\0'; k++) {
                    if (FIRST[first_nt_idx][k] != '#') {
                        for (int l = 0; l < t_cnt; l++) {
                            if (T[l] == FIRST[first_nt_idx][k]) {
                                LL1[nt_idx][l] = i + 1; 
                            }
                        }
                    }
                }
                if (strchr(FIRST[first_nt_idx], '#') == NULL) {
                    break;
                }
            }
        }

        if (first_found && strchr(prod, '#') != NULL) {
            int follow_nt_idx = find_nt(G[i][0]);
            for (j = 0; FOLLOW[follow_nt_idx][j] != '\0'; j++) {
                for (k = 0; k < t_cnt; k++) {
                    if (T[k] == FOLLOW[follow_nt_idx][j]) {
                        LL1[nt_idx][k] = i + 1; 
                    }
                }
            }
        }
    }
    return 0; 
}
