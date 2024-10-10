//CS22B1093 Rohan G

// calculate first of a grammar

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct to store production
struct production { 
    char lhs;
    char rhs[10];
};

// struct to store first set
struct firsts {
    char non_terminal;
    char first[10];
    char follow[10];
};

// add a symbol to first set
void addToFirst(char* firstSet, char symbol) {
    if (!strchr(firstSet, symbol)) {
        strncat(firstSet, &symbol, 1);
    }
}

// calculate first set of a grammar
void first(struct production* p, struct firsts* f, char* non_terminal, char* terminal, int productions, int non_terminals) {
    int updated;

    do {
        updated = 0; 
        // iterate through all productions
        for (int i = 0; i < productions; i++) {
            int lhs_index = 0;
            for (int j = 0; j < non_terminals; j++) {
                if (f[j].non_terminal == p[i].lhs) {
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
                        if (!strchr(f[lhs_index].first, p[i].rhs[k])) {
                            addToFirst(f[lhs_index].first, p[i].rhs[k]);
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
                    if (f[n].non_terminal == p[i].rhs[k]) {
                        rhs_index = n;
                        break;
                    }
                }
                // add first set of rhs to first set of lhs
                for (int j = 0; j < strlen(f[rhs_index].first); j++) {
                    if (f[rhs_index].first[j] != 'e') {
                        if (!strchr(f[lhs_index].first, f[rhs_index].first[j])) {
                            addToFirst(f[lhs_index].first, f[rhs_index].first[j]);
                            updated = 1;
                        }
                    }
                }
                
                // if first symbol of rhs is epsilon, add epsilon to first set of lhs
                if (strchr(f[rhs_index].first, 'e')) {
                    k++;
                    if (p[i].rhs[k] == '\0') {
                        if (!strchr(f[lhs_index].first, 'e')) {
                            addToFirst(f[lhs_index].first, 'e');
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

void follow(struct production* p, struct firsts* f, char* non_terminal, char* terminal, int productions, int non_terminals){
    int updated;
    
    addToFirst(f[0]. follow,'$');

    do{
        updated = 0;
        for(int i = 0; i < productions; i++){
            for(int j = 0; j < strlen(p[i].rhs); j++){
                if(p[i].rhs[j] == f[i].non_terminal){
                    int lhs_index = j;

                    if(p[i].rhs[j+1] != '\0'){
                        int next_symbol = -1;
                        for(int k = 0; k < non_terminals; k++){
                            if(p[i].rhs[j+1] == f[k].non_terminal){
                                next_symbol = k;
                                break;
                            }
                        }

                        if(next_symbol == -1){

                        }
                    }if(!strchr(f[lhs_index].follow,p[i].rhs[j+1])){
                        addToFirst(f[lhs_index].follow,p[i].rhs[j+1]);
                        updated = 1;
                    }
                }
            }
        }
    }while(updated);
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
    struct firsts f[non_terminals];

    for (int i = 0; i < non_terminals; i++) {
        f[i].non_terminal = non_terminal[i];
        f[i].first[0] = '\0';
    }

    printf("Enter the productions in the form A->aB\n");
    for (int i = 0; i < productions; i++) {
        printf("Enter the production %d: ", i + 1);
        scanf(" %c->%s", &p[i].lhs, p[i].rhs);
    }

    printf("The productions are:\n");
    for (int i = 0; i < productions; i++) {
        printf("%c->%s\n", p[i].lhs, p[i].rhs);
    }

    first(p, f, non_terminal, terminal, productions, non_terminals);

    printf("First sets:\n");
    for (int i = 0; i < non_terminals; i++) {
        printf("First(%c) = { }", f[i].non_terminal);
        for (int j = 0; j < strlen(f[i].first); j++) {
            printf("%c ", f[i].first[j]);
        }
        printf("\n");
    }

    follow(p, f ,non_terminal, terminal, productions, non_terminals);

    return 0;
}