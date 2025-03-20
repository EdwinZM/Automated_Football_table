#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <ModbusMaster.h>

#define MOTOR_BAUDRATE 115200
#define PULSES_PER_REV 2000
#define NUM_PLAYERS 4
#define DEFAULT_ACCEL 400
#define DEFAULT_SPEED 700

enum Player {
    GOALKEEPER,
    DEFENDERS,
    MIDFIELD,
    STRIKER
};

enum Motor_type {
    RADIAL,
    SLIDER
};

void motorLibInitialize();
void motorInitialize(int ID);
void preTransmission();
void postTransmission();
uint8_t setPosition(int ID, float position);
uint8_t setSpeed(int ID);
uint8_t setHome(int ID);
uint8_t setAcceleration(int ID);
uint8_t doKick(int ID);
uint8_t moveToPosition(int ID);
uint16_t getPosition(int ID);

typedef struct MOTOR {
    int id;
    Player player;
    Motor_type motor_type;
    float pos_cur = -1;   // Can't move before homing -> check if pos_cur == -1
} MOTOR;

#endif
