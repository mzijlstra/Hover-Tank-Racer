#include "test.h"

void Test::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	map->display(xView, yView, wView, hView, zoom);
	p1->display(xView, yView);

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
	// -100 z for near is actually +100
	glOrtho(0, w * zoom, h * zoom, 0, -100, 0);
	//glFrustum(0, (w * zoom) / 100, (h * zoom) / 100, 0, 100, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set global view width and hight
	wView = w;
	hView = h;

	//printf("resizing: %d %d\n", w, h);
}

void Test::onZoom() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// -100 z for near is actually +100
	glOrtho(0, wView * zoom, hView * zoom, 0, -100, 0);
	//glFrustum(0, (wView * zoom)  / 100, (hView * zoom) / 100, 0, 100, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Test::setXView(float x) {
	xView = x; //p1->getX() - wView / 2;
	if (xView < 0) xView = 0;
	if (xView > map->getW() - wView) xView = map->getW() - wView;
}

void Test::setYView(float y) {
	yView = y; //p1->getY() - hView / 2;
	if (yView < 0) yView = 0;
	if (yView > map->getH() - hView) yView = map->getH() - hView;
}
