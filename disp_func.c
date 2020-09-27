/*
*  disp_func.c
*  display functions source file
*
*  Last modified on 13/12/19.
*/

/* ----------------------------------------------------------------- */
/* SOURCE FILE FOR FUNCTIONS USED TO DISPLAY INFORMATION TO THE USER */
/* ----------------------------------------------------------------- */

#include "EE30186.h"
#include "system.h"
#include "socal/socal.h"
#include <inttypes.h>
#include <stdio.h>
#include "disp_func.h"

// Including other necessary custom headers
#include "misc_func.h"
#include "globals.h"

/*
* Function: Display
* --------------------------------
* Displays key information on the seven-segment displays depending
* on which mode is selected. There are two sets of information that
* can be displayed for each mode; the set that is displayed is
* determined by the value of SW9.
*
* Mode: The selected operating mode of the system (0-3).
* Switch9: The value of SW9 on the FPGA.
* DutyCycle: The operating duty cycle (0-100).
* RPS: The speed of the fan in revolutions per second.
* DesiredSpeed: The desired speed set by the user (0-50).
* OnTime: The number of cycles during which the fan is turned on (0=100).
* PWMFrequnecy: The operating frequency of the fan in Hz.
*/

void Display(int Mode, int Switch9, int DutyCycle, int RPS, int DesiredSpeed, int OnTime, int PWMFrequency)
{

    int MultiDigit; // Variable used to write values onto the seven-segment displays
    int RPM = RPS*60; // Measured seed of the fan in RPM
    int MeasuredSpeed = (RPS*50)/MaxRPS; // Scaled measured speed (0-50)

    // Determining what set of values to display based on the position of SW9
    if (!Switch9)
    {
        // Switch statement that determines what values are displayed
        // on the seven-segment displays
        switch (Mode)
        {
        // OFF displayed using HEX5, HEX4 and HEX3
        case 0:
            *Hex3to0 = (segF << 24) | (segBlank << 16) | (segBlank << 8) | (segBlank);
            *Hex5to4 = (segO << 8) | (segF & segBlank);
            break;
        // AU displayed using HEX5 and HEX4; HEX3 to HEX0 displays
        // the speed of the fan in RPM
        case 1:
            *Hex3to0 = MultiDigitDecoder(RPM);
            *Hex5to4 = (segA << 8) | (segU);
            break;
        // CL displayed using HEX5 and HEX4; desired speed displayed
        // using HEX3 and HEX2; measured speed displayed using HEX1
        // and HEX0
        case 2:
            MultiDigit = (MultiDigitDecoder(MeasuredSpeed)) & ((segBlank << 8) | (segBlank));
            MultiDigit |= (MultiDigitDecoder(DesiredSpeed)) << 16;
            *Hex3to0 = MultiDigit;
            *Hex5to4 = (segC << 8) | (segL);
            break;
        // OP displayed using HEX5 and HEX4; desired speed displayed
        // using HEX3 and HEX2; measured speed displayed using HEX1
        // and HEX0
        case 3:
            MultiDigit = (MultiDigitDecoder(MeasuredSpeed)) & ((segBlank << 8) | (segBlank));
            MultiDigit |= (MultiDigitDecoder(DesiredSpeed)) << 16;
            *Hex3to0 = MultiDigit;
            *Hex5to4 = (segO << 8) | (segP);
            break;
        default:
            break;
        }
    }
    else
    {
        // Switch statement that determines what values are displayed
        // on the seven-segment displays
        switch (Mode)
        {
        // OFF displayed on the display
        case 0:
            *Hex3to0 = (segF << 24) | (segBlank << 16) | (segBlank << 8) | (segBlank);
            *Hex5to4 = (segO << 8) | (segF & segBlank);
            break;
        // On time displayed using HEX2 to HEX0
        case 1:
            *Hex3to0 = (segBlank << 24) | MultiDigitDecoder(OnTime);
            *Hex5to4 = (segA << 8) | (segU);
            break;
        // Duty cycle displayed using HEX5 to HEX3; on time displayed
        // using HEX2 to HEX0
        case 2:
            *Hex3to0 = (segBlank << 24) | MultiDigitDecoder(OnTime);
            *Hex5to4 = (segC << 8) | (segL);
            break;
        // RPM displayed using HEX3 to HEX0
        case 3:
            *Hex3to0 = MultiDigitDecoder(RPM);
            *Hex5to4 = (segO << 8) | (segP);
            break;
        default:
            break;
        }
    }
    
}

/*
* Function: ScrollDisplay
* --------------------------------
* Takes in six display values based on their final positions and
* displays them by scrolling them from left to right on the
* seven-segment displays.
*
* SegArray[]: Integer rray of segment values to be displayed
* on the seven-segment displays.
*/

void ScrollDisplay(int SegArray[])
{
    
    int Del = 500000; // The delay between shifting each displayed value

    // Setting the seven-segment displays to be blank
    *Hex3to0 = (segBlank << 24) | (segBlank << 16) | (segBlank << 8) | (segBlank);
    *Hex5to4 = (segBlank << 8) | (segBlank);

    Delay(Del);

    // Displaying the first value on HEX0
    *Hex3to0 = (segBlank << 24) | (segBlank << 16) | (segBlank << 8) | (SegArray[0]);
    *Hex5to4 = (segBlank << 8) | (segBlank);

    Delay(Del);

    // Displaying the first two values on HEX1 and HEX0
    *Hex3to0 = (segBlank << 24) | (segBlank << 16) | (SegArray[0] << 8) | (SegArray[1]);
    *Hex5to4 = (segBlank << 8) | (segBlank);

    Delay(Del);

    // Displaying the first three values on HEX2 to HEX0
    *Hex3to0 = (segBlank << 24) | (SegArray[0] << 16) | (SegArray[1] << 8) | (SegArray[2]);
    *Hex5to4 = (segBlank << 8) | (segBlank);

    Delay(Del);

    // Displaying the first four values on HEX3 to HEX0
    *Hex3to0 = (SegArray[0] << 24) | (SegArray[1] << 16) | (SegArray[2] << 8) | (SegArray[3]);
    *Hex5to4 = (segBlank << 8) | (segBlank);

    Delay(Del);

    // Displaying the first five values on HEX4 to HEX0
    *Hex3to0 = (SegArray[1] << 24) | (SegArray[2] << 16) | (SegArray[3] << 8) | (SegArray[4]);
    *Hex5to4 = (segBlank << 8) | (SegArray[0]);

    Delay(Del);

    // Displaying all six values on HEX5 to HEX0
    *Hex3to0 = (SegArray[2] << 24) | (SegArray[3] << 16) | (SegArray[4] << 8) | (SegArray[5]);
    *Hex5to4 = (SegArray[0] << 8) | (SegArray[1]);

    Delay(Del*2);

}

/*
* Function: SevenSegmentDecoder
* Created by Dr. Despina Moschou
* --------------------------------
* Takes in a single-digit integer and outputs a value that can be
* used to set the segments of the seven-segment display to match
* the inputted value.
*
* Digit: A single-digit integer.
*
* Returns: Segments, a hexadecimal number representing Digit that
* can be written to one of the hex displays on the FPGA.
*/

int SevenSegmentDecoder (int Digit)
{

    int Segments;

    // Chooses a value for the segments based on the value of the input. The Hex
    // display is inverted so a bit value of 1 turns off the digit.
    switch (Digit)
    {
    case 0:
        Segments = seg0;
        break;
    case 1:
        Segments = seg1;
        break;
    case 2:
        Segments = seg2;
        break;
    case 3:
        Segments = seg3;
        break;
    case 4:
        Segments = seg4;
        break;
    case 5:
        Segments = seg5;
        break;
    case 6:
        Segments = seg6;
        break;
    case 7:
        Segments = seg7;
        break;
    case 8:
        Segments = seg8;
        break;
    case 9:
        Segments = seg9;
        break;
    default:
        Segments = seg0;
        break;
    }

    // Pass back the value needed to set the segments correctly
    return Segments;
}

/*
* Function: MultiDigitDecoder
* Created by Dr. Despina Moschou
* --------------------------------
* Takes in a multi-digit integer and outputs a value that can be
* used to set the segments of the seven-segment display to match
* the inputted value. Makes use of the function SevenSegmentDecoder.
*
* Value: A multi-digit integer.
*
* Returns: ReturnValue, a hexadecimal number that can be used ti
* display Value across multiple seven-segment displays.
*/

int MultiDigitDecoder (int Value)
{

    // Starts with a display value of zeros to return
    int ReturnValue = (seg0 << 24) | (seg0 << 16) | (seg0 << 8) | (seg0);

    // We need to be able to keep track of which digit in the number we are dealing
    // with
    int CurrentDigit = 0;

    // As we extract the digits we need a temporary variable to put the values into
    int SingleDigitDisplay;

    // Loop up through the digits in the number. By using a do-while loop we
    // ensure that the decoder is called at least once. This means that a zero is
    // displayed as a single zero on the display rather than all blank.
    do
    {
        // Extract the bottom digit
        SingleDigitDisplay = SevenSegmentDecoder(Value%10);

        // Adjust the input value to reflect the extraction of the bottom digit
        Value /= 10;

        // Clear the space that we are going to put the decoder result into
        ReturnValue = ReturnValue & ~(segBlank << (CurrentDigit * 8));

        // Shift the single decoded digit to the right place in the int and insert
        // it
        ReturnValue = ReturnValue |  (SingleDigitDisplay << (CurrentDigit * 8));

        // Update the digit postion so that if the value is non-zero we put the
        // next digit 8 bits further to the left.
        CurrentDigit++;

    } while (Value > 0);

    // Pass back the multi-digit decoded result.
    return ReturnValue;
}

/*
* Function: LEDLights
* --------------------------------
* Writes a value to the LEDs on the FPGA (*LEDs) based on the
* current duty cycle value to turn specific LEDs on and off
* (i.e. the four leftmost LEDs would be turned on for a duty
* cycle between 40 and 49).
*
* DutyCycle: The current operating duty cycle (0-100).
*/

void LEDLights(int DutyCycle)
{

    int SingleDigitCycle = DutyCycle/10; // Value of left-most digit of the duty cycle

    // Turning on appropriate LEDs based on duty cycle value
    *LEDs = (0b1111111111) << (10 - SingleDigitCycle);

}
