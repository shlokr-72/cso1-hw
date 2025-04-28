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

void requestName(char *name) {
    puts("Please enter your name:");
    fgets(name, 100, stdin);

    for (int i = 0; i < strlen(name); i++)
        if (name[i] == '\n')
            name[i] = '\0';
}

int seedRandomNumber() {
    char buffer[100];
    buffer[0] = 0;
    strcat(buffer, "[This is strange]");

    srand(time(NULL));

    return strlen(buffer);
}

char shufflePointers(void **ptrs, size_t n) {
    if (n <= 0)
        return 0;

    void *first = *ptrs;

    for (size_t i = 0; i < n-1; i++) {
        size_t randomNumber = (size_t) rand();
        size_t randomIndex = i + (randomNumber / (RAND_MAX / (n - i) + 1));
        
        void *tmp = ptrs[randomIndex];
        ptrs[randomIndex] = ptrs[i];
        ptrs[i] = tmp;
    }

    size_t k = 0;
    while (ptrs[k] != first)
        k++;

    return (char) (0x41 + k);
}

void shuffleQuestions(int numQuestions) {
    if (numQuestions <= 0)
        return;

    size_t questionSize = sizeof(questionData);

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
    char filepath[100];
    snprintf(filepath, 100, "/p/cso1/labs/trivia/data/%s", category);

    FILE *data = fopen(filepath, "r");
    if (data == NULL)
        return nofile;

    char buffer[4096];
    int actualQuestions = 0;
    for (int i = 0; i < numQuestions; i++) {
        if (fgets(questions[i].text, 4096, data) == NULL)
            break;

        for (int k = 0; k < strlen(questions[i].text); k++)
            if (questions[i].text[k] == '\n')
                questions[i].text[k] = '\0';

        int readAllAnswers = 1;
        for (int j = 0; j < 4; j++) {
            if (fgets(buffer, 4096, data) == NULL) {
                readAllAnswers = 0;
                break;
            }
            questions[i].answers[j] = strdup(buffer);
            for (int k = 0; k < strlen(questions[i].answers[j]); k++)
                if (questions[i].answers[j][k] == '\0')
                    break;
                else if (questions[i].answers[j][k] == '\n')
                    questions[i].answers[j][k] = '\0';
        }

        if (!readAllAnswers)
            break;

        questions[i].correct = shufflePointers((void **) questions[i].answers, 4);
        actualQuestions++;
    }

    fclose(data);
    return actualQuestions;
}

int main() {
    int numQuestions = 30;
    char category[20];
    int gameLength = 0;
   
    puts("Hello and welcome to CSO1 Lab 12 Trivia!");
    puts("We should have developed this better, but now it's up to you to fix our memory errors!");
    puts("");
    puts("Time to get started!");

    char name[100];
    requestName(name);
    seedRandomNumber();

    printf("Hello %s! We'll now read %d questions from a category of your choice!\n\n", name, numQuestions);

    puts("Enter a category to get started (computing or boardgames):");
    fgets(category, 20, stdin);

    printf("Okay, %s, now loading %d questions to get started.\n\n", name, numQuestions);
    numQuestions = readTriviaQuestions(strtok(category, "\n"), numQuestions);

    if (numQuestions <= 0) {
        printf("An error occurred when reading the questions.\n");
        return 1;
    }

    shuffleQuestions(numQuestions);

    puts("How many questions would you like to answer?:");
   
    char buffer[10];
    fgets(buffer, 10, stdin);
    sscanf(buffer, "%d", &gameLength);

    char givenAnswer;
    int correct = 0;
    for (int i = 0; i < gameLength; i++) {
        printf("Question %d: %s\n", (i + 1), questions[i].text);
        
        for (int j = 0; j < 4; j++) {
            printf("  %c. %s\n", (char) (0x41 + j), questions[i].answers[j]);
        }

        printf("\nAnswer: ");
        fgets(buffer, 10, stdin);
        sscanf(buffer, "%c", &givenAnswer);

        if (givenAnswer == questions[i].correct) {
            correct += 1;
            puts("Correct!\n");
        } else {
            printf("Sorry, the answer was: %c\n\n", questions[i].correct);
        }
    }

    printf("\n\nYour Final Score: %d / %d\n", correct, gameLength);

    for (int i = 0; i < numQuestions; i++) {
        for (int j = 0; j < 4; j++) {
            free(questions[i].answers[j]);
        }
    }

    return 0;
}
