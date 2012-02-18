#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "tank.h"

class Test {
	private:
		bool            running;
		SDL_Surface*    display;
		bool            keys[322];
		float           width;
		float           height;
		float           zoom;
		float           xRot;
		float           yRot;
		float           zRot;

		Tank*           p1;

	// first four methods (constructor) in test.cpp
	public:
		Test();
		int onExecute();                

	private:
		bool onInit();
		void onCleanup();
		// onEvent and onInput in eventHandling.cpp
		void onEvent(SDL_Event* event); 
		void onInput();
		// onRender and onResize in graphics.cpp
		void onRender();
		void onResize(int w, int h);
};

#endif // TEST_H_INCLUDED
