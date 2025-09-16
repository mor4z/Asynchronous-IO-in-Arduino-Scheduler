CC=avr-gcc
AS=avr-gcc
INCLUDE_DIRS=-I.
CC_OPTS=-Wall --std=gnu99 -DF_CPU=16000000UL -O3 -funsigned-char -funsigned-bitfields -fshort-enums \
         -Wall -Wstrict-prototypes -mmcu=atmega2560 $(INCLUDE_DIRS) -D__AVR_3_BYTE_PC__
AS_OPTS=-x assembler-with-cpp $(CC_OPTS)

AVRDUDE=avrdude

# Porta seriale per Arduino Mega 2560
AVRDUDE_PORT = /dev/ttyACM0

# Programmatore e parametri upload
AVRDUDE_FLAGS = -p m2560 -P $(AVRDUDE_PORT) -c wiring -b 115200
AVRDUDE_FLAGS += -D -q -V -C /etc/avrdude.conf

# Oggetti del progetto
OBJS = uart.o \
       functions.o

# Header del progetto (solo per dipendenze, non vengono compilati)
HEADERS = uart.h \
          functions.h

# Binari da generare
BINS = main.elf

.PHONY: all clean

all: $(BINS) $(BINS:.elf=.hex)

# Compilazione file .c → .o
%.o: %.c $(HEADERS)
	$(CC) $(CC_OPTS) -c $< -o $@

%.o: %.s
	$(AS) $(AS_OPTS) -c $< -o $@

# Linking → ELF
%.elf: %.o $(OBJS)
	$(CC) $(CC_OPTS) -o $@ $< $(OBJS) $(LIBS)

# ELF → HEX + Upload
%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U flash:w:$@:i

clean:
	rm -rf $(OBJS) $(BINS) *.hex *.o *~
