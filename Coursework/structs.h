#ifndef STRUCTS_H
#define STRUCTS_H

#include "stdbool.h"

enum selectedBot
{
    EASY = 0,
    HARD = 1
};

enum action
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3,
    NONE = 4
};

typedef struct player
{
    unsigned short** board;
    int score;
    bool isWin;
    bool isLose;
} player_t;

typedef struct gameMode
{
    int boardSize;
    int finalNumber;
    enum selectedBot selectedBot;
} gamemode_t;

#endif