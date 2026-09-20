CC = gcc
CXX = g++
CFLAGS = -g -DSCI_LEXER -Wall -I../../../include -I../../../src -I../../../../lexilla/include \
  -I../../../../lexilla/lexlib -I ../../ -I../../vendor/termbox2/
CXXFLAGS = $(CFLAGS)

scintilla = ../../../bin/scintilla.a ../../../../lexilla/bin/liblexilla.a

all: semester 
semester.o: semester.c ; $(CC) $(CFLAGS) -c $<
semester: semester.o $(scintilla) ; $(CXX) $^ -o $@
clean: ; rm -f semester *.o
