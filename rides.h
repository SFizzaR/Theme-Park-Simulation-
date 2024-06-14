#ifndef RIDES_H
#define RIDES_H

#include <pthread.h>
#include <semaphore.h>

#define MAX_CUSTOMERS 1000
#define MAX_EXTRA_WAITING_CUSTOMERS 1000
#define NUM_RIDES 5

typedef struct {
    pthread_t tid;
    char name[100];
    int ride;
    int isVIP;
} Customer;

typedef struct CustomerNode {
    char name[100];
    int ride;
    int isVIP;
    struct CustomerNode* next;
} CustomerNode;

typedef struct Ride {
    int capacity;
    CustomerNode* queue;
    float price;    // Add price attribute
    float revenue;  // Add revenue attribute
    sem_t sem_capacity;
} Ride;
typedef struct {
    Customer extra_waiting_customers[MAX_EXTRA_WAITING_CUSTOMERS];
    int num_extra_waiting_customers;
} ExtraWaitingLine;

// Function prototypes
extern Ride rides[NUM_RIDES];
extern pthread_mutex_t ride_mutexes[NUM_RIDES];
extern pthread_mutex_t extra_waiting_line_mutex;
extern ExtraWaitingLine extra_waiting_line;
extern float ride_revenue;

void* customer_thread(void* arg);
void generate_tickets(int num_customers, Customer ride_customers[MAX_CUSTOMERS]);
void display_customer_ids(int num_customers, Customer ride_customers[MAX_CUSTOMERS]);
void display_and_serve_customers(int num_customers, Customer ride_customers[MAX_CUSTOMERS]);
CustomerNode* enqueue(Ride* ride, Customer* customer);
CustomerNode* enqueue_waiting_line(void* ride_or_waiting_line, Customer* customer);
CustomerNode* dequeue(Ride* ride);
void process_extra_waiting_line(Customer ride_customers[MAX_CUSTOMERS]);
void serve_customers_in_waiting_lines();
void display_revenue();

#endif /* RIDES_H */
