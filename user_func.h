/*
*  user_func.h
*  user functions header file
*
*  Last modified on 13/12/19.
*/

/* ------------------------------------------------------------- */
/* HEADER FILE FOR FUNCTIONS CONTROLLED THROUGH USER INTERACTION */
/* ------------------------------------------------------------- */

#ifndef USER_FUNC_H
#define USER_FUNC_H

// FUNCTION DECLARATIONS //

int ModeSelect(int, int *, int *, int *);   // Selects between open-loop, closed-loop
                                            // and auto-mode depending on which key is
                                            // pressed.


int FreqSelect(int, int);   // Alters PWMFrequency based on the values of
                            // switches 0 to 4.


int RespSelect(int, int);   // Alters the responsiveness of the rotary
                            // encoder based on the values of switches 5
                            // to 8.

#endif
