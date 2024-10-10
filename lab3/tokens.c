#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char* operators[] = {"+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!"};

int token_count = 0;

void count_token(char* line) {
    int i = 0;
    int token = 0;

    while (line[i] != '\0') {
        if (isspace(line[i])) {
            i++;
            continue;
        }
        if (line[i] == '"') {
            i++;
            while (line[i] != '\0' && line[i] != '"') {
                i++;
            }
            if (line[i] == '"') {
                i++;
                token++;
                token_count++;
                continue;
            }
        }

        int is_operator = 0;
        for (int k = 0; k < sizeof(operators) / sizeof(operators[0]); k++) {
            int len = strlen(operators[k]);
            if (strncmp(&line[i], operators[k], len) == 0) {
                token++;
                token_count++;
                i += len;
                is_operator = 1;
                break;
            }
        }

        if (!is_operator) {
            while (line[i] != '\0' && !isspace(line[i]) && !ispunct(line[i])) {
                i++;
            }
            if (i > 0 && !isspace(line[i - 1]) && !ispunct(line[i - 1])) {
                token++;
                token_count++;
            }
        } else {
            continue;
        }

        if (ispunct(line[i]) && line[i] != '\0' && !is_operator && line[i] != '"') {
            token++;
            token_count++;
            i++;
        }
    }
    //printf("Tokens: %d\n", token);
}

int main() {
    FILE* file = fopen("input_prog1.txt", "r");

    char line[256];

    if (file != NULL) {
        printf("The contents of the file are:\n");
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
            count_token(line);
        }
        printf("\nTotal tokens: %d\n", token_count-3);
        fclose(file);
    } else {
        fprintf(stderr, "Unable to open file!\n");
    }

    return 0;
}
