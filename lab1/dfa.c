//Rohan G
//CS22B1093

// DFA for the following languages:
// 1. Ending with a
// 2. Starting with a
// 3. Contains a
// 4. Length at least 2
// 5. Length at most 2
// 6. Length mod 3

#include <stdio.h>

void ending_with_a(char arr[]){
	int state = 0; // state is the state of the DFA
	int i = 0; // i is the index of the array
	while(arr[i]!='\0'){
		if(state==0 && arr[i]=='a'){
			state = 1;
		}else if(state==1 && arr[i]=='b'){
			state = 0;
		}else if(state==1 && arr[i]=='a'){
			state = 1;
		}else if(state==0 && arr[i]=='b'){
			state = 0;
		}
		i++;
	}
	if(state==1){
		printf("String accepted by DFA\n");
		return;
	}
	printf("String rejected by DFA\n");
	return;
}

void starting_with_a(char arr[]){
	int i = 0; // i is the index of the array
	int state = 0; // state is the state of the DFA

	while(arr[i]!='\0'){
		if((arr[i]=='a') && state == 0){
			state = 1;
		}else if(arr[i]=='b' && state == 1){
			state = 1;
		}else if(arr[i]=='a' && state == 1){
			state = 1;
		}else if(arr[i]=='b' && state == 0){
			state = 2;
		}else if(arr[i]=='a' && state == 2){
			state = 2;
		}else if(arr[i]=='b' && state == 2){
			state = 2;
		}
		if(state==2){
			printf("String not accepted by DFA\n");
			return;
		}
		i++;
	}
	if(state==1){
		printf("String accepted by DFA\n");
		return;
	}
	printf("String not accepted by DFA\n");
	return;
}

void contains_a(char arr[]){
	int i = 0; // i is the index of the array
	int state = 0; // state is the state of the DFA
	
	while(arr[i]!='\0'){
		if(arr[i]=='a' && state == 0){
			state = 1;
		}else if(arr[i]=='b' && state == 1){
			state = 1;
		}else if(arr[i]=='a' && state == 1){
			state = 1;
		}else if(arr[i]=='b' && state == 0){
			state = 0;
		}
		i++;
	}
	if(state==1){
		printf("String accepted by DFA\n");
		return;
	}
	printf("String not accepted by DFA\n");
	return;
}

void length_atleast_2(char arr[]){
	int i = 0; // i is the index of the array
	int state = 0; // state is the state of the DFA
	
	while(arr[i]!='\0'){
		if((arr[i]=='a' || arr[i]=='b') && state == 0){
			state = 1;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 1){
			state = 2;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 2){
			state = 2;
		}
		i++;
	}
	if(state==2){
		printf("String accepted by DFA\n");
		return;
	}
	printf("String not accepted by DFA\n");
	return;
}

void length_atmost_2(char arr[]){
	int i = 0; // i is the index of the array
	int state = 0; // state is the state of the DFA
	
	while(arr[i]!='\0'){
		if((arr[i]=='a' || arr[i]=='b') && state == 0){
			state = 1;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 1){
			state = 2;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 2){
			state = 3;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 3){
			state = 3;
		}
		if(state==3){
			printf("String not accepted by DFA\n");
			return;
		}
		i++;
	}
	if(state==2 || state==1 || state==0){ 
		printf("String accepted by DFA\n");
		return;
	}
	printf("String not accepted by DFA\n");
	return;
}

void length_mod_3(char arr[]){
	int i = 0; // i is the index of the array
	int state = 0; // state is the state of the DFA
	
	while(arr[i]!='\0'){
		if((arr[i]=='a' || arr[i]=='b') && state == 0){
			state = 1;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 1){
			state = 2;
		}else if((arr[i]=='a' || arr[i]=='b') && state == 2){
			state = 0;
		}
		i++;
	}
	if(state==0){
		printf("String accepted by DFA\n");
		return;
	}
	printf("String not accepted by DFA\n");
	return;	
}

void main(){
	char arr[100];
	printf("Enter the string (enter space ' ' for no|empty input): ");
	scanf("%[^\n]s", arr);  // Accept input including space or empty string

	int i = 0;
	while(arr[i]!='\0'){
		if(arr[i]!='a' && arr[i]!='b' && arr[i]!=' '){  // Allow empty input or space
			printf("Invalid string\n");
			return;
		}
		i++;
	}

	// if (arr[0] == '\0' || (arr[0] == ' ' && arr[1] == '\0')) { // Check for empty input
	// 	printf("Empty input or space\n");
	// 	return;
	// }

	printf("Choose one of the following options:\n");
	printf("1. Ending with a\n");
	printf("2. Starting with a\n");
	printf("3. Contains a\n");
	printf("4. Length at least 2\n");
	printf("5. Length at most 2\n");
	printf("6. Length mod 3\n");
	int choice;
	scanf("%d", &choice);

	switch(choice){
		case 1:
			ending_with_a(arr);
			break;
		case 2:
			starting_with_a(arr);
			break;
		case 3:
			contains_a(arr);
			break;
		case 4:
			length_atleast_2(arr);
			break;
		case 5:
			length_atmost_2(arr);
			break;
		case 6:
			length_mod_3(arr);
			break;
		default:
			printf("Invalid choice\n");
	}
}