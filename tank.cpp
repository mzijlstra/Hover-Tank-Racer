#include "tank.h"

//==============================================================================
/* System functions */
Tank::Tank(Map* map, GLuint texture, int x, int y) {
	this->map = map;
	this->texture = texture;
	w = h = 32;
	this->x = x;
	this->y = y;
	z = 15;

	xRot = yRot = 0;
	zRot = 0; // face north

	xVel = yVel = zVel = zRotVel = 0; // not moving

	forwardFrc = 0.25;
	breakFrc = 0.25;
	strafeFrc = 0.25;
	turnFrc = 0.1;

	gunRot = 0;
	gunAngle = 45;
	gunPow = 50;
	gunTurnSpd = 1.0;
	gunRaiseSpd = 1.0;
	gunPowChgSpd = 1.0;
}

Tank::~Tank() {
	glDeleteTextures(1, &texture);
}

//------------------------------------------------------------------------------
void Tank::display() {

	glPushMatrix();

	glTranslatef(x, y, z);

    // first draw the velocity arrow
    float curVel   = sqrt(pow(yVel, 2) + pow(xVel, 2));
	if (curVel > 1.5) {
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();

		// enable line smoothing
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glEnable(GL_LINE_SMOOTH);

		float velAngle = TODEG(atan(yVel / xVel));
		if (xVel < 0) {
			velAngle += 180;
		}

		glRotatef(velAngle, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		float opacity = 1;

		glColor4f(1.0, 0.5, 0.0, opacity);
		glVertex3f(25, 0, 0);
		glColor4f(1.0, 0.0, 0.0, opacity);
		glVertex3f(25 + curVel* 3, 0, 0);

		/*	for (float i=1; i <= curVel / 5; i++) {
			opacity = 0.75 / i;

			glColor4f(1.0, 0.5, 0.0, opacity);
			glVertex3f(25, i, 0);
			glColor4f(1.0, 0.0, 0.0, opacity);
			glVertex3f(25 + curVel* 3, i, 0);

			glColor4f(1.0, 0.5, 0.0, opacity);
			glVertex3f(25, -i, 0);
			glColor4f(1.0, 0.0, 0.0, opacity);
			glVertex3f(25 + curVel* 3, -i, 0);
			}
			*/
		glEnd();

		glTranslatef(25 + curVel * 3.5, 0, 0);
		glRotatef(135, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		opacity = 1;

		glColor4f(1.0, 0.0, 0.0, opacity);
		glVertex3f(0, 0, 1);
		glColor4f(1.0, 0.5, 0.0, opacity);
		glVertex3f(curVel * 1, 0, 1);

		/*	for (int i = 1; i <= curVel / 10; i++) {
			opacity = 0.75 / i;

			glColor4f(1.0, 0.0, 0.0, opacity);
			glVertex3f(0, i, 1);
			glColor4f(1.0, 0.5, 0.0, opacity);
			glVertex3f(curVel * 1, i, 1);

			glColor4f(1.0, 0.0, 0.0, opacity);
			glVertex3f(0, -i, 1);
			glColor4f(1.0, 0.5, 0.0, opacity);
			glVertex3f(curVel * 1, -i, 1);
			}
			*/
		glEnd();

		glRotatef(90, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		opacity = 1;

		glColor4f(1.0, 0.0, 0.0, opacity);
		glVertex3f(0, 0, 1);
		glColor4f(1.0, 0.5, 0.0, opacity);
		glVertex3f(curVel * 1, 0, 1);
		/*
		   for (int i=1; i <= curVel / 10; i++) {
		   opacity = 0.75 / i;

		   glColor4f(1.0, 0.0, 0.0, opacity);
		   glVertex3f(0, i, 1);
		   glColor4f(1.0, 0.5, 0.0, opacity);
		   glVertex3f(curVel * 1, i, 1);

		   glColor4f(1.0, 0.0, 0.0, opacity);
		   glVertex3f(0, -i, 1);
		   glColor4f(1.0, 0.5, 0.0, opacity);
		   glVertex3f(curVel * 1, -i, 1);
		   }
		   */
		glEnd();
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
	}

    // then draw the tank
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glRotatef(zRot, 0.0f, 0.0f, 1.0f);

	glColor4f(1.0, 1.0, 1.0, 1.0); // white, full alpha body
	glBindTexture(GL_TEXTURE_2D, texture);

	glBegin(GL_QUADS);
		// bottom left corner
		glTexCoord2i(0, 0); glVertex3f(-w/2, -h/2, 0);
		// bottom right
		glTexCoord2i(1, 0); glVertex3f( w/2, -h/2, 0);
		// top right
		glTexCoord2i(1, 1); glVertex3f( w/2,  h/2, 0);
		// top left
		glTexCoord2i(0, 1); glVertex3f(-w/2,  h/2, 0);
	glEnd();

	glPopMatrix();
}

//------------------------------------------------------------------------------
void Tank::onUpdate() {
	if (x < 0 || x > map->getW()) { xVel = -xVel; x += xVel; }
	if (y < 0 || y > map->getH()) { yVel = -yVel; y += yVel; }

	x += xVel;
	y += yVel;
	zRot += zRotVel;

	float vel = sqrt(pow(yVel, 2) + pow(xVel, 2)); 
	float newVel = applyDrag(vel);
	//printf("%f %f\n", vel, newVel);

	if (newVel > 0.20) {
		if (fabs(xVel) > 0.1) {
			float xRatio = xVel / vel;
			xVel = newVel * xRatio;
		}
		if (fabs(yVel) > 0.1) {
			float yRatio = yVel / vel;
			yVel = newVel * yRatio;
		}
	} else {
		xVel = yVel = 0;
	}


	zRotVel = applyDrag(zRotVel);

/*	if (xRot != 0) {
		if (xRot < 0) {
			xRot += 0.5;
		} else {
			xRot -= 0.5;
		}
	}
*/
//	applyDrag(&xVel);
//	applyDrag(&yVel);


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
float Tank::applyDrag(float vel) {
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
/*
	if (fabs(vel) < 0.1) {
		vel = 0;
	} else {
		float drag = 0;
		if (vel < 3){
			drag = 0.01 * pow(vel, 2);
		} else {
			drag = 0.0001 * pow(vel, 4);
		}

		if (vel < 0) {
			vel += drag;
		} else {
			vel -= drag;
		}
	}
	return vel;
*/

	return vel * 0.99; // 1% drag per frame

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
	push(zRot - 90, strafeFrc);
/*	if (xRot > -30) {
		xRot -= 1;
	}
*/	
}

void Tank::pushLeft() {
	push(zRot + 90, strafeFrc);
/*	if (xRot < 30) {
		xRot += 1;
	}
*/
}

void Tank::turnLeft() {
	turn(turnFrc);
}

void Tank::turnRight() {
	turn(-turnFrc);
}

void Tank::straighten() {
	zRotVel = 0;
}

void Tank::setZRot(float angle) {
	this->zRot = angle;
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

