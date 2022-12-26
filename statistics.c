#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void searchByName();
int configurations();
void allGames();

int configurations()
{

    int op;

    printf("Enter a number for operation: \n 1: Search by name \n 2: All games \n 3: Main Menu \n Any Number: Exit\n");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
        searchByName();
        printf("----------------------------------------------\n");
        return 1;
        break;
    case 2:
        allGames();
        printf("----------------------------------------------\n");

        return 1;
        break;
    case 3:
        return 1;
        printf("----------------------------------------------\n");

        break;
    }
    return 0;
}

void searchByName()
{
    char name[50];
    char moveLog[1000], data[1000];
    int a = 0, i = 3,count=0;

    // Checking if player exist. 

    while (!a)
    {
        printf("Please enter username for search:\n");
        scanf("%s", name);

        FILE *file;

        char loc[] = "Files\\Players\\player_";

        strcat(loc, name);
        strcat(loc, ".txt");

        file = fopen(loc, "r");
        if (file == NULL)
        {
            printf("Sorry but there is no such user. Please try diffirent username.\n");
        }
        else
        {
            a = 1;
        }
        fclose(file);
    }

    printf("Player: %s Wins: %d Losses: %d Draws: %d\n", name, getPlayerInfo(name, 0), getPlayerInfo(name, 1), getPlayerInfo(name, 2));
    printf("----------------------------------------------\n");
    printf("*****History of all games*****\n");
    while (getPlayerInfo(name,i))
    {
        printf("%d: %s  %s\n", i - 2, readTextFile(data, getPlayerInfo(name,i), 0), readTextFile(moveLog, getPlayerInfo(name,i), 1));
        i++;
    }
}

void allGames()
{
    char moveLog[1000], data[1000];
    int i = 1;

    printf("*****History of all games*****\n");

    while (readTextFile(data, i, 0) != NULL)
    {
        printf("%d: %s  %s\n", i, readTextFile(data, i, 0), readTextFile(moveLog, i, 1));
        i++;
    }
}