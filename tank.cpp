#include "tank.h"

//==============================================================================
/* System functions */

//---------Constructor----------------------------------------------------------
Tank::Tank(Map* map, int x, int y, GLuint baseTex, GLuint engFireTex, GLuint gunTex) {
	this->map = map;
	this->baseTex = baseTex;
	this->engFireTex = engFireTex;
	this->gunTex = gunTex;

	w = h = 32; // 32x32 square
    engFire = false;
    engPow = 0.25;
    gunRot = 0;
    gunTilt = 45;
    gunPow = 20;

	pos.x = x;
	pos.y = y;
	pos.z = 2;

    rot.x = rot.y = rot.z = 0; // face north
	vel.x = vel.y = vel.z = 0; // not moving
}

//--------Destructor------------------------------------------------------------
Tank::~Tank() {
	glDeleteTextures(1, &baseTex);
	glDeleteTextures(1, &engFireTex);
	glDeleteTextures(1, &gunTex);
}

//------------------------------------------------------------------------------
void Tank::display() {

	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);

    // first draw the velocity arrow
    float curVel   = sqrt(pow(vel.y, 2) + pow(vel.x, 2));
	if (curVel > 0.2) {
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();

		// enable line smoothing
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glEnable(GL_LINE_SMOOTH);

		float velAngle = TODEG(atan(vel.y / vel.x));
		if (vel.x < 0) {
			velAngle += 180;
		}

		glRotatef(velAngle, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		float opacity = 1;

		glColor4f(0.0, 1.0, 0.0, opacity);
		glVertex3f(25, 0, 0);
		glColor4f(0.5, 1.0, 0.0, opacity);
		glVertex3f(25 + curVel* 3, 0, 0);

		glEnd();

		glTranslatef(25 + curVel * 3.5, 0, 0);
		glRotatef(135, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		opacity = 1;

		glColor4f(0.0, 1.0, 0.0, opacity);
		glVertex3f(0, 0, 1);
		glColor4f(0.5, 1.0, 0.0, opacity);
		glVertex3f(curVel * 1, 0, 1);

		glEnd();

		glRotatef(90, 0.0, 0.0, 1.0);
		glBegin(GL_LINES);
		opacity = 1;

		glColor4f(0.0, 1.0, 0.0, opacity);
		glVertex3f(0, 0, 1);
		glColor4f(0.5, 1.0, 0.0, opacity);
		glVertex3f(curVel * 1, 0, 1);

		glEnd();
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);
	}

    // draw targeting line
    if (gunPow > 0) {
        glPushMatrix();

        // TODO gravity should probably be defined in the map
        float gravity = 1.0;


        // where ax^2 + bx is trajectory with gravity
        // gunTilt => atan(b)
        // gunPow => hypotenus (not taking gravity in account)
        float Y = sin(TORAD(gunTilt)) * gunPow; // upwards motion of bullet
        float X = cos(TORAD(gunTilt)) * gunPow; // ground velocity of bullet
        float Yp = Y - gravity;
        float C = pos.z + 1; // nozzle elevation above the ground
        float B = Y / X; // equal to tan(gunTilt)
        float A = (Yp - B*X) / (X*X); // find A based on change in Y
        float len = (-B - sqrt((B*B) - 4*A*C)) / (2*A); // quadratic formula

        float acc = 8; // croshair accuracy

 		// enable line smoothing
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glEnable(GL_LINE_SMOOTH);

        // current gun rotation
        glRotatef(gunRot, 0.0f, 0.0f, 1.0f);

        // enable line stippling
        glEnable(GL_LINE_STIPPLE);
        GLushort pattern = 0x00FF;
        GLint factor = 1;
        glLineStipple(factor, pattern);

        // draw line from tank to target
        glBegin(GL_LINES);
            glColor4f(1.0, 0.0, 0.0, 0.5);
            glVertex3f(25, 0, 0);
            glColor4f(1.0, 0.3, 0.0, 0.5);
            glVertex3f(len - acc, 0, 0);
        glEnd();


        // draw the 4 croshair lines
        glBegin(GL_LINES);
            glColor4f(1.0, 0.0, 0.0, 0.5);
            glVertex3f(len - acc, +acc, 0);
            glColor4f(1.0, 0.3, 0.0, 0.5);
            glVertex3f(len, 0, 0);
        glEnd();

        glBegin(GL_LINES);
            glColor4f(1.0, 0.0, 0.0, 0.5);
            glVertex3f(len - acc, -acc, 0);
            glColor4f(1.0, 0.3, 0.0, 0.5);
            glVertex3f(len, 0, 0);
        glEnd();

        glBegin(GL_LINES);
            glColor4f(1.0, 0.0, 0.0, 0.5);
            glVertex3f(len + acc, +acc, 0);
            glColor4f(1.0, 0.3, 0.0, 0.5);
            glVertex3f(len, 0, 0);
        glEnd();

        glBegin(GL_LINES);
            glColor4f(1.0, 0.0, 0.0, 0.5);
            glVertex3f(len + acc, -acc, 0);
            glColor4f(1.0, 0.3, 0.0, 0.5);
            glVertex3f(len, 0, 0);
        glEnd();

        glDisable(GL_LINE_STIPPLE);

        glPopMatrix();
    }

    // draw the tank base
	glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
	glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
	glRotatef(rot.z, 0.0f, 0.0f, 1.0f);

	glColor4f(1.0, 1.0, 1.0, 1.0); // white, full alpha body
	GLuint texture = engFire ? engFireTex : baseTex;
	engFire = false; // reset engFire state to false
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

    // draw the tank gun
   	glTranslatef(0, 0, 1);
	glRotatef(gunRot - rot.z, 0.0f, 0.0f, 1.0f);

	glColor4f(1.0, 1.0, 1.0, 1.0); // white, full alpha body
	glBindTexture(GL_TEXTURE_2D, gunTex);

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
	if (pos.x < 0 || pos.x > map->getW()) { vel.x = -vel.x; pos.x += vel.x; }
	if (pos.y < 0 || pos.y > map->getH()) { vel.y = -vel.y; pos.y += vel.y; }

	pos.x += vel.x;
	pos.y += vel.y;

	float curVel = sqrt(pow(vel.y, 2) + pow(vel.x, 2));
	float newVel = applyDrag(curVel);

	if (newVel > 0.20) {
		if (fabs(vel.x) > 0.1) {
			float xRatio = vel.x / curVel;
			vel.x = newVel * xRatio;
		}
		if (fabs(vel.y) > 0.1) {
			float yRatio = vel.y / curVel;
			vel.y = newVel * yRatio;
		}
	} else { // stop when curVel <= 0.20
		vel.x = vel.y = 0;
	}
}

//==============================================================================
/* public interface functions */

//------------------------------------------------------------------------------
void Tank::engageEngine(float angle) {
	vel.x += cos(TORAD(angle)) * engPow;
	vel.y += sin(TORAD(angle)) * engPow;
	engFire = true;
}

void Tank::setBaseRot(float angle) {
	rot.z = angle;
}

void Tank::setGunRot(float angle) {
	gunRot = angle;
}

float Tank::getX() {
	return pos.x;
}
float Tank::getY() {
	return pos.y;
}

//==============================================================================
/* private (helper) functions */
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

	return vel * 0.995; // 0.1% drag per frame

}
