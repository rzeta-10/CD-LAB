//CS22B1093 ROHAN G

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100

int isTerm(char ter);
int isNTerm(char nT);
int LeftCheck(char nT, char search, int escape);
int RightCheck(char nT, char search);
void LeftToRight(char nT);
void PrintProd(char nT);

char terms[MAX_SIZE], nonterms[MAX_SIZE];
char prodDict[MAX_SIZE][MAX_SIZE][MAX_SIZE];
int leftCheckList[MAX_SIZE][MAX_SIZE];
int leftCheckDict[MAX_SIZE], rightCheckDict[MAX_SIZE];
int nontermsCount = 0, termsCount = 0, prodCount[MAX_SIZE];

int isTerm(char ter) {
    if (ter == 'e') {
        return 1;
    }
    for (int i = 0; i < termsCount; i++) {
        if (ter == terms[i]) {
            return 1;
        }
    }
    return 0;
}

int isNTerm(char nT) {
    for (int i = 0; i < nontermsCount; i++) {
        if (nT == nonterms[i]) {
            return 1;
        }
    }
    return 0;
}

int LeftCheck(char nT, char search, int escape) {
    for (int i = 0; i < prodCount[nT]; i++) {
        if (search == prodDict[nT][i][0]) {
            leftCheckList[search][i] = nT;
            return 1;
        } else if (isNTerm(prodDict[nT][i][0])) {
            return LeftCheck(prodDict[nT][i][0], search, escape + 1);
        }
    }
    return 0;
}

int RightCheck(char nT, char search) {
    for (int i = 0; i < prodCount[nT]; i++) {
        int len = strlen(prodDict[nT][i]);
        if (search == prodDict[nT][i][len - 1]) {
            return 1;
        }
    }
    return 0;
}

void LeftToRight(char nT) {
    char bet[MAX_SIZE][MAX_SIZE], alpha[MAX_SIZE][MAX_SIZE];
    int betCount = 0, alphaCount = 0;

    for (int i = 0; i < prodCount[nT]; i++) {
        if (nT == prodDict[nT][i][0]) {
            strcpy(alpha[alphaCount++], prodDict[nT][i] + 1);
        } else {
            strcpy(bet[betCount++], prodDict[nT][i]);
        }
    }

    printf("%c -> ", nT);
    for (int i = 0; i < betCount; i++) {
        printf("%s%c' ", bet[i], nT);
        if (i != betCount - 1) printf("/");
    }
    printf("\n");

    printf("%c' -> ", nT);
    for (int i = 0; i < alphaCount; i++) {
        printf("%s%c' ", alpha[i], nT);
        if (i != alphaCount - 1) printf("/");
    }
    printf("/e\n");
}

void PrintProd(char nT) {
    printf("%c -> ", nT);
    for (int i = 0; i < prodCount[nT]; i++) {
        printf("%s", prodDict[nT][i]);
        if (i != prodCount[nT] - 1) printf("/");
    }
    printf("\n");
}

int main() {
    int n;
    char productions[MAX_SIZE][MAX_SIZE];

    printf("Enter the number of Productions: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Enter production %d: ", i + 1);
        scanf("%s", productions[i]);
    }

    for (int i = 0; i < n; i++) {
        nonterms[nontermsCount++] = productions[i][0];
    }

    for (int i = 0; i < n; i++) {
        for (int j = 3; j < strlen(productions[i]); j++) {
            if (!isNTerm(productions[i][j]) && productions[i][j] != 'e' && productions[i][j] != '/') {
                terms[termsCount++] = productions[i][j];
            }
        }
    }

    for (int i = 0; i < nontermsCount; i++) {
        prodCount[nonterms[i]] = 0;
    }

    for (int i = 0; i < n; i++) {
        char nonterm = productions[i][0];
        char *expansion = strtok(&productions[i][3], "/");
        while (expansion != NULL) {
            strcpy(prodDict[nonterm][prodCount[nonterm]++], expansion);
            expansion = strtok(NULL, "/");
        }
    }

    for (int i = 0; i < nontermsCount; i++) {
        leftCheckDict[nonterms[i]] = LeftCheck(nonterms[i], nonterms[i], 0);
        rightCheckDict[nonterms[i]] = RightCheck(nonterms[i], nonterms[i]);
    }

    printf("\n");
    for (int i = 0; i < nontermsCount; i++) {
        if (leftCheckDict[nonterms[i]]) {
            printf("%c contains Left Recursive Grammar\n", nonterms[i]);
        }
        if (rightCheckDict[nonterms[i]]) {
            printf("%c contains Right Recursive Grammar\n", nonterms[i]);
        }
    }

    printf("Left Eliminated Grammar:\n");
    for (int i = 0; i < nontermsCount; i++) {
        if (leftCheckDict[nonterms[i]]) {
            LeftToRight(nonterms[i]);
        } else {
            PrintProd(nonterms[i]);
        }
    }

    return 0;
}
