// ADT - Deck (stack that can be accessed from both ends)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "deck.h"

void waitForInput()
{
    printf("Press any key to continue...");
    fflush(stdin);
    while( getchar() != '\n' );
}

int askForData()
{
    printf("Enter data to push: ");
    int data;
    scanf("%d", &data);
    return data;
}

int askForDeck(int deckCounter, Deck **deck)
{
    if (deckCounter == 0 || deck == NULL)
        return -2;

    int idToCheck;
    printf("Available deck ID's: ");
    for (int i = 0; i < deckCounter; i++)
    {
        printf("%d ", deck[i]->id);
    }
    printf("\nEnter deck ID: ");
    scanf("%d", &idToCheck);
    if (checkID(idToCheck, deckCounter, deck) == 0)
        return idToCheck;
    else
        return -1;
}

int getPosition(Deck **deck, int deckCounter, int id)
{
    for (int i = 0; i < deckCounter; i++)
    {
        if (deck[i]->id == id)
            return i;
    }
    return -1;
}

Deck **addToArray(Deck **ptr_array, Deck *ptr, int *elementCount)
{
    printf("Element count in func: %d\n", *elementCount);
    if (ptr_array == NULL)
    {
        ptr_array = (Deck **) malloc(sizeof(Deck *));
        ptr_array[0] = ptr;
        *elementCount = 1;
    }
    else
    {
        *elementCount = *elementCount + 1;
        ptr_array = (Deck **) realloc(ptr_array, sizeof(Deck *) * (*elementCount));
        ptr_array[*elementCount-1] = ptr;
    }
    return ptr_array;
}

void menu()
{
    bool showMenu = true;
    int choice;
    int idCount = 0; // deck id counter
    int deckCounter = 0; // deck counter
    Deck **deck_arr = NULL; // array of decks
    Deck *dq;
    int idToCheck, dataToPush, position, pop;
    while(showMenu)
    {
        fflush(stdin);
        system("clear");
        printf("1. Create deck\n");
        printf("2. Check if deck is empty\n");
        printf("3. Check if deck is full\n");
        printf("4. Push to front\n");
        printf("5. Push to back\n");
        printf("6. Pop from front\n");
        printf("7. Pop from back\n");
        printf("8. Print deck\n");
        printf("9. Destroy deck\n");
        printf("0. Exit\n");
        // printf("Deck Counter: %d\n", deckCounter);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                dq = createDeck(idCount);
                idCount++;
                deck_arr = addToArray(deck_arr, dq, &deckCounter);
                // printf("DeckCntr: %d | idCount: %d\n", deckCounter, idCount);
                // deck = deckInit(&deckCounter, &idCount);
                // if(deckCounter == 0)
                // {
                //     deckCounter++;
                //     idCount++;
                //     deck = malloc(sizeof(Deck *) * deckCounter);
                //     if (deck == NULL)
                //     {
                //         printf("Error allocating memory");
                //         exit(1);
                //     }
                // }
                // else
                // {
                //     deckCounter++;
                //     idCount++;
                //     deck = realloc(deck, sizeof(Deck *) * deckCounter);
                //     if (deck == NULL)
                //     {
                //         printf("Error allocating memory");
                //         exit(1);
                //     }
                // }
                // printf("FUNC\n");
                // deck[idCount-1] = createDeck(deckCounter);
                system("clear");
                printf("Deck created with id: %d\n", idCount-1);
                waitForInput();
                break;
            case 2:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    if (checkEmpty(deck_arr[idToCheck]) == 0)
                        printf("Deck is empty.\n");
                    else
                        printf("Deck is not empty.\n");
                }
                else
                {
                    printf("Incorrect ID.");
                }
                waitForInput();
                break;
            case 3:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    if (checkFull(deck_arr[idToCheck]) == 0)
                        printf("Deck is full.\n");
                    else
                        printf("Deck is not full.\n");
                }
                else
                {
                    printf("Incorrect ID.");
                }
                waitForInput();
                break;
            case 4:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    dataToPush = askForData();
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    pushFront(deck_arr[position], dataToPush);
                }
                else
                {
                    printf("Incorrect ID.");
                }
                waitForInput();
                break;
            case 5:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    dataToPush = askForData();
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    pushBack(deck_arr[position], dataToPush);
                }
                else
                {
                    printf("Incorrect ID.");
                }
                waitForInput();
                break;
            case 6:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    popFront(deck_arr[position], &pop);
                    // waitForInput();
                }
                else
                {
                    printf("Incorrect ID!\n");
                    // waitForInput();
                }
                waitForInput();
                break;
            case 7:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    int pop;
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    popBack(deck_arr[position], &pop);
                    // waitForInput();
                }
                else
                {
                    printf("Incorrect ID!\n");
                    // waitForInput();
                }
                waitForInput();
                break;
            case 8:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    position = getPosition(deck_arr, deckCounter, idToCheck);
                    print(deck_arr[position]->head);
                }
                else
                {
                    printf("Incorrect ID.");
                }
                waitForInput();
                break;
            case 9:
                idToCheck = askForDeck(deckCounter, deck_arr);
                if (idToCheck == -2)
                    printf("No decks available\n");
                else if (idToCheck != -1)
                {
                    deck_arr = removeDeckFromArr(deck_arr, &deckCounter, idToCheck);
                    printf("Deck with id %d has been removed!\n", idToCheck);
                }
                waitForInput();
                break;
            case 0:
                printf("Exiting program...\n");
                exit(0);
                break;
            default:
                printf("Invalid choice, try again");
        }
    }
}

int main()
{   
    menu();
    // int elements = 0;
    // Deck *dq = createDeck(0);
    // Deck *dqq = createDeck(1);
    // pushFront(dq, 5);
    // pushFront(dq, 25);
    // pushFront(dqq, 12);
    // pushFront(dqq, 14);
    // Deck **ptr_arr = NULL;
    // ptr_arr = addToArray(ptr_arr, dq, &elements);
    // ptr_arr = addToArray(ptr_arr, dqq, &elements);
    // printf("Elements: %d\n", elements);
    // print(ptr_arr[0]->head);
    // print(ptr_arr[1]->head);
    return 0;
}

/*
Deck functions:
    createDeck(int id);                 // Creates deck element and assigns an id to it. Example: Deck *dq = createDeck(0);
    checkEmpty(Deck *d);                // Checks if deck is empty (returns 0 if true and -1 if false)
    checkFull(Deck *d);                 // Checks if deck is full (returns 0 if true and -1 if false)
    print(Node *n);                     // Prints deck elements to the screen
    pushFront(Deck *d, int n_data);     // Pushes elements to the front of the deck
    pushBack(Deck *d, int n_data);      // Pushes elements to the back of the deck
    popFront(Deck *d);                  // Pops element from the front of the deck. Example: int x = popFront(dq);
    addToArray(Deck **ptr_array, Deck *ptr, int *elementCount);

Menu functions:
    waitForInput();
    checkID(int idToCheck, int deckCounter, Deck **deck);
    askForDeck(int deckCounter, Deck **deck);

Make int elements variable (int elements = 0;)
Create Deck element and assign it to pointer (Deck *dq = createDeck(0));

*/