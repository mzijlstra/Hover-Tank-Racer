# Compiler
CC=g++

# Compiler flags
CPPFLAGS=-Wall -g `sdl-config --cflags`

# Linker flags
LDFLAGS=`sdl-config --libs` -lSDL -lSDL_image -lGL

main: test.cpp
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o test test.cpp test_eventHandling.cpp test_graphics.cpp tank.cpp 

# cleanup
clean:
	rm test

# EOF
