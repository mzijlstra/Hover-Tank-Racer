#include "test.h"

//==============================================================================
Test::Test() {
	// system variables
    display = NULL;
    running = true;
	for (int i=0; i < 322; i++) {
		keys[i] = false;
	}

	// view port variables
	wView   = 1024;
	hView   = 768;
	xView   = wView / 2;
	yView   = hView / 2;
	zoom    = 1.0;
	follow  = true;

	// map variables
	map     = NULL;
	p1      = NULL;
}

//------------------------------------------------------------------------------
int Test::onExecute() {
    if(onInit() == false) {
        return -1;
    }

    SDL_Event event;
	Uint32 fps = 60;
	Uint32 frametime = 1000 / fps;
	Uint32 starttime, exectime;

    while(running) {
		starttime = SDL_GetTicks();

		// start by checking for OpenGL error
		GLenum error;
		while ((error = glGetError()) != GL_NO_ERROR) {
			printf("Error: %s\n", gluErrorString(error));
		}

		// check for input events
        while(SDL_PollEvent(&event)) {
            onEvent(&event);
        }

		// act on input events
        onInput();

		// TODO more game state updates here,
		// like applying velocity, gravity, etc
		p1->onUpdate();
		if (follow) {
			setXView(p1->getX());
			setYView(p1->getY());
		}

		// draw the screen
        onRender();

		// sleep time to make fps
		exectime = SDL_GetTicks() - starttime;
		if (exectime < frametime) {
			//SDL_Delay(SDL_GetTicks() % frametime);
			SDL_Delay(frametime - exectime);
		}
    }

    onCleanup();

    return 0;
}

//------------------------------------------------------------------------------
bool Test::onInit() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("problem initializing SDL\n");
        return false;
    }

	if (   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1) != 0

		|| SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,    32) != 0
		|| SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,     16) != 0
/*
		// Anti-aliasing -- why won't this work under linux???
		|| SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1) != 0
		|| SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  2) != 0
*/
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,  8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8) != 0 )
	{
		printf("SDL OpenGL setup problem\n");
		return false;
	}

	// create window with given width and height
	onResize(wView, hView);

	// hide system cursor
	SDL_ShowCursor(SDL_DISABLE);

	// set white as the default clear / background color
    glClearColor(1.0, 1.0, 1.0, 0.0);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// enable 2d textures
	glEnable(GL_TEXTURE_2D);

	// enable blending (translucency / trasparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// GL_MULTISAMPLE Why does the include under windows not find it???
	//glEnable(0x809D);

	// load textures
	GLuint mapTexture    = 0;
	GLuint tankTexture   = 0;
	GLuint cursorTexture = 0;
	if (   (mapTexture  = loadTexture("./graphics/test-track.png")) < 0
		|| (tankTexture = loadTexture("./graphics/newtank2.png")) < 0
		|| (cursorTexture = loadTexture("./graphics/triangle.png")) < 0)
	{
		printf("problem loading textures\n");
		return false;
	}

	map = new Map(4096, 4096, mapTexture);
	p1 = new Tank(map, tankTexture, 100, 100);
	cursor = new Cursor(0, 0, 0, cursorTexture);

    return true;
}

GLuint Test::loadTexture(const char* file) {
	GLuint texture;		  // Handle to our texture object
	SDL_Surface* surface; // surface will tell us the details of the image
	GLenum texture_format;
	GLint  nOfColors;

    if((surface = IMG_Load(file)) == NULL) {
        char path[PATH_LEN];
        getcwd(path, PATH_LEN);
        printf("cwd: %s\n", path);
		printf("Can't open file: %s\n", file);
		printf("SDL Says: %s\n", SDL_GetError());
        return -1;
    }

	// check width is power of 2 (newer opengl doesn't need this)
	if ( (surface->w & (surface->w - 1)) != 0 ) {
		printf("warning: image width is not a power of 2\n");
	}

	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0 ) {
		printf("warning: image height is not a power of 2\n");
	}

	// get the number of channels in the SDL surface
	nOfColors = surface->format->BytesPerPixel;
	if (nOfColors == 4) {    // contains an alpha channel
		printf("texture has alpha\n");
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	} else if (nOfColors == 3) {    // no alpha channel
		printf("texture has no alpha\n");
		if (surface->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	} else {
		printf("Error: the image is not truecolor, this will probably break\n");
		SDL_FreeSurface(surface);
		return -2;
	}

	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &texture );
	printf("Texture: %d\n", texture);

	// Bind the texture object -- TODO do i need this???
	glBindTexture( GL_TEXTURE_2D, texture );

	// Set the texture's stretching properties
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// multiply texture with the underlying color
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Edit the texture object's image data using information in SDL_Surface
	glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface->pixels );

	// clean up the SDL surface
    SDL_FreeSurface(surface);

    return texture;
}

//------------------------------------------------------------------------------
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

			// toggle follow mode
			if (event->key.keysym.sym == SDLK_f) {
				follow = !follow;
			}
			break;
		case SDL_MOUSEMOTION:
			xMouse = event->motion.x;
			yMouse = event->motion.y;
			break;
		default:
			break;
	}
}

//------------------------------------------------------------------------------
void Test::onInput() {
	// update mouse pointer & p1 rotation
	float xBotLeft = xView - wView / 2;
	float yBotLeft = yView - hView / 2;
	float xPlayer = p1->getX() - xBotLeft;
	float yPlayer = p1->getY() - yBotLeft;
	float x = xPlayer - xMouse;
	float y = yPlayer - (hView - yMouse) ;
	float angle = 0;
	if (x != 0) {
		angle = TODEG(atan(y / x));
	} else  {
		if (y < 0) {
			angle = -90;
		} else {
			angle = 90;
		}
	}	
	if (x < 0) {
		angle += 180;
	}
	cursor->setX(xBotLeft + xMouse);
	cursor->setY(yBotLeft + hView - yMouse);
	//printf("%f, %f\n", xBotLeft + xMouse, yBotLeft + hView - yMouse);
	cursor->setRot(angle);
	p1->setZRot(180 + angle);

	/* viewport modification stuffs */
	if (keys[SDLK_RIGHT]) {
		if (xView < map->getW() - wView*zoom) { xView++; }
	}
	if (keys[SDLK_LEFT]) {
		if (xView > 0) { xView--; }
	}
	if (keys[SDLK_DOWN]) {
		if (yView < map->getH() - hView*zoom) { yView--; }
	}
	if (keys[SDLK_UP]) {
		if (yView > 0) { yView++; }
	}
	if (keys[SDLK_COMMA]) {
		zoom -= 0.01;
	}
	if (keys[SDLK_PERIOD]) {
		zoom += 0.01;
	}
	if (keys[SDLK_KP0]) {
		//setXView(p1->getX() - wView / 2);
		//setYView(p1->getY() - hView / 2);
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

//------------------------------------------------------------------------------
void Test::onRender() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// reset to identity matrix
	glLoadIdentity();

	// apply view transformation
	gluLookAt(xView, yView, (wView / 2) * zoom, // eye location
			xView, yView, 0, // looking at this point
			0, 1, 0); // up vector


	map->display();
	p1->display();
	cursor->display();
	//renderCursor();

    SDL_GL_SwapBuffers();
}

/* TODO I should probably make a cursor class at some point 
void Test::renderCursor() {
	// calculate angle between cursor and player
	float xBotLeft = xView - wView / 2;
	float yBotLeft = yView - hView / 2;

	float xPlayer = p1->getX() - xBotLeft;
	float yPlayer = p1->getY() - yBotLeft;

	float x = xPlayer - xMouse;
	float y = yPlayer - (hView - yMouse) ;
	float angle = 0;
	if (x != 0) {
		angle = TODEG(atan(y / x));
	} else  {
		if (y < 0) {
			angle = -90;
		} else {
			angle = 90;
		}
	}	
	if (x < 0) {
		angle += 180;
	}

	glPushMatrix();

	glTranslatef(xBotLeft + xMouse, yBotLeft + hView - yMouse, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, cursorTexture);
	glBegin(GL_QUADS);
		// triangle points up by default, we want it to point right
		// applied texture so that it indeed points right
		glTexCoord2i(1, 0); glVertex3f(-8.0, -8.0, 5.0);
		glTexCoord2i(1, 1); glVertex3f( 8.0, -8.0, 5.0);
		glTexCoord2i(0, 1); glVertex3f( 8.0,  8.0, 5.0);
		glTexCoord2i(0, 0); glVertex3f(-8.0,  8.0, 5.0);
	glEnd();

	glPopMatrix();
}
*/

//------------------------------------------------------------------------------
void Test::onResize(int w, int h) {
	// never less than 640x480
	if (w < 640) w = 640;
	if (h < 480) h = 480;

    SDL_FreeSurface(display);
    display = SDL_SetVideoMode(w, h, 32, SDL_OPENGL | SDL_RESIZABLE);
	if (display == NULL) { printf("no display :|\n"); }

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	float aspect = (float) h / (float) w;
	glFrustum(-1, 1, -aspect, aspect, 1.0, 65535); //16bit depth

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// set global view width and hight
	wView = w;
	hView = h;
}

void Test::setXView(float x) {
	xView = x;

	// check border conditions
	float wZoomed = wView / 2 * zoom;
	if (xView < wZoomed) xView = wZoomed;
	if (xView > map->getW() - wZoomed) xView = map->getW() - wZoomed;
}

void Test::setYView(float y) {
	yView = y;

	// check border conditions
	float hZoomed = hView / 2 * zoom;
	if (yView < hZoomed) yView = hZoomed;
	if (yView > map->getH() - hZoomed) yView = map->getH() - hZoomed;
}

//------------------------------------------------------------------------------
void Test::onCleanup() {
	delete p1;
	delete map;
    SDL_FreeSurface(display);
    SDL_Quit();
}

//==============================================================================
int main(int argc, char* argv[]) {
    Test test;
    return test.onExecute();
}

