// converting dfa code with c //

#include <stdio.h>

void ending_with_a(char arr[]){
	int i = 0;
	while(arr[i]!='\0'){
		if(arr[i]=='a' && arr[i+1]=='\0'){
			printf("String accepted by DFA");
			return;
		}
		i++;
	}
	printf("String not accepted by DFA");
	return;
}

void starting_with_a(char arr[]){
	int i = 0;
	if(arr[i]=='a'){
		printf("String accepted by DFA");
		return;
	}
	printf("String not accepted by DFA");
}

void atleast_length_2(char arr[]){
	int i = 0;
	while(arr[i]!='\0'){
		i++;
	}
	if(i>=2){
		printf("String accepted by DFA");
		return;
	}
	printf("String not accepted by DFA");
	return;
}

void atmost_length_2(char arr[]){
	int i = 0;
	while(arr[i]!='\0'){
		i++;
	}
	if(i<=2){
		printf("String accepted by DFA");
		return;
	}
	printf("String not accepted by DFA");
	return;

}

void main(){
	char arr[20];
	scanf("%s", arr);
	int length = sizeof(arr)/sizeof(arr[0]);
	//ending_with_a(arr);
	starting_with_a(arr);
	//atleast_length_2(arr);
	//atmost_length_2(arr);
	//printf("The size of array is: %d\n", length);
}
