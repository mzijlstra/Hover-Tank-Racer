#include "tank.h"

Tank::Tank(int x, int y) {
	w = h = 32;
	this->x = x;
	this->y = y;
	z = 100;

	xRot = yRot = 0;
	zRot = 0; // face north

	xVel = yVel = zVel = zRotVel = 0; // not moving

	forwardFrc = 1.0;
	breakFrc = 0.5;
	strafeFrc = 0.5;
	turnFrc = 0.5;

	gunRot = 0;
	gunAngle = 45;
	gunPow = 50;
	gunTurnSpd = 1.0;
	gunRaiseSpd = 1.0;
	gunPowChgSpd = 1.0;
}

void Tank::display() {
	glColor3f(0.0, 0.0, 1.0);

	glPushMatrix();

	glTranslatef(x, y, z);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glBegin(GL_QUADS);
		glVertex3f(-w/2, -h/2, 0);
		glVertex3f(w/2, -h/2, 0);
		glVertex3f(w/2, h/2, 0);
		glVertex3f(-w/2, h/2, 0);	

		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(w/2 - 7, -h/2 + 2, -1);
		glVertex3f(w/2 - 2, -h/2 + 2, -1);
		glVertex3f(w/2 - 2,  h/2 - 2, -1);
		glVertex3f(w/2 - 7,  h/2 - 2, -1);
	glEnd();

	glPopMatrix();
}

void Tank::onUpdate() {
	if (x < 0 || x > 640) { xVel = -xVel; }
	if (y < 0 || y > 480) { yVel = -yVel; }
	if (z < 0 || z > 100) { zVel = -zVel; }

	x += xVel;
	y += yVel;
	z += zVel;
	zRot += zRotVel;
	
	// TODO should be based on gravity / wind / atmospher etc
	float dragFrc = 0.1;

	reduceVel(&xVel, dragFrc);
	reduceVel(&yVel, dragFrc);
	reduceVel(&zVel, dragFrc);
	reduceVel(&zRotVel, dragFrc);
}

/* private functions (the important stuff) */
void Tank::push(float angle, float force) {
	xVel += cos(TORAD(angle)) * force; 
	yVel += sin(TORAD(angle)) * force;
}

void Tank::turn(float angle) {
	zRotVel = fmod(zRotVel + angle, 360);
};

void Tank::reduceVel(float* vel, float frc) {
	if (*vel == 0) {
		return;
	} else if (fabs(*vel) < frc) {
		*vel = 0;
	} else {
		if (*vel < 0) {
			*vel += frc;
		} else {
			*vel -= frc;
		}
	}
}

void Tank::rotateGun(float angle) {
	gunRot += angle;
}

void Tank::raiseGun(float angle) {
	gunAngle += angle;
}

void Tank::changePow(float force) {
	gunPow += force;
}


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
	reduceVel(&zRotVel, turnFrc);
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
