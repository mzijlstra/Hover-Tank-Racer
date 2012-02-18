#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_platform.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_opengl.h>

#if defined( __WIN32__ )
    #include <direct.h>
	#define PATH_LEN  _MAX_PATH
#elif defined( __LINUX__ )
	#include <unistd.h>
	#include <linux/limits.h>
	#define PATH_LEN  PATH_MAX
#endif

#include "tank.h"
#include "map.h"
#include "cursor.h"

class Client {
	private:
		// system variables
		bool            running;
		SDL_Surface*    display;
		bool            keys[322];
		bool            mbts[6];
		Cursor*         cursor;
		int             xMouse;
		int             yMouse;

		// view port variables
		float           wView;
		float           hView;
		float           xView;
		float           yView;
		float           zoom;
		bool            follow;

		// player variables
		Tank*           p1;
		Map*            map;

	public:
		Client();
		int onExecute();

	private:
		// system methods
		bool onInit();
		void onCleanup();
		void onEvent(SDL_Event* event);
		void onInput();
		void onRender();
		void onResize(int w, int h);

		// helper methods
		void setXView(float x);
		void setYView(float y);
		GLuint loadTexture(const char* file);
};

#endif // TEST_H_INCLUDED
