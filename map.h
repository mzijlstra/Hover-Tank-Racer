#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include <stdio.h>
#include <GL/gl.h>

class Map {
	private:
		int w;
		int h;
	
	public:
		Map(int w, int h);
		void display(float x, float y, float w, float h, float zoom);
		int getW();
		int getH();
};
#endif // MAP_H_INCLUDED
