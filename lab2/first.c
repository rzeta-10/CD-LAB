// caclulate the first of a given grammar

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct production{
    char lhs;
    char rhs[10];
};

struct firsts{
    char non_terminal;
    char first[10];
};

int main(){
    int non_terminals , terminals, productions;
    printf("Enter the number of non-terminals: ");
    scanf("%d", &non_terminals);
    printf("Enter the number of terminals: ");
    scanf("%d", &terminals);

    char non_terminal[non_terminals];
    char terminal[terminals];

    for(int i = 0; i<non_terminals; i++){
        printf("Enter the non-terminal %d: ", i+1);
        scanf(" %c", &non_terminal[i]);
    }

    for(int i = 0; i<terminals; i++){
        printf("Enter the terminal %d: ", i+1);
        scanf(" %c", &terminal[i]);
    }

    printf("Enter the number of productions: ");
    scanf("%d", &productions);    

    struct production p[productions];

    struct firsts f[non_terminals];

    printf("Enter the productions in the form A->aB\n");
    for(int i = 0; i<productions; i++){
        printf("Enter the production %d: ", i+1);
        scanf(" %c->%s", &p[i].lhs, p[i].rhs);
    }

    printf("The productions are:\n");
    for(int i = 0; i<productions; i++){
        printf("%c->%s\n", p[i].lhs, p[i].rhs);
    }

    for(int i = 0; i<productions; i++){
        first(p,f,non_terminal,terminal,productions,non_terminals);
    }
}