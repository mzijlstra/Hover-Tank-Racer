#include "map.h"

Map::Map(int w, int h, GLuint texture) {
	this->w = w;
	this->h = h;
	this-> texture = texture;
}

Map::~Map() {
	glDeleteTextures(1, &texture);
}

void Map::display() {
	// This method could be optimized by calculating the part of the quad
	// that is going to be inside the clipping area, and then only drawing
	// a quad of that size, and only drawing stuff inside it
	glPushMatrix();

	// map itself is light grey
	glColor3f(0.99, 0.99, 0.99);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
		// bottom left corner
		glTexCoord2i(0, 0); glVertex3f(0.0f, 0.0f, 0.0f);
		// bottom right
		glTexCoord2i(1, 0); glVertex3f(   w, 0.0f, 0.0f);
		// top right
		glTexCoord2i(1, 1); glVertex3f(   w,    h, 0.0f);
		// top left
		glTexCoord2i(0, 1); glVertex3f(0.0f,    h, 0.0f);
	glEnd();
/*
	// draw green grid lines every 64 px
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);

	// vertical lines
	for (int i = 0; i < w; i += 64) {
		glVertex3f(i, 0, 1);
		glVertex3f(i, h, 1);
	}

	// horizontal lines
	for (int i = 0; i < h; i += 64) {
		glVertex3f(0, i, 1);
		glVertex3f(w, i, 1);
	}
	glEnd();
*/
	glPopMatrix();
}

int Map::getW() {
	return w;
}

int Map::getH() {
	return h;
}
