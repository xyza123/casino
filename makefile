CC := g++
CFLAGS := -Wall -std=c++17 -O2 -v
ALLEGRO_LIBRARIES := allegro-5 allegro_image-5 allegro_font-5 allegro_ttf-5 allegro_dialog-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5
ALLEGRO_FLAGS := $(shell pkg-config --cflags --libs $(ALLEGRO_LIBRARIES)) -lallegro -lallegro_main
 
OUT:= game
SOURCE = Main.cpp GameWindow.cpp global.cpp Texas.cpp poker.cpp player.cpp
OBJ = Main.o GameWindow.o global.o Texas.o poker.o player.o

all: 
	$(CC) -c -g $(CFLAGS) $(SOURCE) $(ALLEGRO_FLAGS)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(ALLEGRO_FLAGS)
	rm $(OBJ)

.PHONY:clean
clean:
	rm $(OUT)