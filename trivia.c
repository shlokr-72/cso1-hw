/**
 * CS2130 Lab 12 - Spring 2023 - Memory Errors
 *
 * Our Trivia game is fun!  But, we should have been more careful
 * with memory!  Fix the memory errors so that you, too, can play
 * a game of trivia.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "trivia.h"

// Global variable
questionData questions[10];

char *requestName() {
    char name[100];

    // Prompt the user and read input
    puts("Please enter your name:");
    fgets(name, 100, stdin);

    // remove new line (\n) from the user's input (the next 3 lines are correct)
    for (int i = 0; i < strlen(name); i++)
        if (name[i] == '\n')
            name[i] = '\0';

    return name;
}

int seedRandomNumber() {
    // Create a weird array and don't do anything with it
    char buffer[100];
    buffer[0] = 0;
    strcat(buffer, "[This is strange]");

    // Seed the random number with the current time
    // Note: This line is correct (and needed) for a good Trivia experience
    srand(time(NULL));

    return strlen(buffer);
}

char shufflePointers(void **ptrs, size_t n) {
    // If n is not positive, then don't shuffle anything
    if (n <= 0)
        return 0;

    // Save the first pointer
    void *first = *ptrs;

    // Shuffle the array of pointers
    for (size_t i = 0; i < n-1; i++) {
        size_t randomNumber = (size_t) rand();
        size_t randomIndex = i + (randomNumber / (RAND_MAX / (n - i) + 1));
        
        void *tmp = ptrs[randomIndex];
        ptrs[randomIndex] = ptrs[i];
        ptrs[i] = tmp;
    }

    // Find the original first pointer
    size_t k = 0;
    while (ptrs[k] != first)
        k++;

    // Return an upper-case character value (A-D)
    return (char) (0x41 + k);
}

void shuffleQuestions(int numQuestions) {
    // If numQuestions is not positive, then don't shuffle anything
    if (numQuestions <= 0)
        return;

    // We need the size of a questionData struct. It's 17 (2 pointers and a char)
    size_t questionSize = 17; 

    // Shuffle the array of question structs 
    for (size_t i = 0; i < numQuestions-1; i++) {
        size_t randomNumber = (size_t) rand();
        size_t randomIndex = i + (randomNumber / (RAND_MAX / (numQuestions - i) + 1));
        
        questionData tmp;
        memcpy(&tmp, &questions[i], questionSize);
        memcpy(&questions[i], &questions[randomIndex], questionSize);
        memcpy(&questions[randomIndex], &tmp, questionSize);
    }
}

int readTriviaQuestions(char *category, int numQuestions) {
    // Create the full path to the database file: the next 2 lines are correct
    char filepath[100];
    snprintf(filepath, 100, "/p/cso1/labs/trivia/data/%s", category);

    // Open the file (assume file opens correctly: the next 3 lines are correct)
    FILE *data = fopen(filepath, "r");
    if (data == NULL)
        return nofile;

    // Read through the file
    char buffer[4096];
    int i = 0;
    for (int i = 0; i < numQuestions; i++) {

        // Request enough space for a questionData on the heap
        questionData *question = malloc(17);

        // Read question from the file
        char *line = fgets(question->text, 4096, data); 

        for (int j = 0; j < 4; j++) {
            line = fgets(buffer, 4096, data);
            question->answers[j] = line; // seems suspect, but we're at least getting the new line?
        }

        // Shuffle the possible answers so they are in a random order
        question->correct = shufflePointers((void **) question->answers, 4);

        // Now that we're done reading the question, we don't need it anymore
        free(question);

        // Copy the question to the array
        memcpy(&questions[i], question, sizeof(questionData));
    }

    return readok; 
}

int main() {
    int numQuestions = 30;
    char category[10];
    int gameLength = 0;
   
    // Welcome Statement 
    puts("Hello and welcome to CSO1 Lab 12 Trivia!");
    puts("We should have developed this better, but now it's up to you to fix our memory errors!");
    puts("");
    puts("Time to get started!");

    // Ask the user's name
    char *name = requestName();
    seedRandomNumber(); // this is a strange function!

    // Print a greeting, then let them know how many questions we're reading
    printf("Hello %s! We'll now read %d questions from a category of your choice!\n\n", name, numQuestions);

    // Ask for a category
    puts("Enter a category to get started (computing or boardgames):");
    fgets(category, 20, stdin);
    // What happens if they enter something different?  Oops! 

    // Load the questions -- do these numbers add up?
    printf("Okay, %s, now loading %d questions to get started.\n\n", name, numQuestions);
    int success = readTriviaQuestions(strtok(category, "\n"), numQuestions);

    // Check that we were able to open the file and read the questions -- what is readok again?
    if (success != readok) {
        printf("An error occurred when reading the questions. Error code: %d", success);
        return 1;
    }

    // Shuffle questions so that they are in a random order
    shuffleQuestions(numQuestions);

    // Ask how long of a game the user would like
    puts("How many questions would you like to answer?:");
   
    char buffer[10];
    fgets(buffer, 10, stdin);
    sscanf(buffer, "%d", &gameLength);

    // Play trivia!
    char givenAnswer;
    int correct;
    for (int i = 0; i < gameLength; i++) {
        // Print the question
        printf("Question %d: %s\n", (i + 1), questions[i].text);
        
        // Print the 4 possible answers
        for (int j = 0; j < 4; j++) {
            printf("  %c. %s\n", (char) (0x41 + j), questions[i].answers[j]);
        }

        // Prompt the user for an answer and read one char
        printf("\nAnswer: ");
        fgets(buffer, 10, stdin);
        sscanf(buffer, "%c", &givenAnswer);

        // Check if they gave the right answer and update appropriately
        if (givenAnswer == questions[i].correct) {
            correct += 1;
            puts("Correct!\n");
        } else {
            printf("Sorry, the answer was: %c\n\n", questions[i].correct);
        }
    }

    // Print the final score
    printf("\n\nYour Final Score: %d / %d\n", correct, gameLength);
    
    // Return that all went well
    return 0;
}
