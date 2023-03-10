CC = g++
CFLAGS = -Wall -std=c++17 -Iinclude
LDLIBS = -L/usr/X11R6/lib -lX11 -lXext
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
TARGET = fractal

all: $(TARGET)

obj:
	mkdir -p obj

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)

obj/%.o: src/%.cpp | obj
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ) $(TARGET)
