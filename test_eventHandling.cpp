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
		xRot++;
	} 
	if (keys[SDLK_LEFT]) {
		xRot--;
	}
	if (keys[SDLK_DOWN]) {
		yRot++;
	}
	if (keys[SDLK_UP]) {
		yRot--;
	}
	if (keys[SDLK_COMMA]) {
		zoom -= 0.01;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width * zoom, height * zoom, 0, 1000, -1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//printf("zoom: %f\n", zoom);
	}
	if (keys[SDLK_PERIOD]) {
		zoom += 0.01;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width * zoom, height * zoom, 0, 1000, -1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		//printf("zoom: %f\n", zoom);
	}

	/* player controll */
	if (keys[SDLK_w]) { p1->pushForward(); }
	if (keys[SDLK_s]) { p1->pushBackward(); }
	if (keys[SDLK_a]) { p1->pushLeft(); }
	if (keys[SDLK_d]) { p1->pushRight(); }
	if (keys[SDLK_q]) { p1->turnLeft(); }
	if (keys[SDLK_e]) { p1->turnRight(); }

	/* close program */
	if (keys[SDLK_ESCAPE] ) {
		running = false;
	}
}

