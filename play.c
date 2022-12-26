#include <stdio.h>
#include "game.c"
#include "statistics.c"


int main()
{

    int op;

    printf("\nEnter a number for operation; \n 1: Play tic-tac-toe \n 2: Statistics \n Any Number: Exit\n");
    scanf("%d", &op);

    switch (op)
    {
    case 1:
         gameConfigurations();
            return main();
        break;
    case 2:
        if(configurations()){
            return main();
        }
        break;
    }

    return 0;
}