#define _CRT_SECURE_NO_WARNINGS  
#include "2048.h"
#include <stdio.h>

int main() {
    int choice;
menu:
    printf("Game 2048:\n"
        "0)Exit program\n"
        "1)Play with bot\n"
        "2)Watch the game of bots\n"
        "3)Watch the game of easy bot\n"
        "4)Watch the game of hard bot\n\n"
        "Your choice:");
    scanf("%d", &choice);
    switch (choice)
    {
    case 0:
        break;
    case 1:
        play2048withBot();
        break;
    case 2:
        watchBotsGame();
        break;
    case 3:
        watchGameBotEasy();
        break;
    case 4:
        watchGameBotHard();
        break;
    default:
        printf("Wrong number, try again\n\n");
        goto menu;
    }
    return 0;
}