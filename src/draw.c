#include <stdio.h>
#include "draw.h"

void drawBox(int width, int height){
    for (int i = 0; i<width +2; i++)printf("-");
    printf("\n");
    for (int i = 0; i <height; i++){
        printf("|");
        for (int j = 0; j<width; j++){
            printf(" ");
        }printf("|\n");
    }for (int i = 0; i<width + 2; i++)printf("-");
    printf("\n");
}