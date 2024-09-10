CC=gcc
CXX=g++
CCOPTS=-std=gnu99
BINS=serial_linux

.phony: clean all

all: $(BINS)  

serial_linux:	serial_linux.cpp
	$(CXX) -$(CCOPTS) -o $@ $^  

clean:
	rm -rf *~ $(BINS) *.pgm

