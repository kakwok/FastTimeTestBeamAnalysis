CC=g++
CFLAGS=-I.
DEPS = H4tree.h
ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

CXXFLAGS  = -g -Wall `root-config --cflags` -fPIC -O2 -ggdb -std=gnu++0x

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

Baseline : Baseline.o ${DEPS}
	$(CC) -o $@ $^ $(CFLAGS) $(ROOTFLAGS) $(ROOTLIBS)
