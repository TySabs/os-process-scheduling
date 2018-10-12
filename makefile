##
# PROGRAM:    Assignment 4
# PROGRAMMER: Tyler Saballus
# LOGON ID:   z1836409
# DATE DUE:   10-12-2018
#

# Compiler variables
CC = g++
CCFLAGS = -Wall

# Rule to link object code files to create executable file
Assign4: hw4.o process.o
	$(CC) $(CCFLAGS) -o Assign4 hw4.o process.cc

# Rules to compile source code file to object code
hw4.o: hw4.cc
	$(CC) $(CCFLAGS) -c hw4.cc

process.o: process.cc process.h
	$(CC) $(CCFLAGS) -c process.cc

# Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign4
