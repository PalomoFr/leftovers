CC=gcc 
CFLAGS=-Wall -Wextra -std=c99 -O0 -g `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0` -I.

APP=./main_gui

ODIR=build

DEPS = img_edit_gui.h

_OBJ = img_edit_gui.o main_gui.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	$(CC) -o $(APP) $^ $(CFLAGS)

run: $(OBJ) build
	$(APP)

.PHONY: clean default

clean:
	rm -f $(ODIR)/*.o

default: clean
	rm -f $(APP)