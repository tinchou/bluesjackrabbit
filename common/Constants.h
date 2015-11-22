#ifndef BLUESJACKRABBIT_COMMON_CONSTANTS_H
#define BLUESJACKRABBIT_COMMON_CONSTANTS_H


#define MAX_CHAR 256

#define OPTION_LENGTH 1
#define NOT 0x00
#define OK 0xFF

// GAME PROTOCOL CONSTANTS
#define LEFT 0x1
#define RIGHT 0x2
#define DOWN 0x3
#define UP 0x4
#define JUMP 0x5
#define SHOOT 0x6

#define CANT_BYTES 1

// MENU PROTOCOL CONSTANTS
#define NEW_GAME 0x20
#define JOIN_GAME 0x21
#define LIST_GAMES 0x22
#define LIST_MAPS 0x23
#define GAME_INIT 0x24
#define GAME_OBJECT 0x25

#define TRUE_PROTOCOL 0x30
#define FALSE_PROTOCOL 0x31

#define MAP_ID_LENGTH 1


#endif  // BLUESJACKRABBIT_COMMON_CONSTANTS_H
