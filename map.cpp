#include "map.h"

Map::Map(int w, int h) {
	this->w = w;
	this->h = h;
}

void Map::display(float x, float y, float w, float h, float zoom) {
	glColor3f(0.0, 1.0, 0.0);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x00ff);
	glBegin(GL_LINES);

	int xRemainder = (int)x % 64;
	int yRemainder = (int)y % 64;

	for (int i = 0; i < w * zoom && i < this->w; i++) {
		if ((xRemainder+ i) % 64 == 0) {
			glVertex3f(i, 0, 1);
			glVertex3f(i, h * zoom, 1);
		}
	}

	for (int i = 0; i < h * zoom && i < this->h; i++) {
		if ((yRemainder + i) % 64 == 0) {
			glVertex3f(0, i, 1);
			glVertex3f(w * zoom, i, 1);
		}
	}

	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

int Map::getW() {
	return w;
}

int Map::getH() {
	return h;
}
