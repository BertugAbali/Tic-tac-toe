#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

char *matrixToLog(char log[1000], int logTable[100][100][100][2], int size, char name1[50], char name2[50]);
void logToMatrix(char log[1000], int logTable[100][100][100][2], char name1[50], char name2[50]);
void writeBinaryFile(int logTable[100][100][100][2], int gameId);
void readBinaryFile(int logTable[100][100][100][2], int gameId);
void writeTextFile(char log[1000], int gameId, char loc[]);
char *readTextFile(char log[1000], int gameId, int type);
void playerCheck(char name[50]);
void updatePlayer(char name[50], int info[1000]);
void createPlayer(char name[50]);
int gameIdCounter();
int getPlayerInfo(char name[50], int index);

void writeBinaryFile(int logTable[100][100][100][2], int gameId)
{
  FILE *file;

  char r[10];

  sprintf(r, "%d", gameId);

  char loc[] = "Files\\Binary Move Logs\\binaryMoveLog";

  strcat(loc, r);
  strcat(loc, ".bin");

  file = fopen(loc, "wb");

  if (file != NULL)
  {

    for (int i = 0; i < 100; i++)
    {
      for (int a = 0; a < 100; a++)
      {
        for (int b = 0; b < 100; b++)
        {
          fwrite(logTable[i][a][b], sizeof(int), 1, file);
        }
      }
    }
  }

  fclose(file);
}

void readBinaryFile(int logTable[100][100][100][2], int gameId)
{
  FILE *file;

  char r[10];

  sprintf(r, "%d", gameId);

  char loc[] = "Files\\Binary Move Logs\\binaryMoveLog";

  strcat(loc, r);
  strcat(loc, ".bin");

  file = fopen(loc, "rb");
  if (file != NULL)
  {

    for (int i = 0; i < 100; i++)
    {
      for (int a = 0; a < 100; a++)
      {
        for (int b = 0; b < 100; b++)
        {
          fread(logTable[i][a][b], sizeof(int), 1, file);
        }
      }
    }
  }

  fclose(file);
}

void writeTextFile(char log[1000], int gameId, char loc[])
{

  FILE *file;

  char r[10];

  sprintf(r, "%d", gameId);

  strcat(loc, r);
  strcat(loc, ".txt");

  file = fopen(loc, "w");

  if (file != NULL)
  {
    fwrite(log, sizeof(char),1000, file);
  }

  fclose(file);
}

char *readTextFile(char log[1000], int gameId, int type)
{

  char loc[1000] = "";

  if (type)
  {
    strcat(loc, "Files\\Text Move Logs\\textMoveLog");
  }
  else
  {
    strcat(loc, "Files\\Games\\game");
  }

  FILE *file;

  char r[10];

  sprintf(r, "%d", gameId);

  strcat(loc, r);
  strcat(loc, ".txt");

  file = fopen(loc, "r");
  if (file != NULL)
  {

    fread(log, sizeof(char) * 1000, 1, file);
    fclose(file);
    return log;
  }
  else
  {
    fclose(file);
    return NULL;
  }
}

int gameIdCounter()
{

  // Keeps track of game ids.

  FILE *file;

  int idCounter = 1;

  char loc[] = "Files\\gameIdCounter.bin";

  file = fopen(loc, "rb");

  if (file != NULL)
  {
    fread(&idCounter, sizeof(int), 1, file);
  }

  fclose(file);

  idCounter++;

  file = fopen(loc, "wb");

  if (file != NULL)
  {
    fwrite(&idCounter, sizeof(int), 1, file);
  }

  fclose(file);

  return idCounter - 1;
}

char *matrixToLog(char log[1000], int logTable[100][100][100][2], int size, char name1[50], char name2[50])
{

  // symbol-level-column-row (order of numbers that represents move place)

  char moves[1000];
  char r[10];
  int loc = 0;

  strcat(log, name1);
  strcat(log, "(x) ");
  strcat(log, name2);
  strcat(log, "(o) ");

  for (int d = 0; d < size; d++)
  {
    for (int v = 0; v < size; v++)
    {
      for (int i = 0; i < size; i++)
      {
        if (logTable[d][v][i][0])
        {
          loc = (logTable[d][v][i][0] - 1) * 4;
          moves[loc] = 'o';
          sprintf(r, "%d", d + 1);
          moves[loc + 1] = r[0];
          sprintf(r, "%d", i + 1);
          moves[loc + 2] = r[0];
          sprintf(r, "%d", v + 1);
          moves[loc + 3] = r[0];
        }
        else if (logTable[d][v][i][1])
        {
          loc = (logTable[d][v][i][1] - 1) * 4;
          moves[loc] = 'x';
          sprintf(r, "%d", d + 1);
          moves[loc + 1] = r[0];
          sprintf(r, "%d", i + 1);
          moves[loc + 2] = r[0];
          sprintf(r, "%d", v + 1);
          moves[loc + 3] = r[0];
        }
      }
    }
  }

  strcat(log, moves);

  return log;
}

void logToMatrix(char log[1000], int logTable[100][100][100][2], char name1[50], char name2[50])
{
  int start = 0, orderOfMoves = 1, level, row, col;

  start = strlen(name1) + 4 + strlen(name2) + 4;

  for (int i = 0; log[i * 4 + start] != '\0'; i++)
  {
    // with substracting zero char becomes intiger
    level = (log[start + (i * 4) + 1] - '0') - 1;
    col = (log[start + (i * 4) + 2] - '0') - 1;
    row = (log[start + (i * 4) + 3] - '0') - 1;
    if (log[start + (i * 4)] == 'x')
    {
      logTable[level][row][col][1] = orderOfMoves;
    }
    if (log[start + (i * 4)] == 'o')
    {
      logTable[level][row][col][0] = orderOfMoves;
    }
    orderOfMoves++;
  }
}

int getPlayerInfo(char name[50], int index)
{

  int playerInfo[1000];

  FILE *file;

  char loc[] = "Files\\Players\\player_";

  strcat(loc, name);
  strcat(loc, ".txt");

  file = fopen(loc, "r");
  if (file != NULL)
  {

    fread(playerInfo, sizeof(int), 1000, file);
  }

  fclose(file);
  return playerInfo[index];
}

void playerCheck(char name[50])
{

  // This function checks that if there is a player that has the username that given. If there is no player has it then it will create empty player.

  FILE *file;

  int playerInfo[1000];

  char loc[] = "Files\\Players\\player_";

  strcat(loc, name);
  strcat(loc, ".txt");

  file = fopen(loc, "r");
  if (file == NULL)
  {
    createPlayer(name);
  }
  else
  {
    fread(playerInfo, sizeof(int), 1000, file);
  }

  fclose(file);
}

void updatePlayer(char name[50], int info[1000])
{

  FILE *file;

  char loc[] = "Files\\Players\\player_";

  strcat(loc, name);
  strcat(loc, ".txt");

  file = fopen(loc, "w");

  fwrite(info, sizeof(int), 1000, file);

  fclose(file);
}

void createPlayer(char name[50])
{

  int info[1000];

  for (int i = 0; i < 1000; i++)
  {
    info[i] = 0;
  }

  FILE *file;

  char loc[] = "Files\\Players\\player_";

  strcat(loc, name);
  strcat(loc, ".txt");

  file = fopen(loc, "w");

  fwrite(info, sizeof(int), 1000, file);

  fclose(file);
}
