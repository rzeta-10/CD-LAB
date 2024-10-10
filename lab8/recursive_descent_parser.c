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
#include <unistd.h>
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

void check()
{
    if(lookahead == string_len){
        printf("\nAccepted");
        exit(0);
    }
}

void error()
{
    printf("\nRejected");
    exit(0);
}

void S()
{
    E();
    check();
}

void E()
{
    T();
    while(string[lookahead] == '+' || string[lookahead] == '-'){
        lookahead++;
        T();
    }
}

void T()
{
    F();
    while(string[lookahead] == '*' || string[lookahead] == '/'){
        lookahead++;
        F();
    }
}

void F()
{
    if(string[lookahead]=='('){
        lookahead++;
        E();
        if(string[lookahead] == ')'){
            lookahead++;
        }else{
            error();
        }
    }else if(string[lookahead] == '-' || string[lookahead] == '+'){
        lookahead++;
        F();
    }else if(string[lookahead] >= '0' && string[lookahead] <= '9'){
        N();        
    }else if(string[lookahead] == 'x' || string[lookahead] == 'y' || string[lookahead] == 'z'){
        V();
    }else{
        error();
    }
}

void V()
{
    if(string[lookahead] =='x' || string[lookahead] == 'y' || string[lookahead] == 'z'){
        lookahead++;
    }else{
        error();
    }
}

void N()
{
    D();
    while (string[lookahead] >= '0' && string[lookahead] <= '9'){
        D();
    }
}


void D()
{
    if(string[lookahead]>='0' && string[lookahead] <= '9'){
        lookahead++;
    }else{
        error();
    }
}
    
int main(int argc, char* argv[])
{
    printf("Enter input string : ");
    scanf("%s",string);
    //printf("\nString : %s", string);
    //printf("\nString len : %ld", strlen(string));
    string_len = strlen(string);
    
    S();
    return 0;
}
