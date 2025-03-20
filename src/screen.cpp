#include "screen.h"
#include "config.h"

#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

menu_item* head         = NULL;
menu_item* second_item  = NULL;
menu_item* third_item   = NULL;
menu_item* fourth_item  = NULL;


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
GFXcanvas16 canvas(128, 128);

int pos_list[MENU_POSITIONS];

char menu_items[5][10] = {"Init", "Player", "Kick", "Slide"};

int x_coords[] = {
    175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 175, 174, 174, 174, 174, 173, 173, 173, 173, 172, 172, 172, 171, 171, 170, 170, 170, 169, 169, 168, 168, 167, 167, 166, 166, 165, 165, 164, 164, 163, 163, 162, 161, 161, 160, 159, 159, 158, 158, 157, 156, 156, 155, 154, 153, 153, 152, 151, 151, 150, 149, 148, 148, 147, 146, 145, 144, 144, 143, 142, 141, 140, 140, 139, 138, 137, 136, 136, 135, 134, 133, 132, 131, 131, 130, 129, 128, 127, 126, 125, 125, 124, 123, 122, 121, 120, 120, 119, 118, 117, 116, 116, 115, 114, 113, 112, 112, 111, 110, 109, 108, 108, 107, 106, 105, 105, 104, 103, 103, 102, 101, 100, 100, 99, 98, 98, 97, 97, 96, 95, 95, 94, 93, 93, 92, 92, 91, 91, 90, 90, 89, 89, 88, 88, 87, 87, 86, 86, 86, 85, 85, 84, 84, 84, 83, 83, 83, 83, 82, 82, 82, 82, 81, 81, 81, 81, 81, 81, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 81, 81, 81, 81, 81, 81, 82, 82, 82, 82, 83, 83, 83, 83, 84, 84, 84, 85, 85, 86, 86, 86, 87, 87, 88, 88, 89, 89, 90, 90, 91, 91, 92, 92, 93, 93, 94, 95, 95, 96, 97, 97, 98, 98, 99, 100, 100, 101, 102, 103, 103, 104, 105, 105, 106, 107, 108, 108, 109, 110, 111, 112, 112, 113, 114, 115, 116, 116, 117, 118, 119, 120, 120, 121, 122, 123, 124, 125, 125, 126, 127, 128, 129, 130, 131, 131, 132, 133, 134, 135, 136, 136, 137, 138, 139, 140, 140, 141, 142, 143, 144, 144, 145, 146, 147, 148, 148, 149, 150, 151, 151, 152, 153, 153, 154, 155, 156, 156, 157, 158, 158, 159, 159, 160, 161, 161, 162, 163, 163, 164, 164, 165, 165, 166, 166, 167, 167, 168, 168, 169, 169, 170, 170, 170, 171, 171, 172, 172, 172, 173, 173, 173, 173, 174, 174, 174, 174, 175, 175, 175, 175, 175, 175, 176, 176, 176, 176, 176, 176, 176, 176, 176
};

int y_coords[] = {
    65, 66, 67, 67, 68, 69, 70, 71, 72, 72, 73, 74, 75, 76, 76, 77, 78, 79, 80, 80, 81, 82, 83, 84, 84, 85, 86, 87, 87, 88, 89, 89, 90, 91, 92, 92, 93, 94, 94, 95, 95, 96, 97, 97, 98, 99, 99, 100, 100, 101, 101, 102, 102, 103, 103, 104, 104, 105, 105, 106, 106, 106, 107, 107, 108, 108, 108, 109, 109, 109, 109, 110, 110, 110, 110, 111, 111, 111, 111, 111, 111, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 112, 111, 111, 111, 111, 111, 111, 110, 110, 110, 110, 109, 109, 109, 109, 108, 108, 108, 107, 107, 106, 106, 106, 105, 105, 104, 104, 103, 103, 102, 102, 101, 101, 100, 100, 99, 99, 98, 97, 97, 96, 95, 95, 94, 94, 93, 92, 92, 91, 90, 89, 89, 88, 87, 87, 86, 85, 84, 84, 83, 82, 81, 80, 80, 79, 78, 77, 76, 76, 75, 74, 73, 72, 72, 71, 70, 69, 68, 67, 67, 66, 65, 64, 63, 62, 61, 61, 60, 59, 58, 57, 56, 56, 55, 54, 53, 52, 52, 51, 50, 49, 48, 48, 47, 46, 45, 44, 44, 43, 42, 41, 41, 40, 39, 39, 38, 37, 36, 36, 35, 34, 34, 33, 33, 32, 31, 31, 30, 29, 29, 28, 28, 27, 27, 26, 26, 25, 25, 24, 24, 23, 23, 22, 22, 22, 21, 21, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 18, 17, 17, 17, 17, 17, 17, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 31, 31, 32, 33, 33, 34, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 41, 42, 43, 44, 44, 45, 46, 47, 48, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 56, 57, 58, 59, 60, 61, 61, 62, 63, 63
};

int num_coords = sizeof(x_coords)/sizeof(x_coords[0]);

int text_size = 2;

int letter_width = 5*text_size;   // Pixels
int letter_height = 7*text_size;  // Pixels

void screenInitialize() {
    tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
    canvas.setTextWrap(false);
    canvas.setTextSize(2);
    canvas.setRotation(3);

    // Positions for 8 menu items, where pos0 is center
    pos_list[0] = 180;
    pos_list[1] = 135;
    pos_list[2] = 90;
    pos_list[3] = 45;
    pos_list[4] = 0;
    pos_list[5] = 315;
    pos_list[6] = 270;
    pos_list[7] = 225;

    // Set up menu
    head        = (menu_item *) malloc(sizeof(menu_item));
    second_item = (menu_item *) malloc(sizeof(menu_item));
    third_item  = (menu_item *) malloc(sizeof(menu_item));
    fourth_item = (menu_item *) malloc(sizeof(menu_item));

    // Menu items
    head->id = 0;
    head->text = (char *) malloc(sizeof(menu_items[0]) + 1);
    strcpy(head->text, menu_items[0]);
    head->menu_position = 0;
    head->circle_location = pos_list[head->menu_position];
    head->next = second_item;

    second_item->id = 1;
    second_item->text = (char *) malloc(sizeof(menu_items[1]) + 1);
    strcpy(second_item->text, menu_items[1]);
    second_item->menu_position = 1;
    second_item->circle_location = pos_list[second_item->menu_position];
    second_item->next = third_item;

    third_item->id = 2;
    third_item->text = (char *) malloc(sizeof(menu_items[2]) + 1);
    strcpy(third_item->text, menu_items[2]);
    third_item->menu_position = 2;
    third_item->circle_location = pos_list[third_item->menu_position];
    third_item->next = fourth_item;

    fourth_item->id = 3;
    fourth_item->text = (char *) malloc(sizeof(menu_items[3]) + 1);
    strcpy(fourth_item->text, menu_items[3]);
    fourth_item->menu_position = 3;
    fourth_item->circle_location = pos_list[fourth_item->menu_position];
    fourth_item->next = head;
    
}

void slideMenuUp() {
    menu_item * current = head;
    for (int i = 0; i < 44; i = i + 2) {
        do {
            current->circle_location = (current->circle_location+2)%360;

            current = current->next;
        } while (current != head);

        delay(1);
        drawMenu();
    }
}

void moveMenuUp() {
    menu_item * current = head;
    slideMenuUp();

    do {
        current->menu_position = (current->menu_position - 1 + MENU_POSITIONS)%MENU_POSITIONS;
        current->circle_location = pos_list[current->menu_position];

        current = current->next;
    } while (current != head);

    drawMenu();
}

void slideMenuDown() {
    menu_item * current = head;
    for (int i = 0; i < 45; i = i + 2) {
        do {
            current->circle_location = (current->circle_location-2+360)%360;

            current = current->next;
        } while (current != head);

        delay(1);
        drawMenu();
    }
}

void moveMenuDown() {
    menu_item * current = head;
    slideMenuDown();

    do {
        current->menu_position = (current->menu_position + 1)%MENU_POSITIONS;
        current->circle_location = pos_list[current->menu_position];

        current = current->next;
    } while (current != head);

    drawMenu();
}

void drawMenu() {
    menu_item * current = head;
    int x, y, center_x, center_y, text_width;

    // Clear screen
    canvas.fillScreen(ST77XX_BLACK);

    // Draw main circle
    canvas.drawCircle(128, 64, 48, ST77XX_WHITE);
    canvas.fillCircle(128, 64, 46, ST77XX_BLACK);

    do {
        // Only draw circles outside of frame
        int num_letters = sizeof(current->text)/sizeof(current->text[0]);

        center_x = x_coords[current->circle_location];
        text_width = letter_width*num_letters;
        x = center_x - text_width/2;

        center_y = y_coords[current->circle_location];
        y = center_y - letter_height/2;

        if ((center_x < IMG_SIZE) or (center_y < IMG_SIZE)) {
            if (current->menu_position == 0) {
                canvas.fillRoundRect(x-4, y-4, letter_width*6 + 6, letter_height+6, 6, ST7735_WHITE);
            } else {
                canvas.fillRoundRect(x-4, y-4, letter_width*6 + 6, letter_height+6, 6, 0x9CD2);
            }

            canvas.fillRoundRect(x-2, y-2, letter_width*6 + 4, letter_height+4, 4, ST7735_BLACK);
            canvas.setCursor(x, y);
            if (current->menu_position == 0) {
                canvas.setTextColor(ST7735_WHITE);
            } else {
                canvas.setTextColor(0x9CD2);
            }

            canvas.print(current->text);
        }

        current = current->next;
    } while (current != head);


    tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
}

int getCurrentMenuItem() {
    menu_item * current = head;
    int select_slot = 0;

    do {
        if (current->menu_position == select_slot) {
            return current->id;
        }

        current = current->next;
    } while (current != head);

    // If no item is selected, return -1
    return -1;
}

void printText(char* text) {
    canvas.setCursor(0, 0);
    canvas.setTextWrap(true);
    canvas.print(text);

    tft.drawRGBBitmap(0, 0, canvas.getBuffer(), canvas.width(), canvas.height());
    canvas.setTextWrap(false);
}

void printScreenDebugInfo() {
    menu_item * current = head;
    do {
        DEBUG_SERIAL.print(current->id);
        DEBUG_SERIAL.print(": ");
        DEBUG_SERIAL.print(current->menu_position);
        DEBUG_SERIAL.print(" - ");
        DEBUG_SERIAL.print(current->circle_location);
        DEBUG_SERIAL.print("\t\t");

        current = current->next;
    } while (current != head);

    DEBUG_SERIAL.println("");
}
