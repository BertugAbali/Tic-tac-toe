#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "fileSettings.c"

int table[100][100][100][2];

struct Players
{
    int wins, losses, draws;
    char name[50];
    int gameIds[1000];
};

void designerOfGame(int size, int level);
int checkTheGame(int size, int level, int move[2], int order);
void gameConfigurations();
int *playerMove(char player[50], int counter, int level, int order);
int controlOfGame(char name1[50], char name2[50], int counter, int size, int order, int computer, int lastMove[2]);
int checkDimension(int size, int move[2], int order);
int controlOfDimensionalGame(char name1[50], char name2[50], int counter, int size, int computer);
int *computerMove(int counter, int level, int playerMove[2], int dimension, int size);

void gameConfigurations()
{

     // Resetting table

    for (int i = 0; i < 100; i++)
    {
        for (int a = 0; a < 100; a++)
        {
            for (int b = 0; b < 100; b++)
            {
                for (int c = 0; c < 2; c++)
                {
                    table[i][a][b][c] = 0;
                }
            }
        }
    }

    /*
     Main game configurations happening here. Variable table store all play data itself. First part is represent level, second is row, third is column, fourth is if
     0 index has value then that place has "o", if 1 index has value then that place has "x" but if either of them has no value then that place empty and that place's equivalent
     is represent order of place. Counter variable count play order. And the other ones self-explanatory.
    */

    int dimension = 0, counter = 1, size = 0, winner, info[2][1000], gameId = 0, computer = 0;
    char locGame[] = "Files\\Games\\game", locMoveLog[] = "Files\\Text Move Logs\\textMoveLog";

    /*
     info variable represent player 1 and player 2 datas as format of text file. In here ı am giving numbers to all indexes because ı dont want C to fill them all random numbers or
     something like that.
    */

    for (int a = 0; a < 2; a++)
    {
        for (int i = 0; i < 1000; i++)
        {
            info[a][a] = 0;
        }
    }

    struct Players player1, player2;

    printf("Play against computer:(1:Yes Any number: No)\n");
    scanf("%d", &computer);

    if (computer == 1)
    {
        strcat(player2.name, "computer");
        printf("Please enter player username:");
        scanf("%s", &player1.name);
    }
    else
    {
        computer=0;
        printf("Please enter first player username:");
        scanf("%s", &player1.name);
        printf("Please enter second player username:");
        scanf("%s", &player2.name);
    }

    while (size < 3)
    {
        printf("Please enter size of game:");
        scanf("%d", &size);

        if (size < 3)
        {
            printf("Size can't be smaller than 3!\n");
        }
    }

    printf("Do you want to game be 3 dimensional (Y:1,N:Any number):");
    scanf("%d", &dimension);

    playerCheck(player1.name);
    playerCheck(player2.name);

    // Getting all player data and putting them info. The first 3 index represent wins,losses and draws in a order. Other indexes represent game ids that player played in.

    for (int a = 0; a < 2; a++)
    {
        for (int i = 0; i < 1000; i++)
        {
            if (a)
            {
                info[a][i] = getPlayerInfo(player2.name, i);
            }
            else
            {
                info[a][i] = getPlayerInfo(player1.name, i);
            }
        }
    }

    player1.wins = info[0][0];
    player1.losses = info[0][1];
    player1.draws = info[0][2];

    player2.wins = info[1][0];
    player2.losses = info[1][1];
    player2.draws = info[1][2];

    if (dimension == 1)
    {
        winner = controlOfDimensionalGame(player1.name, player2.name, counter, size, computer);
    }
    else
    {
        int empty[2];
        designerOfGame(size, 0);
        winner = controlOfGame(player1.name, player2.name, counter, size, 0, computer, empty);
    }

    gameId = gameIdCounter();


    writeBinaryFile(table, gameId);
    char log[1000];
    writeTextFile(matrixToLog(log, table, size, player1.name, player2.name), gameId, locMoveLog);
    char data[1000] = "";

    switch (winner)
    {
    case 0:
        strcat(data, "Draw: ");
        strcat(data, player1.name);
        strcat(data, " ");
        strcat(data, player2.name);
        player1.draws++;
        player2.draws++;
        break;
    case 1:
        strcat(data, "Winner: ");
        strcat(data, player1.name);
        strcat(data, " Loser: ");
        strcat(data, player2.name);
        player1.wins++;
        player2.losses++;
        break;
    case 2:
        strcat(data, "Winner: ");
        strcat(data, player2.name);
        strcat(data, " Loser: ");
        strcat(data, player1.name);
        player2.wins++;
        player1.losses++;
        break;
    }

    // Game result writing to text file with its id.

    writeTextFile(data, gameId, locGame);

    info[0][0] = player1.wins;
    info[0][1] = player1.losses;
    info[0][2] = player1.draws;

    info[1][0] = player2.wins;
    info[1][1] = player2.losses;
    info[1][2] = player2.draws;


    // Putting this game id to each player's data. Which it will be the next empty index in info variable.

    for (int a = 0; a < 2; a++)
    {
        for (int i = 3; i < 1000; i++)
        {
            if (info[a][i] == 0)
            {
                info[a][i] = gameId;
                i = 999;
            }
        }
    }



    updatePlayer(player1.name, info[0]);
    updatePlayer(player2.name, info[1]);

   
}

int controlOfDimensionalGame(char name1[50], char name2[50], int counter, int size, int computer)
{
    int end = 0, selectedLevel, *move, order = 1, winner, *pcMove;
    char *player;

    // First printing empty table for player to see the table.

    for (int u = 0; u < size; u++)
    {
        designerOfGame(size, u);
        printf("-----------------------------------------------\n");
    }

    while (!end)
    {

        // in order variable, 1 for first player and 0 for second player.

        if (order == 1)
        {
            player = name1;
            winner = 1;
        }
        else
        {
            player = name2;
            winner = 2;
        }

        if (computer && !order)
        {
            // always computer will be second player.

            pcMove = computerMove(counter, selectedLevel, move, 1, size);
            selectedLevel = pcMove[2];
        }
        else
        {
            printf("Player %s please make your move. Please enter which level you play: ", player);
            scanf("%d", &selectedLevel);
            move = playerMove(player, counter, selectedLevel - 1, order);
        }
        counter++;
        for (int y = 0; y < size; y++)
        {
            designerOfGame(size, y);
            printf("-----------------------------------------------\n");
        }

        // selected level represent level defined by player but inside of the code level start from 0 to max so when we are using it we always extracting one.

        // In here we are checking just that level

        switch (checkTheGame(size, selectedLevel - 1, move, order))
        {
        case 1:
        case 0:
            printf("Win of %s", player);
            end = 1;
            return winner;
            break;

        case -1:
            // Tie happend that level but we must check other levels because this is dimensional play.
            int tie = 0;
            for (int m = 0; m < size; m++)
            {
                if (checkTheGame(size, m, move, order) == -1)
                {
                    tie++;
                }
                else
                {
                    m = size - 1;
                }
            }

            if (tie == size)
            {
                printf("No one win the game. Tie");
                end = 1;
                return 0;
            }
            break;
        }

        // In here we are checking all levels behavior.

        switch (checkDimension(size, move, order))
        {
        case 1:
        case 0:
            printf("Win of %s", player);
            end = 1;
            return winner;
            break;
        }

        if (order)
        {
            order = 0;
        }
        else
        {
            order = 1;
        }
    }
}

int controlOfGame(char name1[50], char name2[50], int counter, int size, int order, int computer, int lastMove[2])
{
    // This is for just one level game (non-dimensional game).

    int *move;
    if (order)
    {
        order = 0;

        if (computer)
        {
            move = computerMove(counter, 0, lastMove, 0, size);
            lastMove = lastMove;
        }
        else
        {
            move = playerMove(name2, counter, 0, order);
            lastMove = move;
        }
    }
    else
    {
        order = 1;
        move = playerMove(name1, counter, 0, order);
        lastMove = move;
    }
    counter++;
    designerOfGame(size, 0);

    switch (checkTheGame(size, 0, move, order))
    {
    case 1:
        printf("Win of %s", name1);
        return 1;
        break;
    case 0:
        printf("Win of %s", name2);
        return 2;
        break;
    case -1:
        printf("No one win the game. Tie");
        return 0;
        break;
    case 2:
        return controlOfGame(name1, name2, counter, size, order, computer, lastMove);
        break;
    }
}

void designerOfGame(int size, int level)
{

    // print the current game.

    int empty = 1, c = 0;

    for (int i = 0; i < size * 2 - 1; i++)
    {

        if (i % 2 == 0)
        {
            for (int a = 0; a < size; a++)
            {

                if (table[level][c][a][0])
                {
                    empty = 0;
                    printf("o");
                }
                else if (table[level][c][a][1])
                {
                    empty = 0;
                    printf("x");
                }
                else
                {
                    printf(" ");
                }

                if (a < size - 1)
                {
                    printf("|");
                }
            }
            c++;
        }
        else
        {
            for (int j = 0; j < size * 2 - 1; j++)
            {
                if (j % 2 == 0)
                {
                    printf("-");
                }
                else
                {
                    printf("+");
                }
            }
        }
        printf("\n");
    }
}

int *playerMove(char player[50], int counter, int level, int order)
{

    int row, column;
    static int move[2];

    printf("Player %s please make your move. Please enter which row you play: ", player);
    scanf("%d", &row);

    printf("Please enter which column you play: ");
    scanf("%d", &column);

    move[0] = row - 1;
    move[1] = column - 1;

    if (order == 1)
    {
        if (table[level][move[0]][move[1]][0] || table[level][move[0]][move[1]][1])
        {
            printf("You must make your move just blank places!\n");
            playerMove(player, counter, level, order);
        }
        else
        {
            table[level][move[0]][move[1]][1] = counter;
        }
    }
    else
    {
        if (table[level][move[0]][move[1]][0] || table[level][move[0]][move[1]][1])
        {
            printf("You must make your move just blank places!\n");
            playerMove(player, counter, level, order);
        }
        else
        {
            table[level][move[0]][move[1]][0] = counter;
        }
    }
    return move;
}

int *computerMove(int counter, int level, int playerMove[2], int dimension, int size)
{
    int place, symbol, amount[4];
    static int move[3];

    move[0] = playerMove[0];
    move[1] = playerMove[1];
    move[2] = level;

    for (int i = 0; i < 4; i++)
    {
        amount[i] = 1;
    }

    /*

     Computer moves will be related to player move. Computer will try to play nearest place to player move because of blocking player win. So in order to do that it will
     try nearest place to far places. Place variable for which place and sybol for which direction.

    */

    while (table[move[2]][move[0]][move[1]][0] || table[move[2]][move[0]][move[1]][1])
    {

        if (dimension)
        {
            place = rand() % 5;
        }
        else
        {
            place = rand() % 4;
        }

        symbol = rand() % 2;

        if (!symbol)
        {
            symbol = -1;
        }

        switch (place)
        {
        case 0:
            // just adding or extracting to row
            move[0] = move[0] + symbol * amount[0];
            amount[0]++;
            if (amount[0] == size)
            {
                amount[0] = 0;
            }
            break;
        case 1:
            // just adding or extracting to column
            move[1] = move[1] + symbol * amount[1];
            amount[1]++;
            if (amount[1] == size)
            {
                amount[1] = 0;
            }
            break;
        case 2:
            // adding or extracting to column and row for cross
            move[1] = move[1] + symbol * amount[2];
            move[0] = move[0] + symbol * amount[2];

            amount[2]++;
            if (amount[2] == size)
            {
                amount[2] = 0;
            }
            break;
        case 3:
            // adding column or row extracting other one for other cross
            move[1] = move[1] + symbol * amount[2];
            move[0] = move[0] + (-symbol) * amount[2];

            amount[3]++;
            if (amount[3] == size)
            {
                amount[3] = 0;
            }
            break;
        case 4:
            // adding or extracting to level
            move[2] = move[2] + symbol * amount[4];
            amount[4]++;
            if (amount[4] == size)
            {
                amount[4] = 0;
            }
            break;
        }

        // if the move exceed the play zone, it will changed to player last move.

        if (move[0] >= size || move[0] < 0 || move[1] >= size || move[1] < 0 || move[2] >= size || move[2] < 0)
        {
            move[0] = playerMove[0];
            move[1] = playerMove[1];
            move[2] = level;
        }
    }

    table[level][move[0]][move[1]][0] = counter;

    printf("Computer played.\n");

    return move;
}

int checkTheGame(int size, int level, int move[2], int order)
{

    int same = 0;

    // Checking Row
    // In here, i am just checking one row that made move because that row just needed.

    for (int d = 0; d < size; d++)
    {
        if (table[level][move[0]][d][order])
        {

            same = 1;
        }
        else
        {
            same = 0;
            d = size - 1;
        }
    }
    if (same)
    {
        return order;
    }

    // Checking Columns

    for (int r = 0; r < size; r++)
    {
        if (table[level][r][move[1]][order])
        {
            same = 1;
        }
        else
        {
            same = 0;
            r = size - 1;
        }
    }
    if (same)
    {
        return order;
    }

    // Checking left to right for cross.
    // It check if players move is in the cross. (For example: 0-0,1-1,2-2)

    if (move[0] == move[1])
    {
        for (int p = 0; p < size; p++)
        {
            if (table[level][p][p][order])
            {
                same = 1;
            }
            else
            {
                same = 0;
                p = size - 1;
            }
        }

        if (same)
        {
            return order;
        }
    }

    // Checking right to left cross
    // It check if players move is in the cross. (Always column and rows sum equals to size minus 1)

    if (move[0] + move[1] == size - 1)
    {
        for (int g = 0; g < size; g++)
        {
            if (table[level][g][(size - 1) - g][order])
            {
                same = 1;
            }
            else
            {
                same = 0;
                g = size - 1;
            }
        }

        if (same)
        {
            return order;
        }
    }

    // checking tie

    int tieCheck = 1;

    for (int f = 0; f < size; f++)
    {
        for (int i = 0; i < size; i++)
        {
            if (!table[level][f][i][0] && !table[level][f][i][1])
            {
                f = size * size - 1;
                tieCheck = 0;
            }
        }
    }

    if (tieCheck)
    {
        return -1;
    }

    return 2;
}

int checkDimension(int size, int move[2], int order)
{

    int same = 0;

    // Checking Dimension Columns

    for (int l = 0; l < size; l++)
    {
        if (table[l][move[0]][move[1]][order])
        {
            same = 1;
        }
        else
        {
            same = 0;
            l = size - 1;
        }
    }
    if (same)
    {
        return order;
    }

    // Checking Dimension Corner to corner
    // four diffirent corner to corner check

    if (move[0] == move[1])
    {

        for (int i = 0; i < 2; i++)
        {

            int amount, start;

            if (i)
            {
                amount = 1;
                start = 0;
            }
            else
            {
                amount = -1;
                start = size - 1;
            }

            for (int a = 0; a < size; a++)
            {
                if (table[a][start + a * amount][start + a * amount][order])
                {
                    same = 1;
                }
                else
                {
                    same = 0;
                    a = size - 1;
                }
            }
            if (same)
            {
                return order;
            }
        }
    }

    if (move[0] + move[1] == size - 1)
    {
        for (int i = 0; i < 4; i++)
        {

            int amount, startC, startR;

            if (i)
            {
                amount = 1;
                startR = 0;
                startC = size - 1;
            }
            else
            {
                amount = -1;
                startR = size - 1;
                startC = 0;
            }

            for (int a = 0; a < size; a++)
            {
                if (table[a][startR + a * amount][startC + a * -amount][order])
                {
                    same = 1;
                }
                else
                {
                    same = 0;
                    a = size - 1;
                }
            }
            if (same)
            {
                return order;
            }
        }
    }

    // Checking horizontal corner to corner

    for (int a = 0; a < size; a++)
    {
        if (table[a][move[0]][a][order])
        {
            same = 1;
        }
        else
        {
            same = 0;
            a = size - 1;
        }
    }
    if (same)
    {
        return order;
    }

    // Checking vertical corner to corner

    for (int a = 0; a < size; a++)
    {
        if (table[a][a][move[1]][order])
        {
            same = 1;
        }
        else
        {
            same = 0;
            a = size - 1;
        }
    }
    if (same)
    {
        return order;
    }

    return 2;
}
