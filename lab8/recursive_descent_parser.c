// CS22B1093 ROHAN G

/*
Implement a recursive descent parser for the given context-free grammar (CFG), do the needful and show whether the string is accepted/rejected:



1. S → E  

2. E → E + T | E - T | T  

3. T → T * F | T / F | F  

4. F → ( E ) | - F | + F | N | V  

5. V → x | y | z  

6. N → D | D N  

7. D → 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1024

long int lookahead = 0;
long int string_len;
char string[MAX];

void S();
void E();
void T();
void F();
void V();
void N();
void D();

void removespaces()
{
    while(string[lookahead] == ' '){
        lookahead++;
    }
}

void check()
{
    removespaces();
    if(lookahead == string_len){
        printf("Accepted\n");
        exit(0);
    }
}

void error()
{
    printf("Rejected\n");
    exit(0);
}

void S()
{
    E();
    check();
}

void E()
{
    removespaces();
    T(); 
    removespaces();
    while (string[lookahead] == '+' || string[lookahead] == '-') {  
        lookahead++;
        removespaces();
        T();
        removespaces();
    }
}

void T()
{
    removespaces();
    F();  
    removespaces();
    while (string[lookahead] == '*' || string[lookahead] == '/') {  
        lookahead++;
        removespaces();
        F();
        removespaces();
    }
}

void F()
{
    removespaces();
    if(string[lookahead] == '('){ 
        lookahead++;
        removespaces();
        E();  
        if(string[lookahead] == ')'){
            lookahead++;
            removespaces();
        } else {
            error();  
        }
    } else if (string[lookahead] == '+' || string[lookahead] == '-') { 
        lookahead++;
        removespaces();
        F();  
    } else if (string[lookahead] >= '0' && string[lookahead] <= '9') { 
        N();        
    } else if (string[lookahead] == 'x' || string[lookahead] == 'y' || string[lookahead] == 'z') { 
        V();
    } else {
        error();
    }
}

void V()
{
    if (string[lookahead] == 'x' || string[lookahead] == 'y' || string[lookahead] == 'z') { 
        lookahead++;
        removespaces();
    } else {
        error();
    }
}

void N()
{
    removespaces();
    D(); 
    removespaces();
    while (string[lookahead] >= '0' && string[lookahead] <= '9') {  // N → DN
        D();
        removespaces();
    }
}

void D()
{
    if (string[lookahead] >= '0' && string[lookahead] <= '9') {  // D → 0-9
        lookahead++;
        removespaces();
    } else {
        error();
    }
}

int main(int argc, char* argv[])
{
    scanf("%[^\n]s", string);
    string_len = strlen(string);
    //printf("\nString : %s", string);
    //printf("\nString len : %ld", strlen(string));
    S();
    return 0;
}