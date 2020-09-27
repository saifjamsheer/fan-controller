/*
*  fan_func.h
*  fan functions header file
*
*  Last modified on 13/12/19.
*/

/* -------------------------------------------------------------- */
/* HEADER FILE FOR FUNCTIONS USED TO CONTROL THE SPEED OF THE FAN */
/* -------------------------------------------------------------- */

#ifndef FAN_FUNC_H
#define FAN_FUNC_H

// FUNCTION DECLARATIONS //

int Timer(int);    // Uses * Counter to create a cycle count that
                   // loops from 0 to 100 at a rate dependent on
                   // the period.


int RotaryEncoder(int, int);    // Increases or decreases the value of the duty
                                // cycle based on the rotation of the rotary encoder.


int AutoEncoder(int, int);   // Slowly increases the duty cycle to a maximum
                        	 // before decreasing it to a minimum. This loop
                             // occurs repeatedly.


int ClosedLoopController(int, int, int, int *);    // Implements PID control on the on-time of the
                                                   // system to set the measured speed of the fan
                                                   // to the desired speed.


void PWMGenerator(int, int, int *);    // Turns the fan on and off by sending a signal
                                       // that is high or low to the relevant pin on the
                                       // GPIO Port of the FPGA.

int Tachometer(int);    // Calculates the speed of the fan in RPS
                        // using the tachometer pin.

#endif
