#include "test.h"

void Test::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//glColor3f(1.0f, 1.0f, 1.0f);
	//glRectf(25.0f, 75.0f, 75.0f, 25.0f);

/*    glLoadIdentity();

    glBegin(GL_QUADS);
        glColor3f(1, 0, 0); glVertex3f(x, y, z);
        glColor3f(1, 1, 0); glVertex3f(x + w, y, z);
        glColor3f(1, 0, 1); glVertex3f(x + w, y + h, z);
        glColor3f(1, 1, 1); glVertex3f(x, y + h, z);
    glEnd();
*/

	
/*	GLfloat x, y, z, angle;

	glPushMatrix();
	glTranslatef(150.0f, 150.0f, 0);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	z = 0.0f;
	glBegin(GL_LINE_STRIP);
	for (angle = 0.0f; angle < (2.0f * M_PI)* 3.0f; angle += 0.1f)
	{
		x = 50.0f * sin(angle);
		y = 50.0f * cos(angle);

		glVertex3f(x, y, z);

		z += 0.5f;
	}
	glEnd();

	glPopMatrix();
*/
	p1->display();

    SDL_GL_SwapBuffers();
}

//------------------------------------------------------------------------------
void Test::onResize(int w, int h) {
	// never less than 640x480
	if (w < 640) w = 640;
	if (h < 480) h = 480;

    SDL_FreeSurface(display);
    display = SDL_SetVideoMode(w, h, 32, 
			SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_RESIZABLE);

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	glOrtho(0, w * zoom, h * zoom, 0, 1000, -1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set global width and hight
	width = w;
	height = h;

	//printf("resizing: %d %d\n", w, h);
}

