# libsdl2-dev libsdl2-image-dev
HEADER_DIR=src
SOURCE_DIR=src
BUILD_DIR=build

HEADERS=$(wildcard $(HEADER_DIR)/*.h)
SOURCES=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS=$(patsubst $(SOURCE_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.cpp=.o))

CC=g++
CFLAGS=-c -Wall -std=c++14 `sdl2-config --cflags` -I$(HEADER_DIR)
LFLAGS=`sdl2-config --libs` -lSDL2_image -lm
EXECUTABLE=game

all: directories $(SOURCES) $(EXECUTABLE)

directories:
	@mkdir -p $(BUILD_DIR)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LFLAGS) -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm $(BDIR)/*.o $(EXECUTABLE)
