# Compiler settings
CC=avr-gcc
AS=avr-gcc
INCLUDE_DIRS=-I.
CC_OPTS=-Wall --std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fshort-enums -Wall -Wstrict-prototypes -mmcu=atmega328p $(INCLUDE_DIRS)
AS_OPTS=-x assembler-with-cpp $(CC_OPTS)

# Avrdude settings
AVRDUDE=avrdude
AVRDUDE_PORT=/dev/ttyUSB0   # cambia in /dev/ttyACM0 se serve
AVRDUDE_FLAGS=-p m328p -P $(AVRDUDE_PORT) -c arduino -b 115200
AVRDUDE_FLAGS+=-D -q -V -C /etc/avrdude.conf

# Objects and headers
OBJS=uart.o\
     tcb.o\
     tcb_list.o\
     atomport_asm.o\
     timer.o\
     scheduler.o\
     functions.o\

HEADERS=uart.h\
        tcb.h\
        tcb_list.h\
        atomport_asm.h\
        timer.h\
        scheduler.h\
        functions.h\

BINS=main.elf

.PHONY: clean all

all: $(BINS)

# Common objects
%.o: %.c
	$(CC) $(CC_OPTS) -c $<

%.o: %.s
	$(AS) $(AS_OPTS) -c $<

%.elf: %.o $(OBJS)
	$(CC) $(CC_OPTS) -o $@ $< $(OBJS) $(LIBS)

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$@:i

clean:
	rm -rf $(OBJS) $(BINS) *.hex *~ *.o
