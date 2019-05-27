####################################################################

# Makefile
# Definitions
#
CC = gcc
CFLAGS = -g -Wall -std=c99
CCLINK = $(CC)
LIBS = -lm
OBJS = main.o partition.o gentree.o
RM = rm -f
TARGET = partition

#
# Target Section
#
$(TARGET): $(OBJS)
	$(CCLINK) -o $(TARGET) $(OBJS) $(LIBS)

#
# File Section
#
main.o: main.c partition.h
partition.o: partition.c gentree.h defs.h
gentree.o: gentree.c gentree.h defs.h

#
# Cleaning
#
clean:
	$(RM) $(TARGET) *.o *.bak *~"#"* core