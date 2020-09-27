/*
*  misc_func.c
*  miscellaneous functions source file
*
*  Last modified on 13/12/19.
*/

/* --------------------------------------- */
/* SOURCE FILE FOR MISCELLANEOUS FUNCTIONS */
/* --------------------------------------- */

#include "EE30186.h"
#include "system.h"
#include "socal/socal.h"
#include <inttypes.h>
#include <stdio.h>
#include "misc_func.h"

// Including other necessary custom headers
#include "globals.h"

/*
* Function: Set
* --------------------------------
* Accesses the address of an inputted integer to set it to
* a desired value.
*
* *X: A pointer that indicates the address of an integer.
* SetValue: The desired value to set the integer to.
*/

void Set(int *X, int SetValue)
{

    *X = SetValue;

}

/*
* Function: Delay
* --------------------------------
* Creates a manual delay by looping through the length of an
* inputted integer. The greater the value of the input the
* longer the delay.
*
* Length: An integer that determines the length of the loop
* used to create a delay.
*/

void Delay(int Length)
{

    int i = 0;

    while (i < Length)
    {
        i++;
    }

}
