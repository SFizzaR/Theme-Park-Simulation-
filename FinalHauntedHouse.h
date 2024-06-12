#ifndef HAUNTEDHOUSE_H
#define HAUNTEDHOUSE_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_VISITORS 1000
#define NUM_EVENTS 5
#define NUM_DOORS 5
#define NUM_QUESTIONS 5
#define MAX_NAME_LENGTH 50
#define TICKET_PRICE 10
#define MAX_FEEDBACK_LENGTH 500

typedef struct {
    char event[1000];
} SpookyEvent;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int age;
    char doorChoice;
    char experienceComments[MAX_FEEDBACK_LENGTH];
    int ticketPrice;
    float rating;
    bool allAnswersCorrect;
} Visitor;

extern pthread_mutex_t mutex; // Mutex for ticketPrice access
extern pthread_mutex_t feedbackMutex; // Mutex for feedback data access
extern char globalFeedbackArray[MAX_VISITORS][MAX_FEEDBACK_LENGTH];
extern float globalRatingsArray[MAX_VISITORS];

extern SpookyEvent spookyEvents[NUM_EVENTS];
extern char doorOptions[NUM_DOORS][12];
extern char *questions[NUM_QUESTIONS];
extern char *answers[NUM_QUESTIONS];

void *visitorThread(void *visitor);
void inviteVisitors(Visitor *visitors, int numVisitors);
void hauntedexperience(Visitor *visitor);
void askforExperienceAgain(Visitor *visitors, int numVisitors);
void askforFeedback(Visitor *visitors, int numVisitors);
void analyzeFeedback();
void giveSurpriseGift(Visitor *visitors, int numVisitors);

#endif /* HAUNTEDHOUSE_H */

