#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

struct tree {
    char symbol;
    struct tree *left;
    struct tree *right;
};

char postfix[MAX];
int temp_count = 1;

// Stack structure for expression evaluation
struct Stack {
    int top;
    struct tree* items[MAX];
};

// Initialize empty stack
void initStack(struct Stack* s) {
    s->top = -1;
}

// Push to stack
void push(struct Stack* s, struct tree* item) {
    s->items[++(s->top)] = item;
}

// Pop from stack
struct tree* pop(struct Stack* s) {
    if (s->top == -1) return NULL;
    return s->items[(s->top)--];
}

// Create new tree node
struct tree* createNode(char symbol) {
    struct tree* node = (struct tree*)malloc(sizeof(struct tree));
    node->symbol = symbol;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Check if character is operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '^');
}

// Get operator precedence
int precedence(char op) {
    switch (op) {
        case '^': return 3;
        case '*':
        case '/': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

// Convert infix to postfix
void infixToPostfix(char* infix, char* postfix) {
    char stack[MAX];
    int top = -1;
    int j = 0;
    
    for (int i = 0; infix[i]; i++) {
        if (infix[i] == ' ') continue;
        
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i];
        }
        else if (infix[i] == '(') {
            stack[++top] = infix[i];
        }
        else if (infix[i] == ')') {
            while (top >= 0 && stack[top] != '(')
                postfix[j++] = stack[top--];
            if (top >= 0) top--;  // Pop '('
        }
        else if (isOperator(infix[i])) {
            while (top >= 0 && precedence(stack[top]) >= precedence(infix[i]))
                postfix[j++] = stack[top--];
            stack[++top] = infix[i];
        }
    }
    
    while (top >= 0)
        postfix[j++] = stack[top--];
    postfix[j] = '\0';
}

// Build expression tree from postfix
struct tree* buildExpressionTree(char* postfix) {
    struct Stack stack;
    initStack(&stack);
    
    for (int i = 0; postfix[i]; i++) {
        struct tree* node = createNode(postfix[i]);
        
        if (isOperator(postfix[i])) {
            node->right = pop(&stack);
            node->left = pop(&stack);
        }
        
        push(&stack, node);
    }
    
    return pop(&stack);
}

// Generate three address code
void generateCode(struct tree* root) {
    if (root == NULL) return;
    
    generateCode(root->left);
    generateCode(root->right);
    
    if (isOperator(root->symbol)) {
        if (root->symbol == '=' && isalnum(root->left->symbol)) {
            printf("%c = ", root->left->symbol);
            if (root->right->symbol == 't')
                printf("t%d\n", temp_count - 1);
            else
                printf("%c\n", root->right->symbol);
        }
        else {
            printf("t%d = ", temp_count);
            
            if (root->left->symbol == 't')
                printf("t%d ", temp_count - 2);
            else
                printf("%c ", root->left->symbol);
            
            printf("%c ", root->symbol);
            
            if (root->right->symbol == 't')
                printf("t%d\n", temp_count - 1);
            else
                printf("%c\n", root->right->symbol);
                
            root->symbol = 't';
            temp_count++;
        }
    }
}

// Free the expression tree
void freeTree(struct tree* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    char infix[MAX];
    char postfix[MAX];
    
    printf("Enter an arithmetic expression: ");
    scanf(" %[^\n]s", infix);
    
    printf("\nInput Expression: %s\n", infix);
    printf("\nThree Address Code:\n");
    
    // Convert to postfix
    infixToPostfix(infix, postfix);
    
    // Build expression tree
    struct tree* root = buildExpressionTree(postfix);
    
    // Generate three address code
    generateCode(root);
    
    // Free memory
    freeTree(root);
    
    return 0;
}