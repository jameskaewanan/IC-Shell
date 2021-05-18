#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void commandHandler(char**currentInput, char **previousInput) {
	char temp[1000];
	strcpy(temp, *currentInput);
	
	char *command = strtok(temp, " ");
	char *phrase = strtok(NULL, "");
	
	if (!strcmp("echo", command)) {
		printf(phrase, "\n");
		return;
	}
	
	if (!strcmp("!!\n", command)) {
		printf(*previousInput);
		return;
	}
	
	if (!strcmp("exit", command)) {
		printf("See you next time! \n");
		exit(0);
		return;
	}
	
	else {printf("Bad command \n");}

	return;
}


void shell_loop() {

	char *currentInput = malloc(sizeof(char)* 1000);
	char *previousInput = malloc(sizeof(char)* 1000);

	while(1) {
		printf("icsh $ >> ");
		
		fgets(currentInput, 1000, stdin);
		if (!strcmp(currentInput, "\n")) {continue;}
		
		else {
		commandHandler(&currentInput, &previousInput);
		strcpy(previousInput, currentInput);
		}

	}

}


int main (int arg, char *argv[]) {
	printf("Starting IC shell\n");
	shell_loop();

}

