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

int checkID(int idToCheck, int deckCounter, Deck **deck)
{
    for (int i = 0; i < deckCounter; i++)
    {
        if (deck[i]->id == idToCheck)
            return 0;
    }
    return -1;
}

int askForDeck(int deckCounter, Deck **deck)
{
    int idToCheck;
    printf("Available deck ID's: ");
    for (int i = 0; i < deckCounter; i++)
    {
        printf("%d ", deck[i]->id);
    }
    printf("Enter deck ID: ");
    scanf("%d", &idToCheck);
    return checkID(idToCheck, deckCounter, deck);
}

Deck **addToArray(Deck **ptr_array, Deck *ptr, int *elementCount)
{
    if (ptr_array == NULL)
    {
        ptr_array = (Deck **) malloc(sizeof(Deck *));
        ptr_array[0] = ptr;
        *elementCount = 1;
    }
    else
    {
        ptr_array = (Deck **) realloc(ptr_array, sizeof(Deck *) * (*elementCount));
        ptr_array[*elementCount-1] = ptr;
        *elementCount = *elementCount + 1;
    }
    return ptr_array;
}

void menu()
{
    bool showMenu = true;
    int choice;
    int idCount = 0; // deck id counter
    int deckCounter = 0; // deck counter
    Deck **deck = NULL; // array of decks
    int idToCheck, dataToPush;
    while(showMenu)
    {
        system("clear");
        printf("1. Create deck\n");
        printf("2. Check if deck is empty\n");
        printf("3. Check if deck is full PRINT PRINT\n");
        printf("4. Push to front\n");
        printf("5. Push to back\n");
        printf("6. Pop from front\n");
        printf("7. Pop from back\n");
        printf("8. Print deck\n");
        printf("9. Destroy deck\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                // printf("DeckCntr: %d | idCount: %d\n", deckCounter, idCount);
                // deck = deckInit(&deckCounter, &idCount);
                if(deckCounter == 0)
                {
                    deckCounter++;
                    idCount++;
                    deck = malloc(sizeof(Deck *) * deckCounter);
                    if (deck == NULL)
                    {
                        printf("Error allocating memory");
                        exit(1);
                    }
                }
                else
                {
                    deckCounter++;
                    idCount++;
                    deck = realloc(deck, sizeof(Deck *) * deckCounter);
                    if (deck == NULL)
                    {
                        printf("Error allocating memory");
                        exit(1);
                    }
                }
                // printf("FUNC\n");
                waitForInput();
                deck[idCount-1] = createDeck(deckCounter);
                system("clear");
                printf("Deck created with id: %d\n", deckCounter);
                waitForInput();
                break;
            case 2:
                printf("Available deck ID's: ");
                for (int i = 0; i < deckCounter; i++)
                {
                    printf("%d ", deck[i]->id);
                }
                printf("\n");
                printf("Enter deck ID: ");
                scanf("%d", &idToCheck);
                
                break;
            case 3:
                printf("Available deck ID's: ");
                for (int i = 0; i < deckCounter; i++)
                {
                    printf("%d ", deck[i]->id);
                }
                printf("\n");
                printf("Enter deck ID: ");
                scanf("%d", &idToCheck);
                print(deck[idToCheck-1]->head);
                waitForInput();
                break;
            case 4:
                printf("Available deck ID's: ");
                for (int i = 0; i < deckCounter; i++)
                {
                    printf("%d ", deck[i]->id);
                }
                printf("\n");
                printf("Enter deck ID: ");
                scanf("%d", &idToCheck);
                printf("Enter data to push: ");
                scanf("%d", &dataToPush);
                pushFront(deck[idToCheck-1], dataToPush);
                break;
            case 5:
                printf("Available deck ID's: ");
                for (int i = 0; i < deckCounter; i++)
                {
                    printf("%d ", deck[i]->id);
                }
                printf("\n");
                printf("Enter deck ID: ");
                scanf("%d", &idToCheck);
                printf("Enter data to push: ");
                scanf("%d", &dataToPush);
                pushBack(deck[idToCheck-1], dataToPush);
                break;
            case 6:
                printf("Available deck ID's: ");
                for (int i = 0; i < deckCounter; i++)
                {
                    printf("%d ", deck[i]->id);
                }
                printf("\n");
                printf("Enter deck ID: ");
                scanf("%d", &idToCheck);
                printf("Enter data to push: ");
                scanf("%d", &dataToPush);
                int num = popFront(deck[idToCheck-1]);
                printf("Popped number: %d", num);
                waitForInput();
                break;
            case 8:
                showMenu = false;
                break;
            default:
                printf("Invalid choice, try again");
        }
    }
}

int main()
{   
    // menu();
    int elements = 0;
    Deck *dq = createDeck(0);
    pushFront(dq, 5);
    pushFront(dq, 25);
    Deck **ptr_arr = NULL;
    ptr_arr = addToArray(ptr_arr, dq, &elements);
    ptr_arr = addToArray(ptr_arr, dq, &elements);
    printf("Elements: %d\n", elements);
    print(ptr_arr[0]->head);
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