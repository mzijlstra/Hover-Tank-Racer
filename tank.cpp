#include "tank.h"

//==============================================================================
/* System functions */
Tank::Tank(Map* map, int x, int y) {
	this->map = map;
	w = h = 32;
	this->x = x;
	this->y = y;
	z = 5;

	xRot = yRot = 0;
	zRot = 0; // face north

	xVel = yVel = zVel = zRotVel = 0; // not moving

	forwardFrc = 1.0;
	breakFrc = 0.5;
	strafeFrc = 0.5;
	turnFrc = 0.1;

	gunRot = 0;
	gunAngle = 45;
	gunPow = 50;
	gunTurnSpd = 1.0;
	gunRaiseSpd = 1.0;
	gunPowChgSpd = 1.0;
}

//------------------------------------------------------------------------------
void Tank::display(float xView, float yView) {
	glColor3f(0.0, 0.0, 1.0);

	glPushMatrix();

	glTranslatef(x - xView, y - yView, z);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
		glVertex3f(-w/2, -h/2, 0);
		glVertex3f( w/2, -h/2, 0);
		glVertex3f( w/2,  h/2, 0);
		glVertex3f(-w/2,  h/2, 0);

		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(w/2 - 7, -h/2 + 2, 1);
		glVertex3f(w/2 - 2, -h/2 + 2, 1);
		glVertex3f(w/2 - 2,  h/2 - 2, 1);
		glVertex3f(w/2 - 7,  h/2 - 2, 1);
	glEnd();

	glPopMatrix();
}

//------------------------------------------------------------------------------
void Tank::onUpdate() {
	if (x < 0 || x > map->getW()) { xVel = -xVel; x += xVel; }
	if (y < 0 || y > map->getH()) { yVel = -yVel; y += yVel; }
	if (z < 0 || z > 100) { zVel = -zVel; }

	x += xVel;
	y += yVel;
	z += zVel;
	zRot += zRotVel;

	applyDrag(&xVel);
	applyDrag(&yVel);
	applyDrag(&zVel);
	applyDrag(&zRotVel);
}

//==============================================================================
/* private functions (the important stuff) */
void Tank::push(float angle, float force) {
	xVel += cos(TORAD(angle)) * force;
	yVel += sin(TORAD(angle)) * force;
}

//------------------------------------------------------------------------------
void Tank::turn(float angle) {
	zRotVel = fmod(zRotVel + angle, 360);
};

//------------------------------------------------------------------------------
void Tank::applyDrag(float* vel) {
	/*
	 * The drag equation as found on Wikipedia:
	 * FD = 1/2 pu^2 CD A
	 * Where:
	 *   FD is the force of drag
	 *   p  is the mass density of the fluid (air)
	 *   u  is the velocity of the object relative to the fluid
	 *   A  is the reference area (orhographic projection onto perpendicular)
	 *   CD is the drag coefficient - a dimensionless constant e.g. 0.25 - 0.45
	 *
	 * For us this mostly means it should be a quadratic equation
	 */

	// TODO would be cool to include atmosphere desity and reference area 
	if (fabs(*vel) < 0.09) {
		*vel = 0;
	} else {
		float drag = 0.01 * pow(*vel, 2);
		if (*vel < 0) {
			*vel += drag;
		} else { 
			*vel -= drag;
		}
	}
}

//------------------------------------------------------------------------------
void Tank::rotateGun(float angle) {
	gunRot += angle;
}

//------------------------------------------------------------------------------
void Tank::raiseGun(float angle) {
	gunAngle += angle;
}

//------------------------------------------------------------------------------
void Tank::changePow(float force) {
	gunPow += force;
}


//==============================================================================
/* public interface functions -- built ontop of pirvate functions */
void Tank::pushForward() {
	push(zRot, forwardFrc);
}

void Tank::pushBackward() {
	push(zRot + 180, breakFrc);
}

void Tank::pushRight() {
	push(zRot + 90, strafeFrc);
}

void Tank::pushLeft() {
	push(zRot - 90, strafeFrc);
}

void Tank::turnLeft() {
	turn(-turnFrc);
}

void Tank::turnRight() {
	turn(turnFrc);
}

void Tank::straighten() {
	zRotVel = 0;
}

void Tank::gunLeft() {
	rotateGun(gunTurnSpd);
}

void Tank::gunRight() {
	rotateGun(-gunTurnSpd);
}

void Tank::gunUp() {
	raiseGun(gunRaiseSpd);
}

void Tank::gunDown() {
	raiseGun(-gunRaiseSpd);
}

void Tank::powerIncrease() {
	changePow(gunPowChgSpd);
}

void Tank::powerDecrease() {
	changePow(-gunPowChgSpd);
}

void Tank::fire() {
	// TOOD make bullet etc
}

float Tank::getX() {
	return x;
}
float Tank::getY() {
	return y;
}

