#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>

#include "FinalHauntedHouse.h"
#include "FoodCourtFinal.h"
#include "rides.h"
#include "image_viewer.h"

#define MAX_CUSTOMERS 1000
#define MAX_EXTRA_WAITING_CUSTOMERS 1000
#define NUM_RIDES 5



int main(int argc, char *argv[]) {
// Run the image viewer
    image_viewer_main(argc, argv);

system("clear");
// Welcome message to the theme park
    printf("\n----------------------   WELCOME TO THEME PARK!  -----------------------\n\n Get ready for an adventure-filled day with thrilling rides, delicious food, entertaining shows, and a spooky haunted house!\n\n");
   

    int num_customers;
    Customer ride_customers[MAX_CUSTOMERS];
    pthread_t customer_threads[MAX_CUSTOMERS];

    // Initialize rides with varying capacities
    int ride_capacities[NUM_RIDES] = {3, 3, 4, 2, 1}; // varying ride capacities implemented
   
    for (int i = 0; i < NUM_RIDES; i++) {
        sem_init(&rides[i].sem_capacity, 0, ride_capacities[i]);
        rides[i].queue = NULL;
        rides[i].capacity = ride_capacities[i];
        pthread_mutex_init(&ride_mutexes[i], NULL);
    }

    // Get the number of customers from user input
     printf ("\n----------------RIDES ADVENTURE------------------\n");
   
    printf("\nCustomers are about to enjoy thrilling rides and will now be asked for their ride preferences.\n\n");
   
    printf("Enter the TOTAL number of customers going to the rides (1 to 1000): ");
    int valid_input = 0;
    do {
    if (scanf("%d", &num_customers) != 1 || num_customers < 1 || num_customers > 1000) {
        printf("Invalid input. Please enter a number between 1 and 1000: ");
        // Clear the input buffer
        while (getchar() != '\n');
    }
    else {
        valid_input = 1;
    }
    } while (!valid_input);

    // Generate tickets for customers
    generate_tickets(num_customers, ride_customers);

    // Display customer IDs for each ride category
    display_customer_ids(num_customers, ride_customers);
     usleep(1000000); // 1 second delay
     
    // Create customer threads
    for (int i = 0; i < num_customers; i++) {
        if (pthread_create(&customer_threads[i], NULL, customer_thread, (void*)&ride_customers[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for customer threads to finish
    for (int i = 0; i < num_customers; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    // Display and serve customers according to the specified order
    display_and_serve_customers(num_customers, ride_customers);
   
   // Reset ride capacities after serving customers
    for (int ride = 0; ride < NUM_RIDES; ride++) {
    sem_destroy(&rides[ride].sem_capacity); // Destroy the existing semaphore
    sem_init(&rides[ride].sem_capacity, 0, ride_capacities[ride]); // Reset capacity
    }

   // Reset ride queues
    for (int i = 0; i < NUM_RIDES; i++) {
        rides[i].queue = NULL;
        rides[i].capacity = ride_capacities[i];
    }
   
   // Process customers from the extra waiting line
   process_extra_waiting_line(ride_customers);
   
   display_revenue();
   
   printf("\n\n We hope everyone had a fantastic time enjoying the rides at our theme park!\n");

    // Destroy semaphores
    for (int i = 0; i < NUM_RIDES; i++) {
        sem_destroy(&rides[i].sem_capacity);
        pthread_mutex_destroy(&ride_mutexes[i]);
    }

   


  
    printf("\n-------------------HAUNTED HOUSE-------------------\n\n");
    printf("\nWelcome to the Haunted House Experience!\n");
    srand(time(NULL));
    
    int NUM_VISITORS;
    int hauntedHouse_revenue = 0;

    printf("Enter the number of visitors (up to %d): ", MAX_VISITORS);
    while (1) {
        char input[10];
        scanf("%s", input);
        // Check if the input contains non-numeric characters
        bool isValidInput = true;
        for (int i = 0; input[i] != '\0'; i++) {
            if (!isdigit(input[i])) {
                isValidInput = false;
                break;
            }
        }
        if (!isValidInput) {
            printf("Invalid input! Please enter a valid number of visitors: ");
        }
        else {
            NUM_VISITORS = atoi(input);  // Convert input to integer
            if (NUM_VISITORS < 1 || NUM_VISITORS > MAX_VISITORS) {
                printf("Invalid input! Please enter a number between 1 and %d: ", MAX_VISITORS);
            }
            else {
                break;
            }
        }
    }
    pthread_t visitorThreads[NUM_VISITORS];
    Visitor visitors[NUM_VISITORS];
  
    // Error handling for mutex initialization
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Mutex initialization failed\n");
        return 1; // Exit the program with an error code
    }
    if (pthread_mutex_init(&feedbackMutex, NULL) != 0) {
        printf("Feedback mutex initialization failed\n");
        pthread_mutex_destroy(&mutex); // Clean up previously initialized mutex
        return 1; // Exit the program with an error code
    }

    inviteVisitors(visitors, NUM_VISITORS);

    for (int i = 0; i < NUM_VISITORS; i++) {
        
        if (pthread_create(&visitorThreads[i], NULL, visitorThread, &visitors[i]) != 0) {
            perror("pthread_create");
            // Handle the error, such as logging it or taking alternative actions
        }
        usleep(10000);
       
    }

    for (int i = 0; i < NUM_VISITORS; i++) {
        if (pthread_join(visitorThreads[i], NULL) != 0) {
           perror("pthread_join error");
           exit(EXIT_FAILURE);
        }

    }
    
    askforExperienceAgain(visitors, NUM_VISITORS);
    
    for (int i = 0; i < NUM_VISITORS; i++) {
        hauntedHouse_revenue += visitors[i].ticketPrice;
    }
    
    printf("\nTotal Revenue Generated: $%d\n\n", hauntedHouse_revenue);
    
    askforFeedback(visitors, NUM_VISITORS);
    
    analyzeFeedback();
    
    giveSurpriseGift(visitors, NUM_VISITORS); 
    
    if (pthread_mutex_destroy(&mutex) != 0) {
    perror("pthread_mutex_destroy");
    // Handle the error, such as logging it or taking alternative actions
    }

    if (pthread_mutex_destroy(&feedbackMutex) != 0) {
    perror("pthread_mutex_destroy");
    // Handle the error, such as logging it or taking alternative actions
    }

    
	printf("\n-------------------FOOD COURT-------------------\n\n");
	printf("\n Welcome to our taste-packed food court, where every dish is crafted to perfection, promising a journey of culinary bliss!\n\n");
         if (sem_init(&menuSemaphore, 0, 1) != 0 ||
        sem_init(&orderSemaphore, 0, 1) != 0 ||
        sem_init(&feedbackSemaphore, 0, 1) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }
	srand(time(NULL));
	int NUM_DINERS;

	printf("Enter the number of diners: ");
	scanf("%d", &NUM_DINERS);
	while (getchar() != '\n');

	pthread_t dinerThreads[NUM_DINERS];
	Diner diners[NUM_DINERS];
//Open file 
    FILE *menu_file = fopen("menu.csv", "r");
if (menu_file == NULL) {
    printf("Error opening file.\n");
    return 1;
}

// Skip the header line
char header[256];
fgets(header, sizeof(header), menu_file);

// Read each line from the file and parse it
char line[256];
int count = 0;
while (fgets(line, sizeof(line), menu_file)) {
    // Split the line into item name and price
    char *token = strtok(line, ",");
    strcpy(menu[count].name, token); // Copy item name to MenuItem struct

    token = strtok(NULL, ",");
    menu[count].price = atof(token); // Convert price to double and store in MenuItem struct

    count++;
}

// Close the file
fclose(menu_file);


    // Print the menu items and prices
    printf("Menu:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s - £%.2f\n", i + 1, menu[i].name, menu[i].price);
    }

	takeOrders(diners, NUM_DINERS);

	for (int i = 0; i < NUM_DINERS; i++) {
    	if (pthread_create(&dinerThreads[i], NULL, dinerThread, &diners[i]) != 0) {
        perror("pthread_create");
        // Handle the error, such as logging it or taking alternative actions
        }
    	usleep(100000);
	}

	for (int i = 0; i < NUM_DINERS; i++) {
    	if (pthread_join(dinerThreads[i], NULL) != 0) {
        perror("pthread_join");
        // Handle the error, such as logging it or taking alternative actions
        }
	}

	printf("Order delivery process begins:\n");
	deliverOrders(diners, NUM_DINERS);

	collectFeedback(diners, NUM_DINERS);
	
	if (sem_destroy(&menuSemaphore) != 0) {
           perror("Error destroying menu semaphore");
           exit(EXIT_FAILURE);
        }
        if (sem_destroy(&orderSemaphore) != 0) {
           perror("Error destroying order semaphore");
           exit(EXIT_FAILURE);
        }
        if (sem_destroy(&feedbackSemaphore) != 0) {
           perror("Error destroying feedback semaphore");
           exit(EXIT_FAILURE);
        } 
    
    //Total revenue 
     
     FILE *revenue_file = fopen("revenue.csv", "w");
    if (revenue_file == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    // Write headers
    fprintf(revenue_file, "Category, Revenue\n");
    
    // Write rides revenue
    fprintf(revenue_file, "Rides, %.2f\n", ride_revenue);

    // Write haunted house revenue
    fprintf(revenue_file, "Haunted House, %d\n", hauntedHouse_revenue);

    // Write food court revenue
    fprintf(revenue_file, "Food Court, %.2f\n", FoodCourt_revenue);

    // Calculate total revenue
    double totalRevenue = ride_revenue + hauntedHouse_revenue + FoodCourt_revenue;

    // Write total revenue
    fprintf(revenue_file, "Total Revenue, %.2f\n", totalRevenue);

    fclose(revenue_file);
    
     printf("Revenue data has been written to revenue.csv.\n");
     
     return 0;  
}
