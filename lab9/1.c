#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RULES 100
#define MAX_SYMBOLS 50
#define MAX_STATES 50
#define MAX_ITEMS 100

typedef struct {
    char lhs;
    char rhs[MAX_SYMBOLS];
} Production;

typedef struct {
    int prodIndex;
    int dotPosition;
    char lookahead;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int itemCount;
} State;

Production grammar[MAX_RULES];
int numProductions = 0;
char terminals[MAX_SYMBOLS], nonTerminals[MAX_SYMBOLS];
int numTerminals = 0, numNonTerminals = 0;
State states[MAX_STATES];
int numStates = 0;
int action[MAX_STATES][MAX_SYMBOLS], gotoTable[MAX_STATES][MAX_SYMBOLS];
char inputString[MAX_SYMBOLS];

// Function to add a production rule
void addProduction(char lhs, const char* rhs) {
    grammar[numProductions].lhs = lhs;
    strcpy(grammar[numProductions].rhs, rhs);
    numProductions++;
}

// Function to check if a symbol is a terminal
int isTerminal(char symbol) {
    for (int i = 0; i < numTerminals; i++) {
        if (terminals[i] == symbol) {
            return 1;
        }
    }
    return 0;
}

// Function to add a terminal symbol
void addTerminal(char symbol) {
    terminals[numTerminals++] = symbol;
}

// Function to add a non-terminal symbol
void addNonTerminal(char symbol) {
    nonTerminals[numNonTerminals++] = symbol;
}

// Function to create a closure for a state
void closure(State* state) {
    int changed;
    do {
        changed = 0;
        for (int i = 0; i < state->itemCount; i++) {
            Item currentItem = state->items[i];
            char symbolAfterDot = grammar[currentItem.prodIndex].rhs[currentItem.dotPosition];
            
            if (symbolAfterDot != '\0' && !isTerminal(symbolAfterDot)) {
                for (int j = 0; j < numProductions; j++) {
                    if (grammar[j].lhs == symbolAfterDot) {
                        Item newItem = {j, 0, currentItem.lookahead};
                        int alreadyExists = 0;
                        for (int k = 0; k < state->itemCount; k++) {
                            if (state->items[k].prodIndex == newItem.prodIndex &&
                                state->items[k].dotPosition == newItem.dotPosition &&
                                state->items[k].lookahead == newItem.lookahead) {
                                alreadyExists = 1;
                                break;
                            }
                        }
                        if (!alreadyExists) {
                            state->items[state->itemCount++] = newItem;
                            changed = 1;
                        }
                    }
                }
            }
        }
    } while (changed);
}

// Function to compute the GOTO for a state and symbol
int GOTO(State* state, char symbol, State* newState) {
    newState->itemCount = 0;
    for (int i = 0; i < state->itemCount; i++) {
        Item currentItem = state->items[i];
        char symbolAfterDot = grammar[currentItem.prodIndex].rhs[currentItem.dotPosition];
        
        if (symbolAfterDot == symbol) {
            Item newItem = {currentItem.prodIndex, currentItem.dotPosition + 1, currentItem.lookahead};
            newState->items[newState->itemCount++] = newItem;
        }
    }
    if (newState->itemCount > 0) {
        closure(newState);
        return 1;
    }
    return 0;
}

// Function to create the canonical collection of LR(0) items
void createCanonicalCollection() {
    State initialState;
    initialState.itemCount = 0;
    initialState.items[initialState.itemCount++] = (Item){0, 0, '$'};
    closure(&initialState);
    states[numStates++] = initialState;

    int changed;
    do {
        changed = 0;
        for (int i = 0; i < numStates; i++) {
            for (int j = 0; j < numTerminals + numNonTerminals; j++) {
                char symbol = (j < numTerminals) ? terminals[j] : nonTerminals[j - numTerminals];
                State newState;
                if (GOTO(&states[i], symbol, &newState)) {
                    int alreadyExists = -1;
                    for (int k = 0; k < numStates; k++) {
                        if (memcmp(&states[k], &newState, sizeof(State)) == 0) {
                            alreadyExists = k;
                            break;
                        }
                    }
                    if (alreadyExists == -1) {
                        // Ensure state transition is added correctly
                        states[numStates++] = newState;
                        changed = 1;
                    }
                }
            }
        }
    } while (changed);
}

// Function to build the LALR parsing table with accurate transitions
void buildParsingTable() {
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            action[i][j] = -1;  // Initialize action table to -1 (indicating no action)
            gotoTable[i][j] = -1;  // Initialize goto table to -1 (indicating no transition)
        }
    }

    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < states[i].itemCount; j++) {
            Item item = states[i].items[j];
            int prodLen = strlen(grammar[item.prodIndex].rhs);

            // Check if dot is at the end of a production (reduce action)
            if (item.dotPosition == prodLen) {
                int index = (item.lookahead == '$') ? numTerminals : item.lookahead;
                action[i][index] = -(item.prodIndex);  // Indicate a reduce action
            } else {
                // Handle shift actions
                char nextSymbol = grammar[item.prodIndex].rhs[item.dotPosition];
                if (isTerminal(nextSymbol)) {
                    State newState;
                    int gotoState = GOTO(&states[i], nextSymbol, &newState);
                    if (gotoState) {
                        int index = (nextSymbol == '$') ? numTerminals : nextSymbol;
                        action[i][index] = gotoState;  // Shift action
                    }
                } else {
                    // Handle goto actions for non-terminals
                    State newState;
                    int gotoState = GOTO(&states[i], nextSymbol, &newState);
                    if (gotoState) {
                        int index = nextSymbol;
                        gotoTable[i][index] = gotoState;  // Goto transition
                    }
                }
            }
        }
    }
}

// Add logging to visualize state generation and the corresponding table
void printParsingTable() {
    printf("\nParsing Table:\n");
    printf("State\t");
    for (int i = 0; i < numTerminals; i++) {
        printf("%c\t", terminals[i]);
    }
    printf("$\t");
    for (int i = 0; i < numNonTerminals; i++) {
        printf("%c\t", nonTerminals[i]);
    }
    printf("\n");

    for (int i = 0; i < numStates; i++) {
        printf("%d\t", i);
        for (int j = 0; j < numTerminals; j++) {
            if (action[i][terminals[j]] == -1) {
                printf("-\t");
            } else if (action[i][terminals[j]] < 0) {
                printf("r%d\t", -action[i][terminals[j]]);
            } else {
                printf("s%d\t", action[i][terminals[j]]);
            }
        }
        if (action[i]['$'] == -1) {
            printf("-\t");
        } else if (action[i]['$'] < 0) {
            printf("r%d\t", -action[i]['$']);
        } else {
            printf("s%d\t", action[i]['$']);
        }
        for (int j = 0; j < numNonTerminals; j++) {
            if (gotoTable[i][nonTerminals[j]] == -1) {
                printf("-\t");
            } else {
                printf("%d\t", gotoTable[i][nonTerminals[j]]);
            }
        }
        printf("\n");
    }
}

// Function to parse the input string using the LALR table
int parseInput(const char* input) {
    int stack[MAX_SYMBOLS], top = 0;
    stack[top++] = 0;
    int i = 0;
    
    while (1) {
        int state = stack[top - 1];
        char symbol = input[i];
        int actionValue = action[state][symbol];
        
        if (actionValue > 0) {
            stack[top++] = actionValue;
            i++;
        } else if (actionValue < 0) {
            int prodIndex = -actionValue;
            int len = strlen(grammar[prodIndex].rhs);
            top -= len;
            int nonTerminalState = gotoTable[stack[top - 1]][grammar[prodIndex].lhs];
            stack[top++] = nonTerminalState;
        } else {
            if (symbol == '$' && action[state][symbol] == 0) {
                return 1;
            }
            return 0;
        }
    }
}

int main() {
    int numRules;
    char lhs, rhs[MAX_SYMBOLS];
    
    // Input grammar
    printf("Enter number of productions: ");
    scanf("%d", &numRules);
    getchar();
    for (int i = 0; i < numRules; i++) {
        printf("Enter production %d (format: A->BC): ", i + 1);
        scanf("%c->%s", &lhs, rhs);
        getchar();
        addProduction(lhs, rhs);
    }

    // Input terminals
    printf("Enter terminals (as a single string): ");
    scanf("%s", terminals);
    numTerminals = strlen(terminals);

    // Input non-terminals
    printf("Enter non-terminals (as a single string): ");
    scanf("%s", nonTerminals);
    numNonTerminals = strlen(nonTerminals);

    // Create the canonical collection of LR(0) items
    createCanonicalCollection();

    printf("Canonical Collection of LR(1) Items:\n");
    for (int i = 0; i < numStates; i++) {
        printf("State %d:\n", i);
        for (int j = 0; j < states[i].itemCount; j++) {
            Item item = states[i].items[j];
            printf("%c->%s, %d, %c\n", grammar[item.prodIndex].lhs, grammar[item.prodIndex].rhs, item.dotPosition, item.lookahead);
        }
        printf("\n");
    }
    // Build LALR parsing table
    buildParsingTable();

    // Print the parsing table
    printParsingTable();

    // Input string to parse
    printf("Enter the input string (append $ at the end): ");
    scanf("%s", inputString);

    // Parse the input string
    if (parseInput(inputString)) {
        printf("Input string is accepted.\n");
    } else {
        printf("Input string is rejected.\n");
    }

    return 0;
}
