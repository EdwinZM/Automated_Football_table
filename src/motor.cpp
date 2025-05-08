#include "config.h"
#include "motor.h"

#include <ModbusMaster.h>
#include <Arduino.h>

ModbusMaster node[9]; // Initialize 9 because driver ID start at 1

//float max_position[8] = {4.5, 17.4, 3, 10, 0.5, 0.5, 0.5, 0.5};

void motorLibInitialize() {
    pinMode(MAX485_RE_NEG, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);
    // Init in receive mode
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);

    MOTOR_SERIAL.begin(MOTOR_BAUDRATE);
}

void motorInitialize(int ID) {
    // Modbus slave ID
    node[ID].begin(ID, MOTOR_SERIAL);

    // Callbacks allow us to configure the RS485 transceiver correctly
    node[ID].preTransmission(preTransmission);
    node[ID].postTransmission(postTransmission);

    // Set to position mode
    node[ID].writeSingleRegister(0x6200, 0x0001); 

    delay(10);

    setHome(ID);
}

void preTransmission(){
    digitalWrite(MAX485_RE_NEG, 1);
    digitalWrite(MAX485_DE, 1);
}

void postTransmission(){
    digitalWrite(MAX485_RE_NEG, 0);
    digitalWrite(MAX485_DE, 0);
}

uint8_t setHome(int ID) {
    uint8_t result = node[ID].writeSingleRegister(0x6002, 0x0021);
    delay(10);

    return result;
} 

// Set speed to default value
uint8_t setSpeed(int ID) {
    uint8_t result = 0;
    node[ID].writeSingleRegister(0x6203, 0x011E); // set speed
    delay(10);
    
    return result;
}

// Set acceleration to default value
uint8_t setAcceleration(int ID) {
    uint8_t result = 0;
    node[ID].writeSingleRegister(0x6204, DEFAULT_ACCEL); // set speed
    delay(10);
    
    return result;
}

// Set RPM to default value
uint8_t setRPM(int ID) {
    uint8_t result = 0;
    node[ID].writeSingleRegister(0x0001, PULSES_PER_REV);
    delay(10);
    
    return result;
}

// Get current position
uint16_t getPosition(int ID) {
    uint16_t result = node[ID].readHoldingRegisters(0x6202, 16);
    delay(10);

    return result;
}

uint8_t setPosition(int ID, float position) {
    uint8_t result;
  //  float position_rev = (float) max_position[ID]*position;
    int position_pulses = position*PULSES_PER_REV;

    DEBUG_SERIAL.print("Calculated ");
    DEBUG_SERIAL.print(position_pulses, DEC);
    DEBUG_SERIAL.println(" pulses for rotation.");

    node[ID].writeSingleRegister(0x6201, 0x0000); // set position high word
    delay(10);
    
    result = node[ID].writeSingleRegister(0x6202, position_pulses); // set position low word
    delay(10);
    
    return result;
}

// Kick the ball
uint8_t doKick(int ID) {
    uint8_t result = 0;

    setPosition(ID, -0.01);
    moveToPosition(ID);
    setPosition(ID, 0.01);
    moveToPosition(ID);  
   
    return result;
}

uint8_t moveToPosition(int ID) {
    uint8_t result;

    setAcceleration(ID);
    setSpeed(ID);
    setRPM(ID);

    result = node[ID].writeSingleRegister(0x6002, 0x0010); // trigger path
    delay(10);

    return result;
}
