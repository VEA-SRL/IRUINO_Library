#ifndef Pins_Arduino_h
#define Pins_Arduino_h


#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#define IRUINO

#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        40
#define NUM_ANALOG_INPUTS       16


#define analogInputToDigitalPin(p)  (((p)<20)?(esp32_adc2gpio[(p)]):-1)
#define digitalPinToInterrupt(p)    (((p)<40)?(p):-1)
#define digitalPinHasPWM(p)         (p < 34)


extern const uint8_t digital_in_pin[];
extern const uint8_t digital_out_pin[];
extern const uint8_t analog_in_pin[];
extern const uint8_t number_of_input_pins;
extern const uint8_t number_of_output_pins;
extern const uint8_t number_of_analog_pins;
extern const bool input_inverted[];

static const uint8_t I0    = 16;
static const uint8_t I1    = 27;
static const uint8_t I2    = 32;
static const uint8_t I3    = 5;
static const uint8_t I4    = 34;
static const uint8_t I5    = 35;
static const uint8_t I6    = 39;
static const uint8_t I7    = 36;

static const uint8_t O0    = 2;
static const uint8_t O1    = 12;
static const uint8_t O2    = 17;
static const uint8_t O3    = 18;
static const uint8_t O4    = 19;
static const uint8_t O5    = 23;
static const uint8_t O6    = 25;
static const uint8_t O7    = 26;

static const uint8_t DAC1 = 25;
static const uint8_t DAC2 = 26;

static const uint8_t A0 = 4;
static const uint8_t A1 = 13;


static const uint8_t TX = 1;
static const uint8_t RX = 3;

static const uint8_t SDA = 22;
static const uint8_t SCL = 21;

static const uint8_t T0 = 4;
static const uint8_t T1 = 0;
static const uint8_t T2 = 2;
static const uint8_t T3 = 15;
static const uint8_t T4 = 13;
static const uint8_t T5 = 12;
static const uint8_t T6 = 14;
static const uint8_t T7 = 27;
static const uint8_t T8 = 33;
static const uint8_t T9 = 32;



static const uint8_t SS    = 5;
static const uint8_t MOSI  = 23;
static const uint8_t MISO  = 19;
static const uint8_t SCK   = 18;

#endif /* Pins_Arduino_h */


   

    

