# File:   Makefile
# Author: M. P. Hayes, UCECE
# Modified by: Sean O'Connor (sdo51), Matthew Edwards (mje109)
# Date:   7 Oct 2012
# Descr:  Makefile for game

# Definitions.
CC := avr-gcc
WARNINGS := -Wall -Wstrict-prototypes -Wextra -Wshadow -Wpointer-arith -Wcast-align -Wwrite-strings -Wredundant-decls -Wnested-externs -Wno-long-long -Wuninitialized -Wstrict-prototypes
CFLAGS := -mmcu=atmega32u2 -Os  -g -I../drivers -I../fonts -I../drivers/avr -I../utils 
#-DENABLE_ASSERT $(WARNINGS)
OBJCOPY := avr-objcopy
SIZE := avr-size 
#--mcu=atmega32u2 -C
DEL := rm


# Default target.
all: game.out


# Our code
game.o: game.c main_loops.h game_state.h vector.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

main_loops.o: main_loops.c main_loops.h game_state.h debug.h main_loop_utils.h comm.h draw.h input.h my_task.h ../drivers/avr/system.h ../drivers/avr/pio.h ../utils/tinygl.h ../fonts/font3x5_1.h ../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

game_state.o: game_state.c vector.h debug.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

vector.o: vector.c vector.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

debug.o: debug.c debug.h ../drivers/avr/timer0.h ../drivers/avr/ir_uart.h ../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

main_loop_utils.o: main_loop_utils.c main_loop_utils.h game_state.h bool.h debug.h main_loops.h game_state.h comm.h draw.h input.h ../drivers/button.h ../drivers/avr/timer.h ../utils/tinygl.h ../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

comm.o: comm.c comm.h vector.h game_state.h bool.h debug.h ../drivers/avr/system.h ../drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@

draw.o: draw.c draw.h game_state.h debug.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

input.o: input.c input.h game_state.h ../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

my_task.o: my_task.c my_task.h game_state.h ../drivers/avr/system.h ../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

my_pacer.o: my_pacer.c my_pacer.h ../drivers/avr/system.h ../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

# Library code
pio.o: ../drivers/avr/pio.c ../drivers/avr/pio.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
system.o: ../drivers/avr/system.c ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
timer.o: ../drivers/avr/timer.c ../drivers/avr/system.h ../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@
display.o: ../drivers/display.c ../drivers/avr/system.h ../drivers/display.h ../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@
ledmat.o: ../drivers/ledmat.c ../drivers/avr/pio.h ../drivers/avr/system.h ../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@
navswitch.o: ../drivers/navswitch.c ../drivers/avr/delay.h ../drivers/avr/pio.h ../drivers/avr/system.h ../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@
font.o: ../utils/font.c ../drivers/avr/system.h ../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@
tinygl.o: ../utils/tinygl.c ../drivers/avr/system.h ../drivers/display.h ../utils/font.h ../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@
ir_uart.o: ../drivers/avr/ir_uart.c ../drivers/avr/ir_uart.h ../drivers/avr/system.h ../drivers/avr/usart1.h ../drivers/avr/timer0.h ../drivers/avr/pio.h 
	$(CC) -c $(CFLAGS) $< -o $@
usart1.o: ../drivers/avr/usart1.c ../drivers/avr/usart1.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
timer0.o: ../drivers/avr/timer0.c ../drivers/avr/timer0.h ../drivers/avr/system.h ../drivers/avr/bits.h ../drivers/avr/prescale.h
	$(CC) -c $(CFLAGS) $< -o $@
prescale.o: ../drivers/avr/prescale.c ../drivers/avr/prescale.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@
button.o: ../drivers/button.c ../drivers/button.h ../drivers/avr/system.h ../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: game.o main_loops.o game_state.o vector.o debug.o main_loop_utils.o comm.o draw.o input.o my_task.o my_pacer.o pio.o system.o timer.o display.o ledmat.o navswitch.o font.o tinygl.o ir_uart.o usart1.o timer0.o prescale.o button.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase
	dfu-programmer atmega32u2 flash game.hex
	dfu-programmer atmega32u2 start


