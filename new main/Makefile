CC=avr-gcc
AS=avr-gcc
INCLUDE_DIRS=-I. -Imy_functions
CC_OPTS=-Wall --std=gnu99 -DF_CPU=16000000UL -O3 -funsigned-char -funsigned-bitfields  -fshort-enums -Wall -Wstrict-prototypes -mmcu=atmega2560 $(INCLUDE_DIRS)  -D__AVR_3_BYTE_PC__
AS_OPTS=-x assembler-with-cpp $(CC_OPTS)

AVRDUDE=avrdude

AVRDUDE_PORT = /dev/ttyACM0    # programmer connected to serial device

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET):i
AVRDUDE_FLAGS = -p m2560 -P $(AVRDUDE_PORT) -c $(AVRDUDE_PROGRAMMER) -b 115200
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)
AVRDUDE_FLAGS += -D -q -V -C /etc/avrdude.conf
AVRDUDE_FLAGS += -c wiring

OBJS=atomport_asm.o\
     scheduler.o\
     tcb.o\
     tcb_list.o\
     timer.o\
     uart.o\
     my_functions/buf.o\
     my_functions/functions.o\

HEADERS=atomport_asm.h\
        scheduler.h\
        tcb.h\
        tcb_list.h\
        timer.h\
        uart.h\
        my_functions/buf.h\
        my_functions/functions.h\

BINS= main.elf #tcb_queue_test.elf

.phony: clean all

all: $(BINS) 

#common objects
%.o: %.c 
	$(CC) $(CC_OPTS) -c  $<

%.o: %.s 
	$(AS) $(AS_OPTS) -c  $<

my_functions/%.o: my_functions/%.c
	$(CC) $(CC_OPTS) -c $< -o $@

%.elf: %.o $(OBJS)
	$(CC) $(CC_OPTS) -o $@ $< $(OBJS) $(LIBS)

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$@:i

clean:
	rm -rf $(OBJS) $(BINS) *.hex *~ *.o
