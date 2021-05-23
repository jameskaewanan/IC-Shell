#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

pid_t child;
int exitCode = 0;

void signalHandler(int sig) {

	if (sig == 2) {
		printf(" Caught signal %d, killing foreground process...\n", sig);
		kill(child, SIGINT);
	}
	
	if (sig == 20) {
		printf(" Caught signal %d, suspending foreground process...\n", sig);
		kill(child, SIGTSTP);
	}
	
}

void commandHandler(char**currentInput, char **previousInput) {
	char temp[1000];
	strcpy(temp, *currentInput);
	
	char *command = strtok(temp, " ");
	char *phrase = strtok(NULL, "");
	
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
			printf(phrase, "\n");
			printf("\n");
			exitCode = 0;
			return;
		}
	}
	
	if (!strcmp("!!\n", command)) {
		printf(*previousInput);
		printf("\n");
		exitCode = 0;
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

void nullSignal(int sig) { 
	return;
}

int main (int arg, char *argv[]) {

	printf("Initialising IC Shell. . . \n \n");
	
	FILE *file;
	file = fopen("welcome.txt", "r");
	char read_string[1000];
	
	while(fgets(read_string,sizeof(read_string), file) != NULL)
 		printf("%s", read_string);
 		
 	fclose(file);

	if (arg > 1) { scriptReader(&argv[1]); }
	
	struct sigaction sigtstp_default;
	struct sigaction sigint_default;
	
	sigtstp_default.sa_handler = nullSignal;
	sigint_default.sa_handler = nullSignal;
	
	sigaction(SIGTSTP, &sigtstp_default, NULL);
	sigaction(SIGINT, &sigint_default, NULL);
	
	shell_loop();
	
}

