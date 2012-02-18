#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include <SDL.h>
#include <SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "tank.h"
#include "map.h"

class Test {
	private:
		// system variables
		bool            running;
		SDL_Surface*    display;
		bool            keys[322];

		// view port variables
		float           wView;
		float           hView;
		float           xView;
		float           yView;
		float           zoom;

		// player variables
		Tank*           p1;
		Map*            map;

	public:
		Test();                         // test.cpp
		int onExecute();                // test.cpp

	private:
		bool onInit();                  // test.cpp
		void onCleanup();               // test.cpp
		void onEvent(SDL_Event* event); // eventHandling.cpp
		void onInput();                 // eventHandling.cpp
		void onRender();                // video.cpp
		void onResize(int w, int h);    // video.cpp
		void onZoom();                  // video.cpp
		void setXView(float x);
		void setYView(float y);
};

#endif // TEST_H_INCLUDED
