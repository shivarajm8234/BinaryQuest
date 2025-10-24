#include <stdio.h>
#include "game.h"
#include "input.h"

int main(){
    int startNumber;
    printf("Enter a Number: ");
    scanf("%d",&startNumber);

    gameInit(startNumber);
    while (!gameIsOver()){
        gameUpdate();
        gameDraw();
    }
    inputCleanup();
    printf("Game over! Your Score: %d\n",gameGetScore());
    return 0;
}