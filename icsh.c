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
		return exit((u_int8_t)atoi(phrase));
	}
	
	else { printf("Bad command \n"); }

	return;
}


void shell_loop() {

	char *currentInput = malloc(sizeof(char) * 1000);
	char *previousInput = malloc(sizeof(char) * 1000);

	while(1) {
		printf("icsh $ >> ");
		
		fgets(currentInput, 1000, stdin);
		if (!strcmp(currentInput, "\n")) { continue; }
		
		else {
		commandHandler(&currentInput, &previousInput);
		strcpy(previousInput, currentInput);
		}

	}

}


void scriptReader(char **directory) {

	FILE *file;
	file = fopen(*directory, "r");
	char *currentLine = malloc(sizeof(char) * 1000);
	char *previousLine = malloc(sizeof(char) * 1000);
	
	if (file == NULL) { exit(EXIT_FAILURE); }
	
	while(fgets(currentLine, 1000, file) != NULL) {
		//printf(currentLine);
		commandHandler(&currentLine, &previousLine);
		strcpy(previousLine, currentLine);
	}
	return;
}

int main (int arg, char *argv[]) {

	if (arg > 1) { scriptReader(&argv[1]); }
	
	printf("Starting IC shell\n");
	shell_loop();
	
}

