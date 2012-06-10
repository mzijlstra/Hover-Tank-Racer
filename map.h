#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <SDL/SDL_platform.h>

#if defined( __WIN32__ )
	#include <windows.h>
#endif

#include <stdio.h>
#include <GL/gl.h>

class Map {
	private:
		int w;
		int h;
		GLuint texture;
	
	public:
		Map(int w, int h, GLuint texture);
		~Map();
		void display();
		int getW();
		int getH();
};
#endif // MAP_H_INCLUDED
