
#ifndef TANK_H_INCLUDED
#define TANK_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>

#include "map.h"

#define TORAD(X) X*(M_PI/180)
#define TODEG(X) X*(180/M_PI)

class Tank {
	private:
		// map pointer
		Map* map;

		// size
		int w;
		int h;

		// position
		float x;
		float y;
		float z;

		// rotation
		float xRot;
		float yRot;
		float zRot;

		// velocity
		float xVel;
		float yVel;
		float zVel;
		float zRotVel;

		// forces
		float forwardFrc;
		float breakFrc;
		float strafeFrc;
		float turnFrc;

		// gun related
		float gunRot;
		float gunAngle;
		float gunPow;
		float gunTurnSpd;
		float gunRaiseSpd;
		float gunPowChgSpd;

	public:
		// System methods
		Tank(Map* map, int x, int y);
		void display(float xView, float yView);
		void onUpdate();

		// driver interface
		void pushForward();
		void pushBackward();
		void pushRight();
		void pushLeft();
		void turnLeft();
		void turnRight();
		void straighten();

		// gunner interface
		void gunLeft();
		void gunRight();
		void gunUp();
		void gunDown();
		void powerIncrease();
		void powerDecrease();
		void fire();

		// getters
		float getX();
		float getY();
		
	private:
		void push(float angle, float force);
		void turn(float angle);
		void applyDrag(float* vel);
		void rotateGun(float angle);
		void raiseGun(float angle);
		void changePow(float force);
};

#endif // TANK_H_INCLUDED
