#include "FoodCourtFinal.h"

sem_t menuSemaphore;
sem_t orderSemaphore;
sem_t feedbackSemaphore;
MenuItem menu[MENU_SIZE];
double FoodCourt_revenue=0;

void *dinerThread(void *arg) {
    Diner *diner = (Diner *)arg;

    printf("Here's your receipt, %s: \n", diner->name);

    sem_wait(&menuSemaphore); // Protect access to the menu
    double total = 0;
    for (int i = 0; i < diner->numItems; i++) {
        int itemIndex = diner->orderItems[i];
        double price = menu[itemIndex].price * diner->orderQuantities[i];
        total += price;
        printf("%d x %s: £%.2f\n", diner->orderQuantities[i], menu[itemIndex].name, price);
    }
    sem_post(&menuSemaphore);

    printf("Total: £%.2f\n", total);
    printf("-------------------------------------\n");
    FoodCourt_revenue=FoodCourt_revenue+total;
    return NULL;
}

void takeOrders(Diner *diners, int numDiners) {
    for (int i = 0; i < numDiners; i++) {
        printf("\nHey Diner %d!\n", i + 1);
        printf("Please enter your name: ");
        scanf("%s", diners[i].name);
        while (getchar() != '\n');

        char input[100];
        int choice, quantity;
        diners[i].numItems = 0;

        printf("Please place your order (item number and quantity, 0 to finish):\n");

        sem_wait(&orderSemaphore); // Protect order placement
        while (1) {
            printf("Item number: ");
            if (scanf("%d", &choice) != 1) { // Check if the input is not an integer
                printf("Invalid input. Please enter a numeric value.\n");
                while (getchar() != '\n'); // Clear input buffer
                continue;
            }
            while (getchar() != '\n');
            if (choice == 0) {
                break;
            }
            if (choice < 1 || choice > MENU_SIZE) {
                printf("Invalid item number. Please choose again.\n");
                continue;
            }
            printf("Quantity: ");
            scanf("%d", &quantity);
            while (getchar() != '\n');
            while (quantity <= 0) {
                printf("Invalid quantity. Please choose again.\n");
            }
            diners[i].orderItems[diners[i].numItems] = choice - 1;
            diners[i].orderQuantities[diners[i].numItems] = quantity;
            diners[i].numItems++;
        }
        sem_post(&orderSemaphore);
    }
    printf("-------------------------------------\n");
}

void deliverOrders(Diner *diners, int numDiners) {
    for (int i = 0; i < numDiners; i++) {
        printf("%s's order is ready!\n", diners[i].name);
        printf("%s paid the bill and received the order.\n", diners[i].name);
        sleep(2);
    }
}    

void collectFeedback(Diner *diners, int numDiners) {
    int totalFoodRatings = 0;
    int totalServiceRatings = 0;
    int numDinersWithFeedback = 0;
    int numDinersWithoutFeedback = 0;
    int numFoodHighRatings = 0; // Ratings between 4 and 5
    int numServiceHighRatings = 0; // Ratings between 4 and 5
    
    printf("-------------------------------------\n");
    printf("\nFEEDBACK FORM:\n");

    for (int i = 0; i < numDiners; i++) {
        sleep(2); // Sleep for a few seconds after eating

        printf("\n");
        while (1) {
            printf("%s, would you like to provide feedback? (Yes/No): ", diners[i].name);
            scanf("%s", diners[i].feedbackConsent);
            
            // Convert input to lowercase
            for (int j = 0; diners[i].feedbackConsent[j]; j++) {
                diners[i].feedbackConsent[j] = tolower(diners[i].feedbackConsent[j]);
            }
            
            // Check if input is "yes" or "no"
            if (strcmp(diners[i].feedbackConsent, "yes") == 0 || strcmp(diners[i].feedbackConsent, "no") == 0) {
                break; // Valid input, exit the loop
            } else {
                printf("Invalid input. Please enter 'Yes' or 'No'.\n");
            }
        }

        if (strcmp(diners[i].feedbackConsent, "yes") == 0) {
            sem_wait(&feedbackSemaphore); // Protect feedback data
            printf("Please rate the food out of 5: ");
            scanf("%f", &diners[i].foodRating);
            printf("Please rate the service out of 5: ");
            scanf("%f", &diners[i].serviceRating);
            sem_post(&feedbackSemaphore);

            totalFoodRatings += diners[i].foodRating;
            totalServiceRatings += diners[i].serviceRating;

            if (diners[i].foodRating >= 4) {
                numFoodHighRatings++;
            }
            if (diners[i].serviceRating >= 4) {
                numServiceHighRatings++;
            }

            numDinersWithFeedback++;
        } else {
            printf("%s is not willing to give feedback.\n", diners[i].name);
            numDinersWithoutFeedback++;
        }
    }

    double foodPercentage = (double)numFoodHighRatings / numDinersWithFeedback * 100;
    double servicePercentage = (double)numServiceHighRatings / numDinersWithFeedback * 100;
    double feedbackPercentage = (double)numDinersWithFeedback / numDiners * 100;

    printf("\nFeedback Summary:\n\n");
    printf("Number of diners who provided feedback: %d\n", numDinersWithFeedback);
    printf("Number of diners who did not provide feedback: %d\n", numDinersWithoutFeedback);
    printf("Percentage of diners who provided feedback: %.2f%%\n", feedbackPercentage);
    
    printf("\nFood Ratings Analysis:\n");
    
    printf("Percentage of diners who rated 4 to 5 for food: %.2f%%\n", foodPercentage);
    if (foodPercentage <= 30) {
        printf("The food quality needs improvement.\n");
    } else if (foodPercentage <= 60) {
        printf("Satisfactory food quality.\n");
    } else if (foodPercentage <= 90) {
        printf("Good food quality.\n");
    } else {
        printf("Excellent food quality!\n");
    }

    printf("\nService Ratings Analysis:\n");
    
    printf("Percentage of diners who rated 4 to 5 for service: %.2f%%\n", servicePercentage);
    if (servicePercentage <= 30) {
        printf("The service needs improvement.\n");
    } else if (servicePercentage <= 60) {
        printf("Satisfactory service.\n");
    } else if (servicePercentage <= 90) {
        printf("Good service.\n");
    } else {
        printf("Excellent service!\n");
    }
}





