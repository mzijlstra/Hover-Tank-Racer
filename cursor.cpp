#include "cursor.h"

Cursor::Cursor(int x, int y, float rot, GLuint texture) {
	this->x = x;
	this->y = y;
	this->rotation = rot;
	this-> texture = texture;
}

Cursor::~Cursor() {
	glDeleteTextures(1, &texture);
}

void Cursor::display() {
	glPushMatrix();

	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
		// triangle points up by default, we want it to point right
		// applied texture so that it indeed points right
		glTexCoord2i(1, 0); glVertex3f(-8.0, -8.0, 5.0);
		glTexCoord2i(1, 1); glVertex3f( 8.0, -8.0, 5.0);
		glTexCoord2i(0, 1); glVertex3f( 8.0,  8.0, 5.0);
		glTexCoord2i(0, 0); glVertex3f(-8.0,  8.0, 5.0);
	glEnd();

	glPopMatrix();
}

int Cursor::getX() {
	return x;
}

int Cursor::getY() {
	return y;
}
float Cursor::getRot() {
	return rotation;
}
void Cursor::setX(int x) {
	this->x = x;
}
void Cursor::setY(int y) {
	this->y = y;
}
void Cursor::setRot(float rot) {
	this->rotation = rot;
}
