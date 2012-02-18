# Compiler
CC=g++

# Compiler flags
CPPFLAGS=-Wall -g `sdl-config --cflags`

# Include flags
INCFLAGS=-I/usr/include/GL -I/usr/include/SDL

# Linker flags
LDFLAGS=`sdl-config --libs` -lSDL -lSDL_image -lGL -lGLU 

main:
	$(CC) $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS) -o client client.cpp tank.cpp map.cpp cursor.cpp


# Windows cross compiler
WIN=i586-mingw32msvc-g++

# Include flags
#WININC=-I/media/46A87E61A87E4F83/Program\ Files\ \(x86\)/CodeBlocks/MinGW/include -I/media/46A87E61A87E4F83/Program\ Files\ \(x86\)/CodeBlocks/MinGW/include/GL -I/media/46A87E61A87E4F83/SDL-1.2.14/include

WININC=-I/usr/i586-mingw32msvc/include/SDL -I/usr/i586-mingw32msvc/include/GL

# Linker flags
#WINLD=-lmingw32 -lSDLmain -lSDL -lSDL_image -lGL -lGLU 
WINLD=-lmingw32 -lSDLmain -lSDL -lSDL_image -lopengl32 -lglu32 

windows:
	$(WIN) $(CPPFLAGS) $(WININC) $(WINLD) -o client.exe client.cpp tank.cpp map.cpp

# cleanup
clean:
	rm client

# EOF
