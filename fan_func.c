/*
*  fan_func.c
*  fan functions source file
*
*  Last modified on 13/12/19.
*/

/* -------------------------------------------------------------- */
/* SOURCE FILE FOR FUNCTIONS USED TO CONTROL THE SPEED OF THE FAN */
/* -------------------------------------------------------------- */

#include "EE30186.h"
#include "system.h"
#include "socal/socal.h"
#include <inttypes.h>
#include <stdio.h>
#include "fan_func.h"

// Including other necessary custom headers
#include "misc_func.h"
#include "globals.h"

/*
* Function: Timer
* --------------------------------
* Uses the internal counter of the FPGA (*Counter) to create a cycle
* count that loops from 0 to 100. The time it takes to increment is
* dependent on the PWM frequency.
*
* PWMFrequency: Operating frequency of the fan in Hz.
*
* Returns: Cycle, an integer between 0 and 100 indicative of the
* current cycle count.
*/

int Timer(int PWMFrequency)
{

    static int Cycle = 0; // Cycle count of the system
    int Period = ClockFrequency/PWMFrequency; // Period of the system

    // Setting cycle to a value between 0 and 100 based on the period
    Cycle = (100*(*Counter%Period))/Period;

    return Cycle;
}

/*
* Function: RotaryEncoder
* --------------------------------
* Reads pin A and pin B of the rotary encoder to determine if
* the rotary encoder is being rotated and, if so, the direction
* it is being rotated. Based on the direction, the duty cycle is
* either incremented or decremented by a factor of the responsiveness.
*
* DutyCycle: The current operating duty cycle (0-100).
* Responsiveness: The rate at which rotating the encoder affects the
* duty cycle (i.e. sensitivity).
*
* Returns: DutyCycle, the new duty cycle that was either incremented
* decremented or unchanged.
*/

int RotaryEncoder(int DutyCycle, int Responsiveness)
{

    int AState; // Current value of pin A of the rotary encoder
    int BState; // Current value of pin B of the rotary encoder
    static int APrevState; // Previous value of pin A of the rotary encoder

    // Reading the values of pin A and B of the rotary encoder from the relevant
    // pins on GPIO port 0
    AState = (*GpioPort >> 17) & 0x01;
    BState = (*GpioPort >> 19) & 0x01;

    // Determining if the rotary encoder is being rotated by
    // comparing the AState with APrevState
    if (AState != APrevState)
    {
        // Determining the direction the rotary encoder is
        // rotating (clockwise or counter-clockwise) and
        // modifying DutyCycle accordingly
        if (AState != BState)
        {
            // Incrementing the duty cycle
            DutyCycle += (1*Responsiveness);
        }
        else
        {
            // Decrementing the duty cycle
            DutyCycle -= (1*Responsiveness);
        }
    }

    // Restricting the value of the duty cycle between 0 and 100
    DutyCycle = (DutyCycle > 100) ? 100 : DutyCycle;
    DutyCycle = (DutyCycle < 0) ? 0 : DutyCycle;

    // Setting the previous value of pin A
    APrevState = AState;

    return DutyCycle;
}

/*
* Function: AutoEncoder
* --------------------------------
* Automatically increments the value of the DutyCycle by 1 every 7500
* loops. Once a duty cycle of 100 is achieved, the function then begins
* decrementing the duty cycle until it reaches 0 and then begins
* incrementing it again. This goes on continuously. A change is made
* every 7500 loops so that users can clearly see the change in fan
* speed (which is linked to the duty cycle).
*
* DutyCycle: The current operating duty cycle (0-100).
* Multiplier: An integer that determines how quickly the fan
* changes speed.
*
* Returns: DutyCycle, the new duty cycle that was either incremented
* decremented.
*/

int AutoEncoder(int DutyCycle, int Multiplier)
{

    static int Flag = 0; // Determines if DutyCycle should be incremented or decremented
    static int Loop = 0; // Integer that represents the loop number
    int LoopEnd = 1000*Multiplier; // End point of loop

    // DutyCycle is only altered when Loop reaches a certain multiple;
    // this is done to ensure that changes in the fan's speed can be
    // seen by the user
    if (Loop >= LoopEnd)
    {
        // Determining whether the DutyCycle should be incremented or
        // decremented based on the value of the flag
        if (Flag == 0) {
            // Incrementing duty cycle
            DutyCycle++;
        }
        else
        {
            // Decrementing duty cycle
            DutyCycle--;
        }

        // Resetting the loop number after a change is made
        Loop = 0;

    }

    // Restricting the value of the duty cycle between 0 and 100
    DutyCycle = (DutyCycle > 100) ? 100 : DutyCycle;
    DutyCycle = (DutyCycle < 0) ? 0 : DutyCycle;

    // Changing the value of the flag once a duty cycle limit is reached
    Flag = (DutyCycle == 100) ? 1 : Flag;
    Flag = (DutyCycle == 0) ? 0 : Flag;

    Loop++;

    return DutyCycle;
}

/*
* Function: ClosedLoopController
* --------------------------------
* Calculates a new OnTime using PID control. The error used to correct
* the OnTime is calculated by finding the difference between the desired
* speed of the fan and the measured speed of the fan.
*
* DesiredSpeed: The desired speed of the fan set by the user (0-50).
* RPS: The measured speed of the fan in revolutions per second (0-42).
* OnTime: The number of cycles during which the fan is turned on (0-100).
* *ResetClosed: Pointer to integer determining if the static variables
* should be reset.
*
* Returns: OnTime, the controlled number of on cycles, which is used to
* control the speed of the fan.
*/

int ClosedLoopController(int DesiredSpeed, int RPS, int OnTime, int *ResetClosed)
{

    // Declaring the PID variables
    float Error; // Proportional component
    float Derivative; // Derivative component
    static float Integral; // Integral component

    static float PrevError; // Previous error

    static float Timing; // Intermediary variable for OnTime that
                         // allows for more precise control (as it
                         // is a float)

    if (*ResetClosed) {
        // Resetting static variables
        Integral = 0.0;
        PrevError = 0.0;
        Timing = 0.0;
        Set(ResetClosed, 0);
    }

    // Setting measured speed to be a value from 0 to 50
    // so it can be compared with the desired speed
    int MeasuredSpeed = (RPS*50)/MaxRPS;

    // Calculating error by subtracting the measured speed
    // from the desired speed
    Error = DesiredSpeed - MeasuredSpeed;

    // Summing the integral component with the error term
    // to improve stability
    Integral += Error;

    // Calculating the derivative component, which would
    // help reduce the magnitude of overshoot
    Derivative = Error - PrevError;

    // Applying PID control to the intermediary variable
    Timing += Kp*Error + Ki*Integral + Kd*Derivative;

    // Limiting the intermediary variable between 0 and 100
    Timing = (Timing >= 101.0) ? 100.0 : Timing;
    Timing = (Timing <= 0.0) ? 0.0 : Timing;

    // Setting the on time of the system to the rounded
    // value of Timing
    OnTime = Timing;

    // Setting previous error to the current error
    PrevError = Error;

    return OnTime;
}

/*
* Function: PWMGenerator
* --------------------------------
* Compares the values of the two inputs and sets the pin on the GPIO
* port of the FPGA (*GpioPort) associated with the fan to high or
* low appropriately.
*
* Cycle: The current cycle count (0-100).
* OnTime: The number of cycles (0-100) during which the pin
* associated with the fan should be set to high. It is a
* function of the duty cycle.
* *FanOn: Pointer to integer that is set to 1 if the fan is on
* or 0 if the fan is off.
*/

void PWMGenerator(int Cycle, int OnTime, int *FanOn)
{

    // Setting the output pin of the GPIO port to high or low
    // depending on whether the current cycle is less than or
    // greater than the OnTime
    if (Cycle < OnTime)
    {
        // Fan is turned on
        *GpioPort = 0x08;
        Set(FanOn, 1);
    }
    else
    {
        // Fan is turned off
        *GpioPort = 0x00;
        Set(FanOn, 0);
    }

}

/*
* Function: Tachometer
* --------------------------------
* Compares the current and previous values of the pin associated with the
* tachometer to detect any rising edges, which are used to determine the
* number of half-revolutions that occur in 0.5 seconds. The speed of the
* fan is then calculated based on the measured number of half-revolutions.
*
* RPS: The speed of the fan in revolutions per second.
*
* Returns: RPS, the updated or unchanged speed of the fan.
*/

int Tachometer(int RPS)
{

    int Count; // Count value based on the counter
    static int PrevCount; // Value of count in previous loop

    int TachState; // Current value of the tachometer pin
    static int PrevTachState; // Previous value of the tachometer pin
    static int BeforePrevTachState; // Value of the tachometer pin before previous value

    static int HalfRevolutions = 0; // Number of half revolutions the fan undergoes

    // Increment count every half a second
    Count = (*Counter/(ClockFrequency/2));

    // Reads the value of the tachometer pin
    TachState = (*GpioPort >> 1) & 0x01;

    // Determining if a half-second has passed
    if (Count != PrevCount)
    {
        // Updating the RPS and resetting the process
        RPS = HalfRevolutions;
        HalfRevolutions = 0;
    }
    else
    {
        // Detecting rising edges in the tachometer signal (built-in low-pass filter)
        if (TachState == 1 && PrevTachState == 1 && (PrevTachState != BeforePrevTachState))
        {
            // Incrementing half revolutions by 1 for each rising edge
            HalfRevolutions++;
        }
    }

    // Setting previous states to the values of the current states
    PrevCount = Count;
    BeforePrevTachState = PrevTachState;
    PrevTachState = TachState;

    return RPS;
}
