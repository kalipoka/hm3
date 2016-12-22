# makefile for airport

CC = gcc
CFLAGs = -g - Wall
CCLINK = $(CC)
LIBS =
OBJS = graph.o set.o list.o
TARGET = robommt
RM = rm -f

$(TARGET): $(OBJS)
	$(CCLINK) -o $(TARGET) $(OBJS) $(LIBS)
	
# Creating object files using default rules
	
list.o: list.c list.h
set.o: set.c set.h list.h
graph.o: graph.h graph.c set.h list.h
main.o: main.c graph.h set.h set.h

#Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *.bak *~ "#"* core

