#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER_SIZE 	9999

#define BLUE 		"\033[1;34m"
#define D_COLOUR 	"\033[0m"

pid_t child;
int exitCode = 0;


void signalHandler(int sig) {

	if (sig == 2) {
		printf("\n\nCaught signal %d, killing foreground process...\n\n", sig);
		kill(child, SIGKILL);
	}
	
	if (sig == 20) {
		printf("\n\nCaught signal %d, suspending foreground process...\n\n", sig);
		kill(child, SIGTSTP);
	}
	
}

void redirectionHandler(char **currentInput, char **previousInput) {

	char temp[BUFFER_SIZE];
	strcpy(temp, *currentInput);

	if (strstr(*currentInput, " > ") != NULL) { 
	
		char *fileLocation = strchr(temp, '>');
		fileLocation = strchr(fileLocation, ' ');
		fileLocation = strtok(fileLocation, " ");
		fileLocation[strcspn(fileLocation, "\n")] = 0;
		fileLocation[strlen(fileLocation)] = '\0';
		
		char *command = strtok(temp, ">");
		command[strcspn(command, "\n")] = 0;
		command[strlen(command)-1] = '\0';
		
		int file = open(fileLocation, O_WRONLY | O_CREAT, 0666);
		
		int out = dup(1);
		
		dup2(file,1);
		
		exitCode = commandHandler(&command, &previousInput);
		
		dup2(out, 1);
		close(file);
		close(out);
		
		printf("\nFile created and contents saved successfully. \n\n");
		
		return;
		
		if (file < 0) {
			printf("\nUnable to create a file. \n\n");
			return;
		}
	}
	
	if (strstr(*currentInput, " < ") != NULL) {
	
		char *fileLocation = strchr(temp, '<');
		fileLocation = strchr(fileLocation, ' ');
		fileLocation = strtok(fileLocation, " ");
		fileLocation[strcspn(fileLocation, "\n")] = 0;
		fileLocation[strlen(fileLocation)] = '\0';
		
		FILE *file;
		file = fopen(fileLocation, "r");
		char *currentLine = malloc(sizeof(char) * BUFFER_SIZE);
		char *previousLine = malloc(sizeof(char) * BUFFER_SIZE);
	
		if (file == NULL) { exit(EXIT_FAILURE); }
	
		while(fgets(currentLine, BUFFER_SIZE, file) != NULL) {
			commandHandler(&currentLine, &previousLine);
			strcpy(&previousLine, &currentLine);
		}
		
		return;
		
		if (file < 0) {
			printf("\nUnable to locate the file. \n\n");
			return;
		}
	}
}

void commandHandler(char **currentInput, char **previousInput) {
	char temp[BUFFER_SIZE];
	strcpy(temp, *currentInput);
	
	char *command = strtok(temp, " ");
	char *phrase = strtok(NULL, "");

	if (strstr(*currentInput, " > ") != NULL || strstr(*currentInput, " < ") != NULL) { 
		redirectionHandler(currentInput, previousInput);
		return;
	}
	
	if (!strcmp("echo\n", temp) || !strcmp("echo ", temp)) {
		printf("\n");
		exitCode = 0;
		return;
	}
	
	if (!strcmp("echo", command)) {
		if (!strcmp("$?\n", phrase)) {
			printf("%i\n", exitCode);
			printf("\n");
			return;
		}
		else {
			printf("\n%s\n", phrase);
			exitCode = 0;
			return;
		}
	}
	
	if (!strcmp("!!\n", command)) {
		printf(*previousInput);
		printf("\n");
		exitCode = 0;
		return commandHandler(previousInput, currentInput);
	}
	
	if (!strcmp("exit", command)) {
		printf("\nSee you next time! \n\n");
		return exit((u_int8_t)atoi(phrase));
	}
	
	else {	
		char args[BUFFER_SIZE];
		sprintf(args, phrase);

		int pid = fork();
		
		if (pid < 0) {
			printf("\nError, Fork Failed\n\n");
			exit(EXIT_FAILURE);
		}
		
		if (pid == 0) {
			temp[strcspn(temp, "\n")] = 0;
			temp[strlen(temp)] = '\0';
			args[strcspn(args, "\n")] = 0;
			args[strlen(args)] = '\0';
			
			char *arg[3];
			arg[0] = temp;
			
			if (strlen(args) > 0) { arg[1] = args; }
			else { arg[1] = NULL; }
			
			arg[2] = NULL;
			
			if (execvp(temp, arg) < 0) {
				printf("\nCould not find command, please try again. \n\n");
			}
			else { execvp(temp, arg); }
			exit(1);
		}
		
		if (pid > 0) { 
		
			struct sigaction sigtstp;
			struct sigaction sigint;
		
			sigtstp.sa_handler = signalHandler;
			sigint.sa_handler = signalHandler;
			
			child = pid;
			int status;
		
			while (waitpid(pid, &status, WNOHANG) == 0) {
				sigaction(SIGINT, &sigint, NULL);
				sigaction(SIGTSTP, &sigtstp, NULL);	
			}
			return;	
		}
		return;
	}
}

void shell_loop() {

	char *currentInput = malloc(sizeof(char) * BUFFER_SIZE);
	char *previousInput = malloc(sizeof(char) * BUFFER_SIZE);

	while(1) {
	
		printf(BLUE);
		printf("icsh $ >> ");
		fgets(currentInput, BUFFER_SIZE, stdin);
		printf(D_COLOUR);
		
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
	char *currentLine = malloc(sizeof(char) * BUFFER_SIZE);
	char *previousLine = malloc(sizeof(char) * BUFFER_SIZE);
	
	if (file == NULL) { exit(EXIT_FAILURE); }
	
	while(fgets(currentLine, BUFFER_SIZE, file) != NULL) {
		commandHandler(&currentLine, &previousLine);
		strcpy(previousLine, currentLine);
	}
	return;
}

int main (int arg, char *argv[]) {
	
	system("clear");

	printf("Initialising IC Shell. . . \n \n");
	
	FILE *file;
	file = fopen("welcome.txt", "r");
	char read_string[BUFFER_SIZE];
	
	while(fgets(read_string,sizeof(read_string), file) != NULL)
 		printf("%s", read_string);
 		
 	fclose(file);

	if (arg > 1) { scriptReader(&argv[1]); }
	
	struct sigaction sigtstp_default, sigint_default;
	
	sigtstp_default.sa_handler = SIG_IGN;
	sigint_default.sa_handler = SIG_IGN;
	
	sigaction(SIGTSTP, &sigtstp_default, NULL);
	sigaction(SIGINT, &sigint_default, NULL);
	
	shell_loop();
	
}

