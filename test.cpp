#include "test.h"

//==============================================================================
Test::Test() {
    display = NULL;
    running = true;
	xRot = yRot = zRot = 0;
	zoom = 1.0;
	for (int i=0; i < 322; i++) {
		keys[i] = false;
	}

	p1 = NULL;
}

//------------------------------------------------------------------------------
int Test::onExecute() {
    if(onInit() == false) {
        return -1;
    }
	p1 = new Tank(100, 100);

    SDL_Event event;
	Uint32 fps = 60;
	Uint32 frametime = 1000 / fps;
	Uint32 starttime, exectime;

    while(running) {
		starttime = SDL_GetTicks();

        while(SDL_PollEvent(&event)) {
            onEvent(&event);
        }

        onInput();

		// TODO more game state updates here, 
		// like applying velocity, gravity, etc
		p1->onUpdate();

        onRender();

		exectime = SDL_GetTicks() - starttime;
		if (exectime < frametime) {
			SDL_Delay(SDL_GetTicks() % frametime);
		}
    }

	delete p1;
    onCleanup();

    return 0;
}

//------------------------------------------------------------------------------
bool Test::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

	int a = SDL_GL_SetAttribute(SDL_GL_RED_SIZE,            8);
	int b = SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,          8);
	int c = SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,           8);
	int d = SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,          8);
	int e = SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,         32);
	int f = SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,          24);

	int k = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  0);
	int l = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  0);

/*
	int g = SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,      8);
	int h = SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
	int i = SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,     8);
	int j = SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

	int m = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,        1);
*/
	onResize(640, 480);
    glClearColor(1.0, 1.0, 1.0, 0);
	glEnable(GL_DEPTH_TEST);
/*	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
*/
	glEnable(GL_MULTISAMPLE);
    return true;
}

//------------------------------------------------------------------------------
void Test::onCleanup() {
    SDL_FreeSurface(display);
    SDL_Quit();
}

//==============================================================================
int main(int argc, char* argv[]) {
    Test test;
    return test.onExecute();
}

//==============================================================================
