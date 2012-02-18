#include "test.h"

void Test::onEvent(SDL_Event* event) {
	switch (event->type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_VIDEORESIZE:
			onResize(event->resize.w, event->resize.h);
			break;
		case SDL_KEYDOWN:
			keys[event->key.keysym.sym] = true;
			break;
		case SDL_KEYUP:
			keys[event->key.keysym.sym] = false;
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------
void Test::onInput() {
	/* viewport modification stuffs */
	if (keys[SDLK_RIGHT]) {
		if (xView < map->getW() - wView*zoom) { xView++; }
	}
	if (keys[SDLK_LEFT]) {
		if (xView > 0) { xView--; }
	}
	if (keys[SDLK_DOWN]) {
		if (yView < map->getH() - hView*zoom) { yView++; }
	}
	if (keys[SDLK_UP]) {
		if (yView > 0) { yView--; }
	}
	if (keys[SDLK_COMMA]) {
		zoom -= 0.01;
		onZoom();
	}
	if (keys[SDLK_PERIOD]) {
		zoom += 0.01;
		onZoom();
	}
	if (keys[SDLK_KP0]) {
		setXView(p1->getX() - wView / 2);
		setYView(p1->getY() - hView / 2);
	}

	/* player controll */
	if (keys[SDLK_w]) { p1->pushForward(); }
	if (keys[SDLK_s]) { p1->pushBackward(); }
	if (keys[SDLK_a]) { p1->pushLeft(); }
	if (keys[SDLK_d]) { p1->pushRight(); }
	if (keys[SDLK_q]) { p1->turnLeft(); }
	if (keys[SDLK_e]) { p1->turnRight(); }
	if (keys[SDLK_SPACE]) { p1->straighten(); }


	/* close program */
	if (keys[SDLK_ESCAPE] ) {
		running = false;
	}
}

