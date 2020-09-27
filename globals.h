/*
*  global.h
*  global definitions, pointers & constants header file
*
*  Last modified on 13/12/19.
*/

/* -------------------------------------------------------- */
/* HEADER FILE FOR GLOBAL DEFINITIONS, POINTERS & CONSTANTS */
/* -------------------------------------------------------- */

#ifndef GLOBALS_H
#define GLOBALS_H

// Key values
#define key0 0xE
#define key1 0xD
#define key2 0xB
#define key3 0x7

// PID control constants
#define Kp 0.000025
#define Ki 0.000000000015
#define Kd 0.8

// Seven-segment values
#define seg0 0x40
#define seg1 0xF9
#define seg2 0x24
#define seg3 0x30
#define seg4 0x19
#define seg5 0x12
#define seg6 0x02
#define seg7 0xF8
#define seg8 0x00
#define seg9 0x10
#define segA 0x08
#define segC 0x46
#define segD 0x21
#define segE 0x06
#define segF 0x0E
#define segL 0x47
#define segN 0x2B
#define segO 0x40
#define segP 0x0C
#define segR 0x2F
#define segS 0x12
#define segT 0x07
#define segU 0x41
#define segBlank 0xFF

// Declaring pointers that allow interaction with the FPGA //

extern volatile int * LEDs;                // Indicates the address of the 10 LEDs
extern volatile int * Switches;            // Indicates the address of the 10 switches
extern volatile unsigned int * Counter;    // Indicates the address of the counter
extern volatile int * Keys;                // Indicates the address of the 4 keys
extern volatile int * GpioPort;            // Indicates the address of the 40-pin GPIO-0 port
extern volatile int * Hex3to0;             // Indicates the address of hexadecimal displays HEX3 to HEX0
extern volatile int * Hex5to4;             // Indicates the address of hexadecimal displays HEX5 to HEX4

// Declaring constant globals // 

extern const int ClockFrequency;           // Frequency of the internal clock of the FPGA
extern const int MaxRPS;                   // Pre-determined maximum speed of the fan in RPS

#endif
