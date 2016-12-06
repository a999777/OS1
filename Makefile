# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall 
CCLINK = $(CC)
OBJS = smash.o JobsAndInfo.o commands.o signals.o 
RM = rm -f
# Creating the  executable
smash: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
commands.o: commands.cpp commands.h JobsAndInfo.h 
JobsAndInfo.o: JobsAndInfo.cpp JobsAndInfo.h
smash.o: smash.cpp commands.h JobsAndInfo.h 
signals.o: signals.cpp signals.h
# Cleaning old files before new make
clean:
	$(RM) smash *.o *~ "#"* core.*

