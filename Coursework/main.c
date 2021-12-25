#include "2048.h"

int main() {
    int choice;
menu:
    printf("0)Exit program\n"
        "1)Play with bot\n"
        "2)Watch the game of bots\n\n"
        "Your choice:");
    scanf("%d", &choice);
    switch (choice)
    {
    case 0:
        return 0;
    case 1:
        play2048withBot();
        return 0;
    case 2:
        watchBotsGame();
        return 0;
    default:
        printf("Wrong number, try again\n");
        goto menu;
    }
}