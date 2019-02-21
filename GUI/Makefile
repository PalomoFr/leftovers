CC = gcc 
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall -Wextra -std=c99 -o main -g `pkg-config --libs gtk+-3.0`
SRC = *.c -lm
main_gui : ${SRC}

ODIR=build

DEPS = img_edit_gui.h ImageEditing/image_edit.h

_OBJ = img_edit_gui.o main_gui.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/image_edit.o: ImageEditing/image_edit.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ) $(ODIR)/image_edit.o
	$(CC) -o $(APP) $^ $(CFLAGS)

run: $(OBJ) build
	$(APP)

.PHONY: clean default

clean:
	rm -f $(ODIR)/*.o

default: clean
	rm -f $(APP)
