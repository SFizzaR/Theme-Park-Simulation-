#ifndef FOODCOURT_H
#define FOODCOURT_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <semaphore.h>

#define MENU_SIZE 20
#define MAX_ORDER_SIZE 100

typedef struct {
    char name[100];
    double price;
} MenuItem;

typedef struct {
    int dinerId;
    char name[20];
    int orderItems[MAX_ORDER_SIZE];
    int orderQuantities[MAX_ORDER_SIZE];
    int numItems;
    float foodRating;
    float serviceRating;
    char feedbackConsent[10];
} Diner;

extern double FoodCourt_revenue; 
extern MenuItem menu[MENU_SIZE];
extern sem_t menuSemaphore;
extern sem_t orderSemaphore;
extern sem_t feedbackSemaphore;

void *dinerThread(void *arg);
void takeOrders(Diner *diners, int numDiners);
void deliverOrders(Diner *diners, int numDiners);
void collectFeedback(Diner *diners, int numDiners);

#endif /* FOODCOURT_H */

