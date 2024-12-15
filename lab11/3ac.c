#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

struct Node {
    char data;
    char temp[3];
    struct Node *left, *right;
};

struct Stack {
    struct Node *items[MAX];
    int top;
};

struct TAC {
    int id;
    char op;
    char arg1[3];
    char arg2[3];
} arr[MAX];

int tempNum = 1;
int idx = 0;

void initStack(struct Stack *stk) {
    stk->top = -1;
}

void push(struct Stack *stk, struct Node *node) {
    if (stk->top < MAX - 1) {
        stk->items[++stk->top] = node;
    }
}

struct Node* pop(struct Stack *stk) {
    return (stk->top >= 0) ? stk->items[stk->top--] : NULL;
}

int isOp(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '%');
}

int prec(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

void inToPost(char *infix, char *postfix) {
    char stk[MAX];
    int top = -1, k = 0;
    for (int i = 0; infix[i]; i++) {
        if (infix[i] == ' ') continue;
        if (isalnum(infix[i])) {
            postfix[k++] = infix[i];
        } else if (infix[i] == '(') {
            stk[++top] = infix[i];
        } else if (infix[i] == ')') {
            while (top != -1 && stk[top] != '(') {
                postfix[k++] = stk[top--];
            }
            top--;
        } else if (isOp(infix[i])) {
            while (top != -1 && prec(stk[top]) >= prec(infix[i])) {
                postfix[k++] = stk[top--];
            }
            stk[++top] = infix[i];
        }
    }
    while (top != -1) {
        postfix[k++] = stk[top--];
    }
    postfix[k] = '\0';
}

struct Node* buildTree(char *postfix) {
    struct Stack stk;
    initStack(&stk);

    for (int i = 0; postfix[i]; i++) {
        struct Node *node = (struct Node *)malloc(sizeof(struct Node));
        node->data = postfix[i];
        node->left = node->right = NULL;
        strcpy(node->temp, "");

        if (isOp(postfix[i])) {
            node->right = pop(&stk);
            node->left = pop(&stk);
        }
        push(&stk, node);
    }
    return pop(&stk);
}

void generateTAC(struct Node *root) {
    if (root == NULL) return;
    if (isOp(root->data)) {
        generateTAC(root->left);
        generateTAC(root->right);

        char *leftArg = root->left->temp[0] ? root->left->temp : (char[]){root->left->data, '\0'};
        char *rightArg = root->right->temp[0] ? root->right->temp : (char[]){root->right->data, '\0'};

        printf("t%d = %s %c %s\n", tempNum, leftArg, root->data, rightArg);

        arr[idx].id = tempNum;
        arr[idx].op = root->data;
        strcpy(arr[idx].arg1, leftArg);
        strcpy(arr[idx].arg2, rightArg);
        sprintf(root->temp, "t%d", tempNum); // Assign temp variable to node
        idx++;
        tempNum++;
    }
}

void printTAC(char resultVar) {
    printf("ID\tOP\tARG1\tARG2\n");
    for (int i = 0; i < idx; i++) {
        printf("%d\t%c\t%s\t%s\n", arr[i].id, arr[i].op, arr[i].arg1, arr[i].arg2);
    }
    // Final assignment row for the result
    printf("%d\t=\t%s\t\n", tempNum, arr[idx - 1].arg1);
}

int main() {
    char infix[MAX];
    printf("Enter the expression: ");
    scanf("%[^\n]", infix);

    char postfix[MAX];
    inToPost(infix, postfix);

    struct Node *root = buildTree(postfix);
    printf("\nThree-Address Code:\n");
    generateTAC(root);

    // Final assignment of result
    printf("%c = %s\n", infix[0], root->temp);

    printf("\nIntermediate Representation:\n");
    printTAC(infix[0]);

    return 0;
}
