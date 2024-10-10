//CS22B1093
//ROHAN G

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>  

int words = 0;
int characters = 0;  
int lines = 0;
int vowels = 0;
int special_characters = 0;
int consonants = 0;
int letters = 0;
int digits = 0;

void count(char *line) {
    int in_word = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        char ch = line[i];
        characters++;
        if (isalpha(ch)) {
            letters++;
            ch = tolower(ch);  
            if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
                vowels++;
            } else {
                consonants++;
            }
        }

        if (isdigit(ch)) {
            digits++;
        }

        if (!isalnum(ch) && !isspace(ch)) {
            special_characters++;
        }

        if (!isspace(ch) && !in_word) {
            in_word = 1;
            words++;
        } else if (isspace(ch)) {
            in_word = 0;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <c_program_file.c>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        lines++;
        count(line);
    }
    printf("Vowels: %d\n", vowels);
    printf("Consonants: %d\n", consonants);
    printf("Digits: %d\n", digits);
    // printf("Words: %d\n", words);
    //printf("Characters: %d\n", characters);
    //printf("Letters: %d\n",letters);
    printf("Sum : %d",characters);
    //printf("Lines: %d\n", lines);
    
    //printf("Special Characters: %d\n", special_characters); 

    fclose(file);
    return 0;
}
