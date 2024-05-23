#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Subscriber
{
    int id;
    char name[50];
    int visitCount;
    int pin; // Added pin for subscriber authentication
};

struct CallHistory
{
    int callerID;
    char callType[20];
};

// Function to save subscriber information to a file
void saveSubscriberToFile(struct Subscriber subscriber)
{
    FILE *file = fopen("subscribers.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d %s %d %04d\n", subscriber.id, subscriber.name, subscriber.visitCount, subscriber.pin);
        fclose(file);
    }
    else
    {
        printf("Error opening file for writing.\n");
    }
}

// Function to save PINs to a file
void savePinsToFile(struct Subscriber subscribers[], int subscriberCount)
{
    FILE *file = fopen("pins.txt", "w");
    if (file != NULL)
    {
        for (int i = 0; i < subscriberCount; ++i)
        {
            fprintf(file, "%d %04d\n", subscribers[i].id, subscribers[i].pin);
        }
        fclose(file);
    }
    else
    {
        printf("Error opening file for writing PINs.\n");
    }
}

// Function to load PINs from a file
void loadPinsFromFile(struct Subscriber subscribers[], int *subscriberCount)
{
    FILE *file = fopen("pins.txt", "r");
    if (file != NULL)
    {
        while (fscanf(file, "%d %04d", &subscribers[*subscriberCount].id, &subscribers[*subscriberCount].pin) == 2)
        {
            (*subscriberCount)++;
        }
        fclose(file);
    }
    else
    {
        printf("Error opening file for reading PINs.\n");
    }
}

// Function to save call history to a file
void saveCallHistoryToFile(struct CallHistory callHistory)
{
    FILE *file = fopen("call_history.txt", "a");
    if (file != NULL)
    {
        fprintf(file, "%d %s\n", callHistory.callerID, callHistory.callType);
        fclose(file);
    }
    else
    {
        printf("Error opening file for writing call history.\n");
    }
}

// Function to display call history from a file
void displayCallHistoryFromFile()
{
    FILE *file = fopen("call_history.txt", "r");
    if (file != NULL)
    {
        printf("Call History:\n");
        struct CallHistory callHistory;

        // Use %s instead of %19s
        while (fscanf(file, "%d %s", &callHistory.callerID, callHistory.callType) == 2)
        {
            printf("Customer %d: %s\n", callHistory.callerID, callHistory.callType);
        }

        fclose(file);
    }
    else
    {
        printf("Error opening file for reading call history.\n");
    }
}

// Function to generate a random 4-digit PIN
int generateRandomPIN()
{
    return rand() % 10000;
}

// Function to authenticate a subscriber based on name and PIN
int authenticateSubscriber(struct Subscriber subscribers[], int subscriberCount, const char *name, int pin)
{
    for (int i = 0; i < subscriberCount; ++i)
    {
        if (strcmp(subscribers[i].name, name) == 0 && subscribers[i].pin == pin)
        {
            return i; // Return the index of the authenticated subscriber
        }
    }
    return -1; // Return -1 if authentication fails
}

int main()
{
    // Seed for random number generation
    srand((unsigned int)time(NULL));

    int max;
    printf("Enter the maximum number of customers the telephone booth can serve: ");
    scanf("%d", &max);

    int queue[max];
    int customerIDs[max];
    char customerNames[max][50];
    int rear = -1, front = -1;
    int choice, customer, uniqueID = 1;
    int totalEarnings = 0;
    int totalServedCustomers = 0;

    struct Subscriber subscribers[max];
    int subscriberCount = 0;

    // Load PINs from file
    loadPinsFromFile(subscribers, &subscriberCount);

    int subscriberID; // Move the declaration outside the switch block

    while (1)
    {
        printf("Telephone Booth Queue Management:\n");
        printf("1) Add customer\n2) Serve customer\n3) Display subscribers\n4) Total Served Customers\n5) Total Earnings\n6) Subscriber Login\n7)  Display Call History\n8) exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                // Add customer (including subscriber creation)
                if (((rear + 1) % max) == front)
                {
                    printf("Queue is full. Cannot add more customers.\n");
                }
                else
                {
                    if (front == -1 && rear == -1)
                    {
                        front = rear = 0;
                    }
                    else
                    {
                        rear = (rear + 1) % max;
                    }
                    printf("Enter the customer number: ");
                    scanf("%d", &customer);

                    // Assigning customer ID
                    customerIDs[rear] = uniqueID;

                    printf("Customer %d, please pay 5 Rs before using the telephone.\n", customer);
                    printf("Did the customer pay? (1 for Yes, 0 for No): ");
                    int paid;
                    scanf("%d", &paid);

                    if (paid == 1)
                    {
                        printf("Customer %d, choose the type of call:\n", customer);
                        printf("1) Local Call\n2) Abroad Call\n");
                        int callType;
                        scanf("%d", &callType);

                        if (callType == 1)
                        {
                            printf("Local call initiated for Customer %d.\n", customer);
                            printf("Local call details: Enter local number, duration, etc.\n");

                            struct CallHistory callHistory;
                            callHistory.callerID = customerIDs[rear];
                            strcpy(callHistory.callType, "Local Call");
                            saveCallHistoryToFile(callHistory);

                            if (customerIDs[rear] != 0)
                            {
                                totalEarnings += 5; // Assuming a fixed charge for local calls
                                totalServedCustomers++;
                            }
                        }
                        else if (callType == 2)
                        {
                            printf("Abroad call initiated for Customer %d.\n", customer);
                            printf("Enter the abroad number: ");
                            char abroadNumber[20];
                            scanf("%19s", abroadNumber);

                            struct CallHistory callHistory;
                            callHistory.callerID = customerIDs[rear];
                            sprintf(callHistory.callType, "Abroad Call: %s", abroadNumber);
                            saveCallHistoryToFile(callHistory);

                            if (customerIDs[rear] != 0)
                            {
                                totalEarnings += 10; // Assuming a fixed charge for abroad calls
                                totalServedCustomers++;
                            }
                        }
                        else
                        {
                            printf("Invalid choice. Call type not recognized.\n");
                        }

                        // Continue with the rest of the code for subscription and queue addition
                        printf("Would you like to purchase a yearly subscription for 250 Rs? (1 for Yes, 0 for No): ");
                        int subscribe;
                        scanf("%d", &subscribe);

                        if (subscribe == 1)
                        {
                            if (customerIDs[rear] != 0)
                            {
                                totalEarnings += 250;

                                printf("Enter the customer name: ");
                                scanf("%49s", customerNames[uniqueID - 1]);

                                struct Subscriber subscriber;
                                subscriber.id = uniqueID;
                                strcpy(subscriber.name, customerNames[uniqueID - 1]);
                                subscriber.visitCount = 1;
                                subscriber.pin = generateRandomPIN();

                                subscribers[subscriberCount] = subscriber;
                                subscriberCount++;

                                saveSubscriberToFile(subscriber);
                                // Save PINs to file after adding a new subscriber
                                savePinsToFile(subscribers, subscriberCount);

                                printf("Customer %d, your yearly subscription is confirmed. Welcome, %s! Your PIN is: %04d\n", customer, customerNames[uniqueID - 1], subscriber.pin);
                            }
                        }
                        else
                        {
                            customerIDs[rear] = 0;
                        }

                        printf("Customer %d added to the queue.\n", customer);
                        uniqueID++;
                    }
                    else
                    {
                        printf("Customer %d did not pay and was not added to the queue.\n", customer);
                        rear = (rear - 1 + max) % max;
                    }
                }
                break;

            case 2:
                // Serve customer
                if (front == -1 && rear == -1)
                {
                    printf("Queue is empty. No customers to serve.\n");
                }
                else
                {
                    printf("Serving customers...\n");

                    while (front != -1)
                    {
                        printf("Serving customer %d\n", customerIDs[front]);

                        // Increment visit count for the subscriber
                        int currentID = customerIDs[front] - 1;
                        if (currentID >= 0 && currentID < max)
                        {
                            customerNames[currentID][49]++; // Increment the visit count
                        }

                        if (front == rear)
                        {
                            front = rear = -1;
                        }
                        else
                        {
                            front = (front + 1) % max;
                        }
                    }

                    printf("Total customers served: %d\n", totalServedCustomers);
                }
                break;

            case 3:
                // Display subscribers
                if (front == -1 && rear == -1)
                {
                    printf("No subscribers at the moment.\n");
                }
                else
                {
                    int i;
                    printf("Subscribers: ");
                    for (i = front; i != rear; i = (i + 1) % max)
                    {
                        if (customerIDs[i] != 0)
                        {
                            printf("%s ", customerNames[customerIDs[i] - 1]);
                        }
                    }
                    if (customerIDs[i] != 0)
                    {
                        printf("%s\n", customerNames[customerIDs[i] - 1]);
                    }
                    else
                    {
                        printf("\n");
                    }
                }
                break;

            case 4:
                // Total Served Customers
                printf("Total served customers: %d\n", totalServedCustomers);
                break;

            case 5:
                // Total Earnings
                printf("Total earnings: %d Rs\n", totalEarnings);
                break;

            case 6:
                // Subscriber Login
                printf("Enter your name: ");
                char subscriberName[50];
                scanf("%s", subscriberName);

                printf("Enter your pin: ");
                int subscriberPin;
                scanf("%d", &subscriberPin);

                subscriberID = authenticateSubscriber(subscribers, subscriberCount, subscriberName, subscriberPin);
                if (subscriberID != -1)
                {
                    printf("Subscriber %s logged in successfully.\n", subscribers[subscriberID].name);

                    // Prompt subscriber to choose the type of call
                    int callChoice;
                    printf("Choose the type of call:\n");
                    printf("1) Local Call\n2) Abroad Call\n");
                    scanf("%d", &callChoice);

                    switch (callChoice)
                    {
                        case 1:
                            // Local call
                            printf("Local call initiated.\n");
                            printf("Local call details: Enter local number, duration, etc.\n");

                            // Save call details for successful logins
                            struct CallHistory callHistory;
                            callHistory.callerID = subscribers[subscriberID].id;
                            strcpy(callHistory.callType, "Local Call");
                            saveCallHistoryToFile(callHistory);

                            // Increment visit count for the subscriber
                            subscribers[subscriberID].visitCount++;

                            printf("Subscriber %s, your local call is complete.\n", subscribers[subscriberID].name);
                            break;

                        case 2:
                            // Abroad call
                            printf("Enter the abroad number: ");
                            char abroadNumber[20];
                            scanf("%19s", abroadNumber);

                            printf("Abroad call initiated for number %s.\n", abroadNumber);
                            printf("Abroad call details: Enter duration, etc.\n");

                            // Save call details for successful logins
                            struct CallHistory abroadCallHistory;
                            abroadCallHistory.callerID = subscribers[subscriberID].id;
                            sprintf(abroadCallHistory.callType, "Abroad Call: %s", abroadNumber);
                            saveCallHistoryToFile(abroadCallHistory);

                            // Increment visit count for the subscriber
                            subscribers[subscriberID].visitCount++;

                            printf("Subscriber %s, your abroad call to %s is complete.\n", subscribers[subscriberID].name, abroadNumber);
                            break;

                        default:
                            printf("Invalid call choice.\n");
                            break;
                    }
                }
                else
                {
                    printf("Invalid name or pin. Subscriber login failed.\n");
                }
                break;

            case 7:
                // Display Call History
                displayCallHistoryFromFile();
                break;

            case 8:
                // Exit
                printf("Exiting the telephone booth program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
