// CS22B1093 ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int line = 0;

int getPrec(char operator) {
    if (operator == '^')
        return 3;
    else if (operator == '*' || operator == '/')
        return 2;
    else if (operator == '+' || operator == '-')
        return 1;
    else
        return -1;
}

void inToPost(char *expr, char *postfix) {
    int len = strlen(expr);
    char stack[len];
    int j = 0, top = -1;

    for (int i = 0; i < len; i++) {
        char curr = expr[i];
        
        if (isalnum(curr) || curr == '[' || curr == ']') {
            postfix[j++] = curr;
            if (!(isalnum(expr[i + 1]) || expr[i + 1] == '[' || expr[i + 1] == ']')) {
                postfix[j++] = ' ';
            }
        }
        else if (curr == '(') {
            stack[++top] = '(';
        }
        else if (curr == ')') {
            while (top != -1 && stack[top] != '(')
                postfix[j++] = stack[top--];
            top--;
        }
        else if (curr == '*' && (i == 0 || expr[i - 1] == '(')) {
            postfix[j++] = 'D';
        }
        else {
            while (top != -1 && getPrec(curr) <= getPrec(stack[top]))
                postfix[j++] = stack[top--];
            stack[++top] = curr;
        }
    }
    while (top != -1)
        postfix[j++] = stack[top--];
    postfix[j] = '\0';
}

void genTac(char *postfix) {
    int len = strlen(postfix);
    char stack[len][30];
    int temp = 1;
    int sdx = 0;

    printf("\n----- | -------- | --- | -------- | --------");
    printf("\n%-5s | %-8s | %-3s | %-8s | %-8s\n", "#", "Result", "Op", "Arg1", "Arg2");
    printf("----- | -------- | --- | -------- | --------\n");
    for (int i = 0; i < len; i++) {
        char curr = postfix[i];
        
        if (isalnum(curr)) {
            char op[20];
            int k = 0;

            while (isalnum(postfix[i]) || postfix[i] == '[' || postfix[i] == ']') {
                op[k++] = postfix[i++];
            }
            i--;
            op[k] = '\0';
            strcpy(stack[sdx++], op);
        }
        else if (curr == 'D') {
            char op[20];
            strcpy(op, stack[--sdx]);
            snprintf(stack[sdx++], sizeof(stack[sdx]), "*%s", op);
        }
        else if (curr == ' ') {
            continue;
        }
        else {
            char op2[20], op1[20], res[20];
            strcpy(op2, stack[--sdx]);
            strcpy(op1, stack[--sdx]);
            if (op1[0] == 'D')
                op1[0] = '*';
            if (op2[0] == 'D')
                op2[0] = '*';
            snprintf(res, sizeof(res), "T%d", temp++);
            printf("%-5d | %-8s | %-3c | %-8s | %-8s\n", line, res, curr, op1, op2);
            line++;
            strcpy(stack[sdx++], res);
        }
    }
    char res[30];
    snprintf(res, sizeof(res), "T%d", temp);
    printf("%-5d | %-8s | %-3s | %-8s | %-8s\n", line, res, "=", "x", stack[sdx - 1]);
    printf("----- | -------- | --- | -------- | --------\n");
}

int main() {
    char expr[100];
    printf("Enter the expr: ");
    scanf("%s", expr);
    
    char postfix[100];
    inToPost(expr, postfix);
    printf("\nICG -  Direct and Indirect Tuple:\n");
    genTac(postfix);

    return 0;
}