/*
*  disp_func.h
*  display functions header file
*
*  Last modified on 13/12/19.
*/

/* ----------------------------------------------------------------- */
/* HEADER FILE FOR FUNCTIONS USED TO DISPLAY INFORMATION TO THE USER */
/* ----------------------------------------------------------------- */

#ifndef DISP_FUNC_H
#define DISP_FUNC_H

// FUNCTION DECLARATIONS //

void Display(int, int, int, int, int, int, int);    // Displays key information onto the
                                                    // seven-segment displays relevant to
                                                    // the selected mode.


void ScrollDisplay(int[]);    // Displays values by scrolling them
                              // from right to left on the seven-segment
                              // displays.


int SevenSegmentDecoder(int);   // Converts a single-digit integer to a value
                                // that can be displayed on the seven-segment
                                // displays.


int MultiDigitDecoder(int);    // Converts a multi-digit integer to a value
                               // that can be displayed on the seven-segment
                               // displays

void LEDLights(int);    // Sets * LEDs to be representative of the
                        // duty cycle value


#endif

