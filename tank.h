
#ifndef TANK_H_INCLUDED
#define TANK_H_INCLUDED

#define _USE_MATH_DEFINES

#include <SDL/SDL_platform.h>

#if defined( __WIN32__ )
	#include <windows.h>
#endif

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include "map.h"
#include "3dsloader.h"

#define TORAD(X) X*(M_PI/180)
#define TODEG(X) X*(180/M_PI)
#define HYP(X, Y) sqrt(pow(x, 2) + pow(y, 2))

struct vector {
    float x;
    float y;
    float z;
};

class Tank {
	private:
		// map pointer
		Map* map;

		// textures
//		GLuint baseTex;
//		GLuint engFireTex;
//		GLuint gunTex;

		// size
		int w;
		int h;

        bool engFire;
        float engPow;
        float gunRot;

        // TODO work on this
        float gunTilt;
        float gunPow;

        struct vector pos;
        struct vector rot;
        struct vector vel;

		obj_type object;

	public:
		// System methods
		Tank(Map* map, int x, int y);
		~Tank();
		void display();
		void onUpdate();

		void engageEngine(float angle);
		void setBaseRot(float angle);
		void setGunRot(float angle);

		// getters
		float getX();
		float getY();
		obj_type_ptr getObject() { return &object; }

	private:
		float applyDrag(float vel);
};

#endif // TANK_H_INCLUDED
