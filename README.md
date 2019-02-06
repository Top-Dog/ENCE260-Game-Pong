# ENCE 260 Microcontroller Project
This is the final tested and working code for the University of Canterbury (UC) ENCE260 AVR microcontroller assignment in 2012. 
The program runs a game of 'pong' on each UCFK board (requires two boards to play).

The code is designed to run on an Atmel AVR ATmega32u2. The project files are for Atmel Studio.

There are a number of hardware drivers in the `src/drivers` directory that can be ported to other microcontrollers or AVR projects.
There are also a number of high level utility modules (utils) in the `src/utils` directory to help abstract common tasks like PWM, task scheduling, drawing on the dot-matrix display, formatting ASCII strings etc.

There is also a very useful printf debugger that connects stdout to the serial UART on the AVR and stderr on the PC. It is located in the library `src/app/debug.c`.
```C
/*
* TRACE(...): Same arguments as printf. On PC, print a message to  
*              stderr. On UCFK, disable the IR transmitter and send 
*              the message over the serial connection. 
*  
*  ASSERT(ex): If the expression evaluates to false, print a message containing 
*              the expression, the file name, and the line number. On UCFK, 
*              also turn on the blue LED. 
*/
```
