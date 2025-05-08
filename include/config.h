#ifndef CONFIG_H
#define CONFIG_H

#define MAX485_DE      8
#define MAX485_RE_NEG  9

#define TFT_DC         16 // 
#define TFT_RST        17 // Or set to -1 and connect to Arduino RESET pin
#define TFT_CS         10

#define ROT_PIN        A8 // GPIO22 on teensy 4.0
#define POS_PIN        A9 // GPIO23 on teensy 4.0


#define MOTOR_SERIAL Serial3 // Or Serial1 for controller V2
#define DEBUG_SERIAL SerialUSB1


#endif
