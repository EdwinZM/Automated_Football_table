#include <Arduino.h>
#include <ezButton.h>
#include <iostream>
#include <stdio.h>

#include "config.h"
#include "screen.h"
#include "motor.h"
//hello
ezButton downButton(3);
ezButton selectButton(4);
ezButton upButton(5);

int selected_motor = 1;

float max_bound[4] = {0.8, 0.8, 0.8, 0.8};
float min_bound[4] = {0.1, 0.1, 0.1, 0.1};


void setup()
{
    DEBUG_SERIAL.begin(115200);
    screenInitialize();

    delay(100);

    motorLibInitialize();

    selectButton.setDebounceTime(50);
    downButton.setDebounceTime(50);
    upButton.setDebounceTime(50);

    drawMenu();
}

void loop()
{
    selectButton.loop();
    downButton.loop();
    upButton.loop();
    int menu_item;
    if (upButton.isPressed() || downButton.isPressed() || selectButton.isPressed())
    {
        printScreenDebugInfo();
    }
    if (upButton.isPressed())
    {
        moveMenuUp();
        while (getCurrentMenuItem() == -1)
        {
            moveMenuUp();
        }
    }
    else if (downButton.isPressed())
    {
        moveMenuDown();
        while (getCurrentMenuItem() == -1)
        {
            moveMenuDown();
        }
    }
    else if (selectButton.isPressed())
    {
        menu_item = getCurrentMenuItem();
        if (menu_item == 0)
        {
            // Reinitialize everything
            screenInitialize();
            DEBUG_SERIAL.println("Setting home position.");
            printText("Initializing motors.");

            for (int i = 1; i < 9; i++)
            {
                DEBUG_SERIAL.print("Initializing motor ");
                DEBUG_SERIAL.print(i);
                DEBUG_SERIAL.println(".");

                motorInitialize(i);
            }

            printText("Done initializing motors.");
        }
        else if (menu_item == 1)
        {
            // Player
            selected_motor += 1;
            if (selected_motor >= 5)
                selected_motor = 1;
            DEBUG_SERIAL.print("Setting selected player to ");
            DEBUG_SERIAL.print(selected_motor);
            DEBUG_SERIAL.println(".");
        }
        else if (menu_item == 2)
        {
            // Kick
            DEBUG_SERIAL.println("Kicking");
            doKick(selected_motor + 4);
        }
        else if (menu_item == 3)
        {
            // Slide
            int pos_val = analogRead(POS_PIN);
            float position = (float)pos_val / 1023.0;
            // if (position > max_slider_length[selected_motor - 1]) {
            //     position = max_slider_length[selected_motor - 1];
            // } else if (position < min_slider_length[selected_motor - 1]) {
            //     position = min_slider_length[selected_motor - 1];
            // }
            
            std::cout << position << std::endl;

            if (position > max_bound[selected_motor - 1]) {
                position = max_bound[selected_motor - 1];
            } else if (position < min_bound[selected_motor - 1]) {
                position = min_bound[selected_motor - 1];
            }
            DEBUG_SERIAL.print("Moving to position at ");
            DEBUG_SERIAL.print(position * 100);
            DEBUG_SERIAL.println("\% of available moving area.");
            setPosition(selected_motor, position); 
            moveToPosition(selected_motor);
        }
    }
}
