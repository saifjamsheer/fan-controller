/*
*  main.c
*  main program for EE30186
*
*  Last modified on 13/12/19.
*/

/* ----------------------------------- */
/* MAIN PROGRAM FOR THE FAN CONTROLLER */
/* ----------------------------------- */

#include "EE30186.h"
#include "system.h"
#include "socal/socal.h"
#include <inttypes.h>
#include <stdio.h>

// Including custom header files
#include "user_func.h"
#include "fan_func.h"
#include "disp_func.h"
#include "misc_func.h"
#include "globals.h"

// Initializing global pointers that are used to interface with the FPGA
volatile int * LEDs     = (volatile int *)ALT_LWFPGA_LED_BASE;
volatile int * Switches = (volatile int *)ALT_LWFPGA_SWITCH_BASE;
volatile unsigned int * Counter = (volatile unsigned int *)ALT_LWFPGA_COUNTER_BASE;
volatile int * Keys = (volatile int *)ALT_LWFPGA_KEY_BASE;
volatile int * GpioPort = (volatile int *) (ALT_LWFPGA_GPIO_0A_BASE);
volatile int * Hex3to0 = (volatile int *)(ALT_LWFPGA_HEXA_BASE);
volatile int * Hex5to4 = (volatile int *)(ALT_LWFPGA_HEXB_BASE);

// Initializing global constants to be used in multiple functions
const int ClockFrequency = 50000000;
const int MaxRPS = 42;

int main(int argc, char** argv)
{
    // Function call to initialise the FPGA configuration
    EE30186_Start();

    // Setting the data direction register to 1
    volatile int *GpioDdr = GpioPort + 1;

    // Setting the state of all pins on GPIO port 0 to off
    *GpioPort = 0x00;

    // Setting the 4th pin of * GpioPort as an output pin
    *GpioDdr = 0x08;

    // Defining all main variables and setting initial conditions

    int Mode = 0;               // Mode that is selected based on the pressed key
    int PWMFrequency = 10;      // Operating frequency of the fan
    int Responsiveness = 1;     // Constant that alters the rate at which the rotary encoder affects the duty cycle

    unsigned int Cycle;         // Current cycle of the counter, loops from 0 to 100

    int DutyCycle = 0;          // Duty cycle of the PWM, can be any integer between 0 and 100
    int OnTime = DutyCycle;     // On-time of the PWM, equivalent to DutyCycle in this case due to the selected Cycle period
    int DesiredSpeed;           // A function of DutyCycle with a minimum value of 0 and a maximum value of 50

    int RPS = 0;                // Speed of the fan in RPS

    int Switches4to0;           // Takes in the values of SW4 to SW0
    int Switches8to5;           // Takes in the values of SW8 to SW5
    int Switch9;                // Takes in the value of SW9

    int FanOn = 0;              // Integer that determines if fan is on or off (0-1)
    int ResetClosed = 0;        // Integer that determines if closed-loop errors should be reset

    // Start of the main loop which runs continuously
    while (1)
        {

            // Extracting the required values from the switches
            Switches4to0 = (*Switches)&31;
            Switches8to5 = ((*Switches)&480) >> 5;
            Switch9 = ((*Switches)&512) >> 9;

            // Lighting up the LEDs based on the value of DutyCycle
            LEDLights(DutyCycle);

            // Selecting the PWMFrequency and Responsiveness based on the relevant switch values
            PWMFrequency = FreqSelect(Switches4to0, PWMFrequency);
            Responsiveness = RespSelect(Switches8to5, Responsiveness);

            // Selecting the desired mode
            Mode = ModeSelect(Mode, &DutyCycle, &RPS, &ResetClosed);

            // Switch statement that determines which functions to run based on which mode is selected
            switch (Mode)
            {

            // Mode 0: Off-mode; fan is turned off
            case 0:
                *GpioPort = 0x00;
                break;

            // Mode 1: Auto-mode; fan speed gradually increases and then decreases
            case 1:
                Cycle = Timer(PWMFrequency);
                DutyCycle = AutoEncoder(DutyCycle, Responsiveness);
                OnTime = DutyCycle;
                PWMGenerator(Cycle, OnTime, &FanOn);
                // Updating RPS only when the PWM signal is high or the fan is completely stationary
                if (FanOn || (OnTime == 0)) { RPS = Tachometer(RPS); }
                break;

            // Mode 2: Closed-loop; uses PID control to make sure the speed of the fan is similar to the desired speed
            case 2:
                Cycle = Timer(PWMFrequency);
                DutyCycle = RotaryEncoder(DutyCycle, Responsiveness);
                DesiredSpeed = DutyCycle/2;
                // Adjusting OnTime through PID control
                OnTime = ClosedLoopController(DesiredSpeed, RPS, OnTime, &ResetClosed);
                PWMGenerator(Cycle, OnTime, &FanOn);
                // Updating RPS only when the PWM signal is high or the fan is completely stationary
                if (FanOn || (OnTime == 0)) { RPS = Tachometer(RPS); }
                break;

            // Mode 3: Open-loop; standard mode that implements open-loop control
            case 3:
                Cycle = Timer(PWMFrequency);
                DutyCycle = RotaryEncoder(DutyCycle, Responsiveness);
                DesiredSpeed = DutyCycle/2;
                OnTime = DutyCycle;
                PWMGenerator(Cycle, OnTime, &FanOn);
                // Updating RPS only when the PWM signal is high or the fan is completely stationary
                if (FanOn || (OnTime == 0)) { RPS = Tachometer(RPS); }
                break;

            default:
                break;

            }

            // Displaying relevant information on the seven-segment displays
            Display(Mode, Switch9, DutyCycle, RPS, DesiredSpeed, OnTime, PWMFrequency);

        }

    // Function call to clean up and close the FPGA configuration
    EE30186_End();

    return 0;

}


