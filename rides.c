#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <ctype.h>
#include "rides.h"

Ride rides[NUM_RIDES];
pthread_mutex_t extra_waiting_line_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ride_mutexes[NUM_RIDES];
ExtraWaitingLine extra_waiting_line;
float ride_revenue = 0.0;

void generate_tickets(int num_customers, Customer ride_customers[MAX_CUSTOMERS]) {
    char ride_names[NUM_RIDES][50] = {"Roller Coaster", "Ferris Wheel", "Bumper Cars", "Gravity Drop", "Flying Carpet"};
    float ride_prices[NUM_RIDES] = {10.0, 7.5, 5.0, 12.0, 8.0}; // Prices for each ride

    for (int i = 0; i < NUM_RIDES; i++) {
        rides[i].price = ride_prices[i];
        
        rides[i].revenue = 0.0;
    }
    
    for (int i = 0; i < num_customers; i++) {
        int validName = 0; // Flag to track if a valid name has been entered


        do {
            printf("\nEnter name for Customer %d: ", i + 1);
            if (scanf("%99s", ride_customers[i].name) != 1) {
                printf("Invalid input. Please enter alphabets only.\n");
                while (getchar() != '\n'); // Clear input buffer
            } else {
                // Check if the entered name contains only alphabets
                validName = 1; // Assume the name is valid initially
                for (int j = 0; ride_customers[i].name[j] != '\0'; j++) {
                    if (!isalpha(ride_customers[i].name[j])) {
                        validName = 0; // Name contains non-alphabetic characters
                        break;
                    }
                }
                if (!validName) {
                    printf("Invalid input. Please enter alphabets only.\n");
                }
            }
        } while (!validName); // Continue until a valid name is entered


        int anyRideAvailable = 0; // Flag to track if any ride is available


        printf("Available rides:\n");
        for (int j = 0; j < NUM_RIDES; j++) {
            if (rides[j].capacity > 0) {
                printf("%d. %s: %f \n", j + 1, ride_names[j], ride_prices[j]);
             
                anyRideAvailable = 1; // Set the flag to indicate that at least one ride is available
            }
        }


        // If no ride is available, print a message and move to the next customer
      if (!anyRideAvailable) {
      printf("None\n");
     
      printf("\nAll rides are full. %s, you have to wait in the Extra Waiting Line.\n", ride_customers[i].name);
     
      ride_customers[i].ride = -1; // Indicates joining the EXTRA waiting line
      ride_customers[i].isVIP = 0; // VIP status doesn't matter for EXTRA waiting line
      continue; // Move to the next customer
     }


        int chosen_ride;
        do {
            printf("Enter ride preference for Customer %d (1, 2, ..., 5): ", i + 1);
            scanf("%d", &chosen_ride);
         
            if (chosen_ride < 1 || chosen_ride > NUM_RIDES) {
                printf("Invalid ride preference. Please choose again.\n");
                continue;
            }


            if (rides[chosen_ride - 1].capacity <= 0) {
                char choice;
                printf("Maximum capacity for Ride %d reached.\n", chosen_ride);
                printf("Do you want to join the EXTRA waiting line? (Y/N): ");
                scanf(" %c", &choice);
               
                if (choice == 'Y' || choice == 'y') {
                    printf("\n Customer %s is going to wait in the EXTRA waiting line.\n", ride_customers[i].name);
                    ride_customers[i].ride = -1; // Indicates joining the EXTRA waiting line
                    ride_customers[i].isVIP = 0; // VIP status doesn't matter for EXTRA waiting line
                    break;
                }
                else {
                    printf("Please choose another ride.\n");
                    continue;
                }
            }
            else {
                printf("Is Customer %d VIP? (1 for VIP, 0 for non-VIP): ", i + 1);
                do {
                    scanf("%d", &ride_customers[i].isVIP);
                    if (ride_customers[i].isVIP != 0 && ride_customers[i].isVIP != 1) {
                        printf("Invalid input. Please enter either 0 for non-VIP or 1 for VIP.\n");
                        printf("Is Customer %d VIP? (1 for VIP, 0 for non-VIP): ", i + 1);
                    }
                } while (ride_customers[i].isVIP != 0 && ride_customers[i].isVIP != 1);


                printf("\nCustomer %s is going to ride %d.\n", ride_customers[i].name, chosen_ride);
                ride_customers[i].ride = chosen_ride;
                rides[chosen_ride - 1].capacity--; // Decrement ride capacity
                break;
            }
        } while (1);
    }
    usleep(2000000); // 2 second delay
}


void display_customer_ids(int num_customers, Customer ride_customers[MAX_CUSTOMERS]) {
    printf("\nCustomer IDs for each ride category:\n");
    for (int ride = 0; ride < NUM_RIDES; ride++) {
        printf("Ride %d Customers:\n", ride + 1);
        for (int i = 0; i < num_customers; i++) {
           
            if (ride_customers[i].ride == ride + 1) {
                printf("Customer Name: %s | VIP Status: %s\n", ride_customers[i].name,
                       ride_customers[i].isVIP ? "VIP" : "Regular");
            }
        }
        printf("\n");
        usleep(1000000); // 1 second delay
    }
}


void* customer_thread(void* arg) {
    Customer* customer = (Customer*)arg;
    int chosen_ride = customer->ride;
    Ride* current_ride;


    if (chosen_ride == -1) {
        current_ride = &rides[chosen_ride - 1];
        if (sem_trywait(&current_ride->sem_capacity) != 0) {
            pthread_mutex_lock(&extra_waiting_line_mutex);
           
            if (extra_waiting_line.num_extra_waiting_customers < MAX_EXTRA_WAITING_CUSTOMERS) {
               
                // Queue the customer into the extra waiting line
                extra_waiting_line.extra_waiting_customers[extra_waiting_line.num_extra_waiting_customers] = *customer;
                extra_waiting_line.num_extra_waiting_customers++;
                printf("Customer %s queued to extra waiting line\n", customer->name);
            }
          else {
                printf("Extra waiting line is full. %s, you can roam around the theme park and explore other facilities.\n", customer->name);
            }
            pthread_mutex_unlock(&extra_waiting_line_mutex);
            pthread_exit(NULL);
        }
    }
     else
     {
    // If the customer is not queued into the extra waiting line, enqueue them into the ride's waiting line
    current_ride = &rides[chosen_ride - 1];
    pthread_mutex_lock(&ride_mutexes[chosen_ride - 1]); // Lock the mutex for the chosen ride
   
    enqueue(current_ride, customer);
   
     // Simulate customer logic
    printf("Customer %s entered Ride %d\n", customer->name, customer->ride);
    printf("Customer %s queued for Ride %d\n", customer->name, customer->ride);
     }
    pthread_mutex_unlock(&ride_mutexes[chosen_ride - 1]); // Unlock the mutex for the chosen ride
    // Release semaphore if applicable
    if (chosen_ride != -1 && sem_post(&current_ride->sem_capacity) != 0) {
        perror("sem_post");
    }
   
    pthread_exit(NULL);
}
/*The 'customer_thread' function introduces mutexes (ride_mutexes) to synchronize access to the waiting lines of each ride.
  It's imp because:
 1.Concurrency Control: With multiple customer threads trying to enqueue themselves into ride waiting lines simultaneously, there's a risk of race conditions where threads interfere with each other's operations. Mutexes prevent this by allowing only one thread at a time to access the waiting line, ensuring data consistency.


2. Critical Section Protection: The critical section, which includes the enqueue operation, is protected by locking the mutex. This ensures that once a thread enters the critical section, no other thread can modify the waiting line until one customer thread is enqueued.This prevents conflicts and maintains the order in which customers are enqueued into the waiting lines.


3. Thread Safety: Mutexes guarantee thread safety by providing a mechanism for mutual exclusion, ensuring that concurrent access to shared resources (in this case, the waiting lines) does not lead to data corruption or inconsistencies.
 */


void display_and_serve_customers(int num_customers, Customer ride_customers[MAX_CUSTOMERS]) {
    // Display waiting lines before serving customers
    printf("\nWaiting lines for each ride in the order of customers arrival after they are enqueued:\n");
    for (int ride = 0; ride < NUM_RIDES; ride++) {
        printf("Ride %d Waiting Line\n", ride + 1);
        Ride* current_ride = &rides[ride];
        CustomerNode* current_customer = current_ride->queue;

        while (current_customer != NULL) {
            printf("%s (VIP: %s)\n", current_customer->name, current_customer->isVIP ? "Yes" : "No");
            current_customer = current_customer->next;
        }
        printf("\n");
    }

    // Serve customers according to the order of priority and FCFS
    printf("Serving customers:\n");
    for (int ride = 0; ride < NUM_RIDES; ride++) {
        Ride* current_ride = &rides[ride];
        int customers_served = 0; // Flag to track if customers were served for this ride
        while (current_ride->queue != NULL) {
            CustomerNode* current_customer = dequeue(current_ride);
            printf("Customer %s served for Ride %d\n", current_customer->name, current_customer->ride);
            
            // Update revenue
            current_ride->revenue += current_ride->price;
            ride_revenue += current_ride->price;

            free(current_customer);

            // Introduce a delay after serving each customer
            usleep(1000000); // 1 second delay
            customers_served = 1; // Set the flag to indicate customers were served for this ride
        }
        if (customers_served) {
            printf("\n");
            usleep(2000000); // 2 second delay after serving each ride if customers were served
        }
    }
}


void process_extra_waiting_line(Customer ride_customers[MAX_CUSTOMERS]) {
    printf("\nProcessing customers from the extra waiting line:\n");


    char ride_names[NUM_RIDES][50] = {"Roller Coaster", "Ferris Wheel", "Bumper Cars", "Gravity Drop", "Flying Carpet"};


    for (int i = 0; i < extra_waiting_line.num_extra_waiting_customers; i++)
     {
        Customer* customer = &extra_waiting_line.extra_waiting_customers[i];
       
        int anyRideAvailable = 0; // Flag to track if any ride is available


        printf("\nAvailable rides:\n");
        for (int j = 0; j < NUM_RIDES; j++)
        {
            if (rides[j].capacity > 0) {
                printf("%d. %s\n", j + 1, ride_names[j]);
                anyRideAvailable = 1; // Set the flag to indicate that at least one ride is available
            }
        }


        // If no ride is available, print a message and move to the next customer
        if (!anyRideAvailable) {
            printf("None\n"); // Print "None" if no rides are available
            printf("\n All rides are full. %s, you can roam around the Park and explore other facilities.\n", customer->name);
            continue; // Move to the next customer
        }
         
        int chosen_ride;
        do {
            printf("\n Enter ride preference for Customer %s (1, 2, ..., 5): ", customer->name);
            scanf("%d", &chosen_ride);
            if (chosen_ride < 1 || chosen_ride > NUM_RIDES)
            {
                printf("Invalid ride preference. Please choose again.\n");
                continue;
            }
            if (rides[chosen_ride - 1].capacity <= 0) {
                printf("Maximum capacity for Ride %d reached.\n", chosen_ride);
                printf("Please choose another ride.\n");
                continue;
            }
            break;
        } while (1);
       
        // Enqueue into the chosen ride's waiting line
        pthread_mutex_lock(&ride_mutexes[chosen_ride - 1]);
       
        enqueue(&rides[chosen_ride - 1], customer);
        printf("\n Customer %s entered Ride %d\n", customer->name, chosen_ride);
        printf("Customer %s queued for Ride %d\n", customer->name, chosen_ride);
        
        pthread_mutex_unlock(&ride_mutexes[chosen_ride - 1]);
         
        // Update ride capacity after enqueuing
        rides[chosen_ride - 1].capacity--; // Decrement ride capacity


        // Introduce a delay after queuing each customer
        usleep(1000000); // 1 second delay
    }
   
    printf("\nAll customers from the extra waiting line have been queued.\n");
    serve_customers_in_waiting_lines();
}




void serve_customers_in_waiting_lines() {
    printf("\nServing customers from waiting lines:\n");

    for (int ride = 0; ride < NUM_RIDES; ride++)
    {
        Ride* current_ride = &rides[ride];
        int customers_served = 0;  // Flag to track if customers were served for this ride
       
        while (current_ride->queue != NULL) {
            CustomerNode* current_customer = dequeue(current_ride);
             // Update revenue
            current_ride->revenue += current_ride->price;
            ride_revenue += current_ride->price;
            
            free(current_customer);
           
            // Introduce a delay after serving each customer
            usleep(1000000); // 1 second delay
            customers_served = 1; // Set the flag to indicate customers were served for this ride
        }
        if (customers_served) {
            printf("\n");
            usleep(2000000); // 2 second delay after serving each ride if customers were served
        }
    }
 
}


CustomerNode* enqueue(Ride* ride, Customer* customer) {
    CustomerNode* new_node = (CustomerNode*)malloc(sizeof(CustomerNode));
   
    if (new_node == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
   
    strcpy(new_node->name, customer->name);
    new_node->ride = customer->ride;
    new_node->isVIP = customer->isVIP;
    new_node->next = NULL;


    if (ride->queue == NULL) {
        ride->queue = new_node;
    }
    else {
        CustomerNode* current = ride->queue;
       
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_node;
    }


    return new_node;
  }  
 
//'dequeue' function is responsible for removing a customer from the front of a ride's waiting line.
CustomerNode* dequeue(Ride* ride)
  {
    if (ride->queue == NULL)
    {
        return NULL;
    }
    CustomerNode* dequeued_customer = ride->queue;
    ride->queue = ride->queue->next;
    if (ride->queue == NULL)
    {
        // If the queue becomes empty, update the ride's queue pointer to NULL
        ride->queue = NULL;
    }
    dequeued_customer->next = NULL;
    return dequeued_customer;
  }

void display_revenue() {
    printf("\nRevenue generated by each ride:\n");
    for (int ride = 0; ride < NUM_RIDES; ride++) {
        printf("Ride %d: $%.2f\n", ride + 1, rides[ride].revenue);
    }
    printf("Total revenue from all rides: $%.2f\n", ride_revenue);
}

