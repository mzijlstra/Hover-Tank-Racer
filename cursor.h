#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

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
