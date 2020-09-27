/*
*  user_func.c
*  user functions source file
*
*  Last modified on 13/12/19.
*/

/* ------------------------------------------------------------- */
/* SOURCE FILE FOR FUNCTIONS CONTROLLED THROUGH USER INTERACTION */
/* ------------------------------------------------------------- */

#include "EE30186.h"
#include "system.h"
#include "socal/socal.h"
#include <inttypes.h>
#include <stdio.h>
#include "user_func.h"

// Including other necessary custom headers
#include "disp_func.h"
#include "misc_func.h"
#include "globals.h"

/*
* Function: ModeSelect
* --------------------------------
* Uses the keys on the FPGA (*Keys) to determine what operating mode
* should be selected. Resets the values of DutyCycle and RPS when a
* new mode is selected and displays the mode name on the seven-segment
* displays.
*
* Mode: The previously selected mode.
* *DutyCycle: Pointer to the current operating duty cycle (0-100).
* *RPS: Pointer to the current speed of the fan in RPS.
* *ResetClosed: Pointer to the integer (1 or 0) determining if the
* static variables in the function ClosedLoopController should be reset.
*
* Returns: Mode, an integer representing the selected mode.
*/

int ModeSelect(int Mode, int *DutyCycle, int *RPS, int *ResetClosed)
{

    int ModeArray[4] = {0, 1, 2, 3}; // Array of all possible modes
    int SegArray[6]; // Array to hold segment values
    static int PrevMode; // Previously selected mode

    // Switch statement that sets the mode based on the selected key
    // and resets relevant variables to their initial conditions
    switch (*Keys)
    {
    // Off
    case key0:
    	// Setting mode to 1st item in the array
        Mode = ModeArray[0];

        // Checking if the mode has been changed
		if (Mode != PrevMode)
		{
			// Resetting variables to initial conditions
			Set(DutyCycle, 0);
			Set(RPS, 0);

			// OFF displayed on seven-segment displays (scrolls right to left)
			SegArray[0] = segO;
			SegArray[1] = segF;
			SegArray[2] = segF;
			SegArray[3] = segBlank;
			SegArray[4] = segBlank;
			SegArray[5] = segBlank;
			ScrollDisplay(SegArray);
		}
        break;
    // Auto-mode
    case key1:
    	// Setting mode to 2nd item in the array
        Mode = ModeArray[1];

        // Checking if the mode has been changed
		if (Mode != PrevMode)
		{
			// Resetting variables to initial conditions
			Set(DutyCycle, 0);
			Set(RPS, 0);

			// AUto displayed on seven-segment displays (scrolls right to left)
			SegArray[0] = segA;
			SegArray[1] = segU;
			SegArray[2] = segT;
			SegArray[3] = segO;
			SegArray[4] = segBlank;
			SegArray[5] = segBlank;
			ScrollDisplay(SegArray);
		}
        break;
    // Closed-loop
    case key2:
    	// Setting mode to 3rd item in the array
        Mode = ModeArray[2];

        // Checking if the mode has been changed
        if (Mode != PrevMode)
        {
        	// Resetting variables to initial conditions
        	Set(DutyCycle, 0);
			Set(RPS, 0);
			Set(ResetClosed, 1);

			// CLOSEd displayed on seven-segment displays (scrolls right to left)
			SegArray[0] = segC;
			SegArray[1] = segL;
			SegArray[2] = segO;
			SegArray[3] = segS;
			SegArray[4] = segE;
			SegArray[5] = segD;
			ScrollDisplay(SegArray);
        }
        break;
    // Open-loop
    case key3:
    	// Setting mode to 4th item in the array
        Mode = ModeArray[3];

        // Checking if the mode has been changed
        if (Mode != PrevMode)
        {
        	// Resetting variables to initial conditions
        	Set(DutyCycle, 0);
			Set(RPS, 0);

			// OPEn displayed on seven-segment displays (scrolls right to left)
			SegArray[0] = segO;
			SegArray[1] = segP;
			SegArray[2] = segE;
			SegArray[3] = segN;
			SegArray[4] = segBlank;
			SegArray[5] = segBlank;
			ScrollDisplay(SegArray);
        }
        break;
    default:
        break;
    }

    PrevMode = Mode;

    return Mode;
}

/*
* Function: FreqSelect
* --------------------------------
* Uses switches 0 to 4 on the FPGA to select the value of the
* operating frequency of the fan.
*
* Switches4to0: The values of SW4 to SW0 on the FPGA.
* PWMFrequency: The operating frequency of the fan in Hz.
*
* Returns: PWMFrequency, the PWM frequency that was selected
* based on which switches are flipped.
*/

int FreqSelect(int Switches4to0, int PWMFrequency)
{

    int CurrentSwitches = Switches4to0; // Current value of switches
    static int PrevSwitches; // Previous value of switches

    // Switch statement that sets the PWM frequnecy based on the selected
    // switches
    switch (Switches4to0)
    {
    case 0b00001:
        PWMFrequency = 100;
        break;
    case 0b00011:
        PWMFrequency = 1000;
        break;
    case 0b00111:
        PWMFrequency = 3000;
        break;
    case 0b01111:
        PWMFrequency = 5000;
        break;
    case 0b11111:
        PWMFrequency = 7500;
        break;
    default:
        PWMFrequency = 10;
        break;
    }

    if (CurrentSwitches != PrevSwitches)
    {
        // Displaying the frequency selected by the user if a change is made
        *Hex5to4 = segF << 8 | segBlank;
        *Hex3to0 = MultiDigitDecoder(PWMFrequency);
        Delay(2500000);
    }

    // Setting the previous switch values to the current values
    PrevSwitches = CurrentSwitches;

    return PWMFrequency;
}

/*
* Function: RespSelect
* --------------------------------
* Uses switches 5 to 8 on the FPGA to select the value of the
* integer Responsiveness.
*
* Switches8to5: The values of SW8 to SW5 on the FPGA.
* Responsiveness: An integer that determines how responsive the
*.rotary encoder is to rotations.
*
* Returns: Responsiveness, the responsiveness value that is selected
* based on which switches are flipped.
*/

int RespSelect(int Switches8to5, int Responsiveness)
{

    int CurrentSwitches = Switches8to5; // Current value of switches
    static int PrevSwitches; // Previous value of switches

    // Switch statement that sets the responsiveness based on the selected
    // switches
    switch (Switches8to5)
    {
    case 0b0001:
        Responsiveness = 2;
        break;
    case 0b0011:
        Responsiveness = 5;
        break;
    case 0b0111:
        Responsiveness = 10;
        break;
    case 0b1111:
        Responsiveness = 20;
        break;
    default:
        Responsiveness = 1;
        break;
    }

    if (CurrentSwitches != PrevSwitches)
    {
        // Displaying the responsiveness selected by the user if a change is made
        *Hex5to4 = segR << 8 | segBlank;
        *Hex3to0 = (segBlank << 24) | (segBlank << 16) | MultiDigitDecoder(Responsiveness);
        Delay(2500000);
    }

    // Setting the previous switch values to the current values
    PrevSwitches = CurrentSwitches;

    return Responsiveness;
}
