#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "binary.h"
#include "game.h"
#include "draw.h"
#include "input.h"
#include <unistd.h>

#define SCREEN_WIDTH 40
#define SCREEN_HEIGHT 20

static int bits[32];
static int bitCount=0;
static int collectedBits[32];
static int collectedCount=0;
static int playerpos;
static int score;
static int gameOver;
static int dropIndex=0;
static int dropPos=0;
static int currentBitX;

void gameInit(int number){
    bitCount=binaryConvert(number,bits,32);
    playerpos=SCREEN_WIDTH/2;
    collectedCount=0;
    score=0;
    gameOver=0;
    dropIndex=0;
    dropPos=0;
    currentBitX=SCREEN_WIDTH/2;
}

int gameIsOver(){
    return gameOver;
}

int gameGetScore(){
    return score;
}

void gameUpdate(){
    int input = inputGet();
    if (input == -1){
        //no key was pressed
    }else if ((input == 'a' || input == 'A') && playerpos > 0){
        playerpos--;
    }else if ((input == 'd' || input == 'D') && playerpos < SCREEN_WIDTH-1){
        playerpos++;
    }else if (input == 'q' || input == 'Q'){
        gameOver = 1;
        return;
    }
    
    // Simulate bits falling and collecting
    if (dropIndex < bitCount){
        dropPos++;
        
        if (dropPos >= SCREEN_HEIGHT-1){
            // Bit reached bottom - check if player caught it
            if (playerpos == currentBitX || playerpos+1 == currentBitX){
                collectedBits[collectedCount++] = bits[dropIndex];
                score += 10;
            }
            
            // Move to next bit
            dropIndex++;
            dropPos = 0;
            if (dropIndex < bitCount) {
                // Set position for next bit (spread them across screen)
                currentBitX = (dropIndex * 7) % (SCREEN_WIDTH-2) + 1;
            }
        }
    } else {
        gameOver = 1;
    }
    
    // Add small delay for smooth animation
    usleep(150000); // 150ms delay
}

void gameDraw(){
    system("clear");
    
    // Draw game area
    printf("\n=== BINARY QUEST ===\n");
    printf("Score: %d | Bits Collected: %d/%d | Controls: A/D to move, Q to quit\n\n", score, collectedCount, bitCount);
    
    // Draw the falling bit and game area
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            if (y == dropPos && x == currentBitX && dropIndex < bitCount) {
                // Draw falling bit
                printf("%d", bits[dropIndex]);
            } else if (y == SCREEN_HEIGHT-1 && x == playerpos) {
                // Draw player
                printf("[");
            } else if (y == SCREEN_HEIGHT-1 && x == playerpos+1) {
                printf("]");
            } else {
                printf(" ");
            }
        }
        printf("|\n");
    }
    
    // Draw bottom border
    for (int i = 0; i < SCREEN_WIDTH + 2; i++) printf("-");
    printf("\n");
    
    // Show collected bits
    printf("Collected bits: ");
    for (int i = 0; i < collectedCount; i++) {
        printf("%d", collectedBits[i]);
    }
    printf("\n");
}
