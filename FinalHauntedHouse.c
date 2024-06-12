#include "FinalHauntedHouse.h"

SpookyEvent spookyEvents[NUM_EVENTS] = {
    {" You hear ghostly whispers from the shadows. The whispers seem to call your name, sending shivers down your spine."},
    {" A cold chill runs down your spine as you feel a presence behind you. You turn around, but there's nothing there, just the darkness looming."},
    {" You see eerie flickering lights down a dark hallway. As you approach, the lights flicker out one by one, leaving you in complete darkness."},
    {" The floorboards creak beneath your feet with each step. Suddenly, you feel a hand grab your ankle, but when you look down, there's no one there."},
    {" A sudden gust of wind extinguishes the candles, plunging the room into darkness. You hear faint whispers and footsteps approaching you from all sides, but you can't see anything."}
};

char doorOptions[NUM_DOORS][12] = {"A", "B", "C", "D", "Exit Door"};

char *questions[NUM_QUESTIONS] = {
    "What is the capital of France?",
    "How many continents are there in the world?",
    "What is the chemical symbol for water?",
    "When did Pakistan win World Cup'?",
    "What year did the Titanic sink?"
};

char *answers[NUM_QUESTIONS] = {
    "Paris",
    "7",
    "H2O",
    "1992",
    "1912"
};
pthread_mutex_t mutex; // Mutex for ticketPrice access
pthread_mutex_t feedbackMutex; // Mutex for feedback data access
char globalFeedbackArray[MAX_VISITORS][MAX_FEEDBACK_LENGTH];
float globalRatingsArray[MAX_VISITORS];
void *visitorThread(void *visitor) {
    Visitor *visitor_data = (Visitor *)visitor;
    pthread_mutex_lock(&mutex);
    
    hauntedexperience(visitor_data);
    
    pthread_mutex_unlock(&mutex);
    return NULL;
}
/*The visitorThread function is responsible for executing the haunted house experience for each visitor. 
This function is run by multiple threads concurrently to handle multiple visitors simultaneously. 
However, because each thread may access and modify shared resources, such as the Visitor data structure and the spooky events array, there is a risk of data corruption or inconsistent behavior if proper synchronization is not ensured. 
Therefore mutex locking and unlocking is important so that critical section of code is used by one visitor thread at one time. */

void inviteVisitors(Visitor *visitors, int numVisitors) {
    for (int i = 0; i < numVisitors; i++) {
        printf("\nHey Visitor %d!\n", i + 1);
        
        printf("Please enter your name: ");
        
        char name[MAX_NAME_LENGTH];
        
        bool validName = false;
        while (!validName) {
        
            scanf("%s", name);
            
            bool hasDigits = false;
            for (int j = 0; name[j] != '\0'; j++) {
                if (isdigit(name[j])) {
                    hasDigits = true;
                    break;
                }
            }
            if (hasDigits) {
                printf("Invalid input! Name should not contain numbers. Please enter your name again: ");
            } 
            else {
                validName = true;
            }
        }
       
        strcpy(visitors[i].name, name);

        printf("Please enter your age: ");
        while (1) {
            char ageInput[10];
            
            // Read the age input as a string
            scanf("%s", ageInput);
            bool isValidAge = true;
            
            // Check if the age input contains non-numeric characters
            for (int j = 0; ageInput[j] != '\0'; j++) {
                if (!isdigit(ageInput[j])) {
                    isValidAge = false;
                    break;
                }
            }
            
            if (!isValidAge) {
                printf("Invalid input! Age must be a number.\n Please enter your age again: ");
                while (getchar() != '\n');
                continue;
            }
            // Convert the age input to an integer
            int age = atoi(ageInput);
            
            if (age < 1 || age > 100) {
                printf("Invalid input! Age must be between 1 and 100. Please enter your age again: ");
                scanf("%d",&age);
            } 
            else {
                visitors[i].age = age;
                break;
            }
        }
        // Check if the visitor is underage
        if (visitors[i].age < 18) {
            printf("Sorry, %s, you are underage. You are not allowed inside the haunted house.\n", visitors[i].name);
            i--; // Go back to the previous visitor index to ask for their details again
        } 
        else {
            printf("Ticket generated! Price: $%d\n", TICKET_PRICE);
            visitors[i].ticketPrice = TICKET_PRICE;
        }
    }
}

void hauntedexperience(Visitor *visitor) {
    printf("\nWelcome, %s, to the Haunted House Experience!\n", visitor->name);
    printf("Get ready for a spine-chilling adventure...\n");

    int correctAnswers = 0; 
    
    for (int i = 0; i < NUM_DOORS; i++) {
        printf("\n DOOR: %s \n\n", doorOptions[i]);
        printf("\nYou stand before door %s, ready to enter...\n", doorOptions[i]);
        printf("Behind this door, you'll face an IQ question.\n");
        printf("\n Question: %s\n", questions[i]);
        printf("Your answer: ");
        char userAnswer[500];
        scanf("%s", userAnswer);
        
        if (strcasecmp(userAnswer, answers[i]) == 0) {
            printf("Correct answer! You may proceed to the next door.\n");
            correctAnswers++;
        } 
        else {
            printf("Incorrect answer! You must face a spooky experience...\n \n ");
            usleep(10000);
            printf("%s\n", spookyEvents[i].event);
        }
    }
    
    if (correctAnswers == NUM_DOORS) {
        visitor->allAnswersCorrect = true; // Set flag to true
    } else {
        visitor->allAnswersCorrect = false; // Set flag to false
    }
    
    printf("\n %s! You have finally navigated through the Haunted House.\n", visitor->name);
    printf("Thank you for visiting!\n");
    printf("-------------------------------------\n");
}

void askforExperienceAgain(Visitor *visitors, int numVisitors) {
     printf("\n ASKING THE VISITORS IF THEY WOULD LIKE TO EXPERIENCE THE HAUNTED HOUSE AGAIN \n \n" );

    for (int i = 0; i < numVisitors; i++) {
        printf("Visitor %d :  %s , ",i+1, visitors[i].name);
        printf("Would you like to take the haunted house experience again? (yes/no): ");
        char choice[100];
        while (1) {
            scanf("%s", choice);
            if (strcasecmp(choice, "yes") == 0 || strcasecmp(choice, "no") == 0) {
                break; // Exit the loop if a valid choice is entered
            } else {
                printf("Invalid choice! Please enter 'yes' or 'no': ");
                while (getchar() != '\n'); // Clear input buffer
            }
        }
        if (strcasecmp(choice, "no") == 0) {
            printf("\n Thanks for coming. We hope you had fun.\n\n");
        } 
        else {
            printf("Ticket generated! Price: $%d\n", TICKET_PRICE);
            pthread_mutex_lock(&mutex); 
            visitors[i].ticketPrice += TICKET_PRICE; 
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&mutex); 
            hauntedexperience(&visitors[i]);
            usleep(1000000);
            pthread_mutex_unlock(&mutex); 
        }
    }
}

void askforFeedback(Visitor *visitors, int numVisitors) {
    printf("\nAsking visitors for feedback:\n");
    for (int i = 0; i < numVisitors; i++) {
        printf("\n Visitor %d: %s, would you like to provide feedback? (yes/no): ", i + 1, visitors[i].name);
        char choice[10];
        while (1) {
            scanf("%s", choice);
            if (strcasecmp(choice, "yes") == 0 || strcasecmp(choice, "no") == 0) {
                break;
            } 
            else {
                printf("Invalid choice! Please enter 'yes' or 'no': ");
            }
        }

        if (strcasecmp(choice, "yes") == 0) {
            printf("Please rate your experience (1-5): ");
            while (1) {
                if (scanf("%f", &visitors[i].rating) != 1 || visitors[i].rating < 1 || visitors[i].rating > 5) {
                    printf("Invalid rating! Please enter a number between 1 and 5: ");
                    while (getchar() != '\n'); // Clear input buffer
                } 
                else {
                    break;
                }
            }

            printf("Please provide your comments (max 500 characters): ");
            scanf(" %[^\n]", visitors[i].experienceComments);

            // Lock feedbackMutex before accessing global feedback and ratings arrays
            pthread_mutex_lock(&feedbackMutex);
            
            // Update global feedback and ratings arrays
            strcpy(globalFeedbackArray[i], visitors[i].experienceComments);
            
            globalRatingsArray[i] = visitors[i].rating;
            
            // Unlock feedbackMutex after updating global feedback and ratings arrays
            pthread_mutex_unlock(&feedbackMutex);
            
/*This prevents other threads from accessing or modifying the shared data while one thread is in the process of updating it. Without mutex locking, multiple threads could potentially attempt to update the global arrays simultaneously, leading to data corruption or inconsistent state.*/
        } 
        else {
            printf("Thank you for visiting, %s!\n", visitors[i].name);
        }
    }
}

void giveSurpriseGift(Visitor *visitors, int numVisitors) {
    printf("\nGiving surprise gift to visitors who answered all questions correctly:\n");
    int found = 0; // Flag to check if any visitor has all answers correct
    for (int i = 0; i < numVisitors; i++) {
        if (visitors[i].allAnswersCorrect) {
            printf("Congratulations, %s! You've won a surprise gift!\n", visitors[i].name);
            found = 1; // Set flag to true
        }
    }
    if (!found) {
        printf("No visitors had all answers correct. No surprise gifts.\n");
    }
}


void analyzeFeedback() {
 
 // Lock feedbackMutex before accessing global feedback and ratings arrays
    
    pthread_mutex_lock(&feedbackMutex);
    printf("\n--- Feedback Analysis ---\n");
    float totalRating = 0;
    int numFeedbacks = 0;
    for (int i = 0; i < MAX_VISITORS; i++) {
        if (globalRatingsArray[i] > 0) {
            totalRating += globalRatingsArray[i];
            numFeedbacks++;
        }
    }
    if (numFeedbacks > 0) {
        printf("Average Rating: %.2f\n", (float)totalRating / numFeedbacks);
        printf("Feedback Summary:\n");
        for (int i = 0; i < MAX_VISITORS; i++) {
            if (globalRatingsArray[i] > 0) {
                printf("Visitor %d: Rating - %.2f, Comments - %s\n", i + 1, globalRatingsArray[i], globalFeedbackArray[i]);
            }
        }
    } else {
        printf("No feedback provided.\n");
    }
    // Unlock feedbackMutex after accessing global feedback and ratings arrays
    pthread_mutex_unlock(&feedbackMutex);
}

