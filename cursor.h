#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

#include <SDL/SDL_platform.h>

#if defined( __WIN32__ )
	#include <windows.h>
#endif

#include <stdio.h>
#include <GL/gl.h>

class Cursor {
	private:
		int x;
		int y;
		float rotation;
		GLuint texture;
	
	public:
		Cursor(int w, int h, float rot, GLuint texture);
		~Cursor();
		void display();

		// getter and setters
		int getX();
		int getY();
		float getRot();
		void setX(int x);
		void setY(int y);
		void setRot(float rot);
};
#endif // CURSOR_H_INCLUDED
