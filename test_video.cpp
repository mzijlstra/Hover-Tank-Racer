#include "test.h"

void Test::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	p1->display();

    SDL_GL_SwapBuffers();
}

//------------------------------------------------------------------------------
void Test::onResize(int w, int h) {
	// never less than 640x480
	if (w < 640) w = 640;
	if (h < 480) h = 480;

    SDL_FreeSurface(display);
    display = SDL_SetVideoMode(w, h, 16, SDL_OPENGL | SDL_RESIZABLE);
	if (display == NULL) { printf("no display :|\n"); }

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

