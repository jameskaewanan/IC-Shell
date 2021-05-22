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
		printf("\n");
		return;
	}
	
	if (!strcmp("!!\n", command)) {
		printf(*previousInput);
		printf("\n");
		return;
	}
	
	if (!strcmp("exit", command)) {
		printf("See you next time! \n");
		printf("\n");
		return exit((u_int8_t)atoi(phrase));
	}
	
	else {	
		char args[1000];
		sprintf(args, phrase);

		int pid = fork();
		
		if (pid < 0) {
			printf("Error, Fork Failed\n");
			exit(EXIT_FAILURE);
		}
		
		if (pid == 0) {
			temp[strcspn(temp, "\n")] = 0;
			temp[strlen(temp)] = '\0';
			args[strcspn(args, "\n")] = 0;
			args[strlen(args)] = '\0';
			
			char *arg[3];
			arg[0] = temp;
			
			if (strlen(args) > 1) { arg[1] = args; }
			else { arg[1] = NULL; }
			
			arg[2] = NULL;
			
			if (execvp(temp, arg) < 0) {
				printf("Bad command, please try again. \n");
				printf("\n");
			}
			else { execvp(temp, arg); }
			exit(1);
		}
		
		if (pid > 0) { waitpid(pid, NULL, 0); }
	}
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

	printf("Starting IC shell...\n");
	printf("\n");

	if (arg > 1) { scriptReader(&argv[1]); }
	

	shell_loop();
	
}

