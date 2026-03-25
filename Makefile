## Makefile for sillyPaint
## Very simple

CC=gcc
CFLAGS=-std=c99 -Wall -Wextra -pedantic
LDFLAGS=-L. -lSDL3 -Wl,-rpath,'$$ORIGIN'

CPPFLAGS=-g

BUILD=build
OBJ=build/obj

SOURCE=$(SRC)/controller.c \
	$(SRC)/linkedList.c \
	$(SRC)/cell.c \
	$(SRC)/randomSand.c \
	$(SRC)/sillyPaint.c 

OBJECTS=$(OBJ)/controller.o \
	$(OBJ)/linkedList.o \
	$(OBJ)/cell.o \
	$(OBJ)/randomSand.o \
	$(OBJ)/sillyPaint.o

SRC=src

OPTIM=-O1

LIB=-L./lib
INC=-I./include

EXEC=sillyPaint

.PHONY: all clean O1 O2 O3

all: $(EXEC)

clean:
	rm -rf $(EXEC) $(BUILD)

O1:
	$(OPTIM)=-O1

O2:
	$(OPTIM)=-O2

O3:
	$(OPTIM)=-O3

$(BUILD) $(OBJ):
	mkdir -p $@

$(OBJ)/%.o : $(SRC)/%.c | $(OBJ)
	$(CC) $(OPTIM) $(CFLAGS) -c $< -o $@ $(INC)

sillyPaint : $(OBJECTS) | $(OBJ)
	$(CC) $(CPPFLAGS) $(OBJECTS) $(LIB) $(LDFLAGS) -lSDL3 -o sillyPaint
