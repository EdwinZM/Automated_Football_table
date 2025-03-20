#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <Arduino.h>

#define IMG_SIZE 128

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define VISIBLE_MENU_ITEMS 3

#define MENU_POSITIONS 8

void screenInitialize();
void moveMenuUp();
void moveMenuDown();
void drawMenu();
int getCurrentMenuItem();
void printScreenDebugInfo();
void printText(char* text);

typedef struct menu_item {
    int id;
    char* text;
    int menu_position;
    int circle_location;
    struct menu_item * next;
} menu_item;

#endif
