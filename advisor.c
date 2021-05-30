#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLUE 		"\033[1;34m"
#define D_COLOUR 	"\033[0m"

 
void advisor_loop() {

    char *question = NULL;
    size_t len = 0;
    ssize_t read;
    
    const char* answers[20] = {
        "It is certain.",
        "It is decidedly so.",
        "Without a doubt krub.",
        "Yes, definitely.",
        "You may rely on it.",
        "As I see it, yes.",
        "Most likely.",
        "I think it's time for you to change majors na krub.",
        "Consider Withdrawing a difficult course.",
        "Yes.",
        "I'll get back to you.",
        "Ask again later.",
        "Uhh... Is the CS workload too much for you?",
        "I am not sure.",
        "Concentrate and ask again.",
        "Don't bet on it.",
        "Sorry na krub, but my reply is a no.",
        "My sources say no.",
        "Your outlook doesn't look very good na krub.",
        "Very doubtful."
    };
    
    srand(time(NULL));
    
    
    printf(BLUE);
    
    printf("Hello, I am your friendly advisor, Aj. [INSERT ADVISOR NAME].\n\n");
    
    printf("Please only ask yes or no questions, as I believe more pressing matters hould be consulted in person.\n\n");
    
    printf("Please enter your question or leave empty to quit this session.\n");
    
    printf(D_COLOUR);
    
    while (1) {
    	printf(BLUE);
    	printf("\n>> ");
	printf(D_COLOUR);
        read = getline(&question, &len, stdin);
        if (read < 2) break;
        printf("\n%s\n", answers[rand() % 20]);
    }
    if (question) free(question);
    return 0;
}

