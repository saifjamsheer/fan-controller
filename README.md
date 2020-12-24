# CPU Fan Controller
### Programming an FPGA to Control a 3-pin CPU Fan

* The program is used to control the speed of a 3-pin CPU fan
using the Altera DE1-SoC FPGA as an interface.

* The fan can be controlled through manual linear voltage regulation
or automatic thermostatic control.

* Once the program is executed, the system will start in
the 'Off' state (i.e. Mode 0). 

Note: The ribbon cable (connected to the extension board)
must be connected to the GPIO 0 port on the FPGA.

----------------
    Switches
----------------


The switches can be used to either change the operational
frequency of the fan, the responsiveness/sensitivity of
the rotary encoder, or the information that is displayed
on the seven-segment displays (HEX5 to HEX0).


Switches 0 to 4 (SW0 to SW4):

* These are used to selected the value of the frequency.
* If a flip is switched, the value of the frequency selected
  (in Hz) will be displayed on the seven-segment displays for
  a short period of time.

- No switches:         	     Frequency = 10 Hz
- SW0: 		       	     Frequency = 100 Hz
- SW0 & SW1: 	       	     Frequency = 1000 Hz
- SW0, SW1 & SW2:      	     Frequency = 3000 Hz
- SW0, SW1, SW2 & SW3: 	     Frequency = 5000 Hz
- SW0, SW1, SW2, SW3 & SW4:  Frequency = 7500 Hz


Switches 5 to 8 (SW5 to SW8):

* These are used to selected the value of the responsiveness.
* If a flip is switched, the value of responsiveness selected
  will be displayed on the seven-segment displays for a short
  period of time.

- No switches:         	Responsiveness = 1
- SW5: 		       	Responsiveness = 2
- SW5 & SW6: 	       	Responsiveness = 5
- SW5, SW6 & SW7:      	Responsiveness = 10
- SW5, SW6, SW7 & SW8: 	Responsiveness = 20

  Note: In Mode 1 (auto-mode) this affects acceleration of 
  the fan (the lower the responsiveness the greater the rate 
  of change).


Switch 9 (SW9):

* Changes what information is displayed on the seven-segment
  displays.


----------------
     Keys
----------------

The keys can be used to swap between modes and turn off the
system.

To turn the system on, press one of the following keys
on the FPGA:

- KEY1 (Mode 1)
- KEY2 (Mode 2)
- KEY3 (Mode 3)


Mode 1 (Auto Mode - AUtO):

* No user interaction is necessary in this mode.
* The fan will begin to speed up until max speed is reached
  and then it will start to slow down.
* This will occur continuously until a different key is
  pressed.
* If switch 9 is down, the RPM will be displayed on HEX3 to HEX0.
* If switch 9 is up, the on time will be displayed on HEX2 to HEX0.
* AU will be displayed on HEX5 to HEX4.


Mode 2 (Closed-Loop - CLOSEd):

* The rotary encoder is used to set the desired speed. Rotating
  it clockwise increases the desired speed and counter-clockwise
  decreases the desired speed.
* If switch 9 is down, the desired speed will be displayed on 
  HEX3 to HEX2 and the measured speed will be displayed on HEX1
  to HEX0.
* If switch 9 is up, the on time will be displayed on HEX2 to HEX0.  
* CL will be displayed on HEX5 to HEX4.
* In this mode the fan will speed up/slow down so that the measured
  speed matches the desired speed (selected by the user).

Note: Works best with a PWM Frequency of 100 or 10.


Mode 3 (Open-Loop - OPen):

* The rotary encoder is used to set the duty cycle. Rotating
  it clockwise increases the duty cycle and counter-clockwise
  decreases the duty cycle.
* If switch 9 is down, the set speed will be displayed on 
  HEX3 to HEX2 and the measured speed will be displayed on HEX1
  to HEX0.
* If switch 9 is up, the on time will be displayed on HEX2 to HEX0.
* OP will be displayed on HEX5 to HEX4.


To turn the system off press the following key:

- KEY0 (Mode 0)


Mode 0 (Off - OFF):

* The speed of the fan drops to zero.
* OFF will be displayed on HEX5 to HEX4.


----------------
     LEDs
----------------


In modes 1 to 3 the LEDs display the value of the duty cycle 
(0-100) in tens.

E.g. If the duty cycle is 46, 4 lights will be lit up.

___________________________________________________________________

		       EXAMPLE INTERACTION
___________________________________________________________________


1. Press Key 3 on the FPGA:

     * Open-loop mode is selected.

2. Rotate the rotary encoder clockwise:

     * Duty cycle is increased and fan begins to speed up.
     * The change in desired speed and measured speed can
        be seen on HEX3 to HEX2 and HEX1 to HEX0 respectively.

3. Flip SW0:

     * PWM frequency is increased from 10Hz to 100Hz.

4. Flip SW1:

     * PWM frequency is increased from 100Hz to 1000Hz.

5. Rotate the rotary encoder counter-clockwise:

     * Duty cycle is decreased and fan begins to slow down.

6. Flip SW1 back to its initial state:

     * PWM frequency is decreased from 1000Hz to 100Hz.


7. Press Key 2 on the FPGA:

     * Closed-loop mode is selected.

8. Rotate the rotary encoder clockwise:

     * Desired speed is increased and fan speeds up to match
        the speed.
     * Change in measured speed can be observed on HEX1 to HEX0.

9. Flip SW5, SW6 and SW7:

     * Responsiveness is increased from 1 to 10.

10. Rotate the rotary encoder clockwise:

     * Each rotational movement increases the desired speed
        by 5 (duty cycle is increased by 10).

11. Flip SW9:

     * Seven-segment display information changes.
     * On-time is now displayed on HEX2 to HEX0.

12. Press Key 1 on the FPGA:

     * Auto-mode is selected
     * Fan begins to gradually speed up until max speed is 
        reached, fan then begins to slow down.

13. Flip SW6 and SW7 back to their initial states:

     * Responsiveness is decreased from 10 to 2.
     * The rate at which the fan speed increases/decreases
	(i.e acceleration) is increased.

14. Press Key 0 on the FPGA:

     * System is turned off and fan slows down to stationary.

