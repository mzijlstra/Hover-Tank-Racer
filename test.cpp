#include "test.h"
#include <GL/gl.h>

//==============================================================================
Test::Test() {
	// system variables
    display = NULL;
    running = true;
	for (int i=0; i < 322; i++) {
		keys[i] = false;
	}

	// view port variables
	wView = 640;
	hView = 480;
	xView = 0;
	yView = 0;
	zoom = 1.0;

	// map variables
	map =NULL; 
	p1 = NULL;
}

//------------------------------------------------------------------------------
int Test::onExecute() {
    if(onInit() == false) {
        return -1;
    }
	map = new Map(4096, 4096);
	p1 = new Tank(map, 100, 100);

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
		setXView(p1->getX() - (wView * zoom) / 2);
		setYView(p1->getY() - (hView * zoom) / 2);


        onRender();

		exectime = SDL_GetTicks() - starttime;
		if (exectime < frametime) {
			//SDL_Delay(SDL_GetTicks() % frametime);
			SDL_Delay(frametime - exectime);
		}
    }

	delete p1;
	delete map;
    onCleanup();

    return 0;
}

//------------------------------------------------------------------------------
bool Test::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return false;
    }

	if (   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1) != 0

		|| SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,    32) != 0
		|| SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,     24) != 0

		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,  8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8) != 0 ) 
	{
		printf("SDL OpenGL setup problem\n");
	}

/*
	int l = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
	int m = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  4);
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
	glEnable(0x809D);  // GL_MULTISAMPLE Why does the include not find it???
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
