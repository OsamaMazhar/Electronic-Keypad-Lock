# 8051 Based Electronic Keypad Lock

This project is an 8051 based electronic keypad lock. A 5-digit password is hard-coded in the micrcontroller. The lock asks the user to enter the password with 4 trials available. If the password is correct, output is given in 2 hardware bits of microcontroller which are connected externally to a XOR gate to prevent switching of the gate/lock when both the inputs are HIGH or LOW. If password is entered wrong four times, another signal (warning signal) is activated in a similar fashion and the system is locked at this point in the code until the owner resets it by pressing "2" for about eight seconds.

For greater protection, a buzzer is connected to 1-bit of microcontroller which is always HIGH. If the controller is taken out of the base or the IC is damaged or the power supply is disconnected the relay will change its state and turn on the alarm/buzzer. 

The demonstration of this project can be seen in the youtube video linked as follows:

https://www.youtube.com/watch?v=7_CF9adKvfA 

