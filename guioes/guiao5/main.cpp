#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>


float alfa = 0.0f, beta = 0.5f, radius = 200.0f;
float camX, camY, camZ;

float in = 0, out = 0;

float r = 50.0f, ri = 35.0f, rc = 15.0f;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawTree() {
	int randFactor = rand() % 20 - 9; // [-9,10], para serem arvores com alturas diferentes
	int cx, cy = 0, cz;

	cx = rand() % 99;
	cz = rand() % 99;
	if (cx * cx + cz * cz <= r * r) { // x^2 + z^2 <= r^2 (se está dentro do circulo grande)
		float multiplier = (rand() % 10 + 10) / 10.0f; // [1,2]
		if (rand() % 2 == 0) {
			cx = r * multiplier;
		}
		else {
			cz = r * multiplier;
		}
	}
	if (rand() % 2 == 0) { cx = -cx; }
	if (rand() % 2 == 0) { cz = -cz; }

	glPushMatrix();

	glTranslatef((float)cx, (float)cy, (float)cz); // para ficar na coord certa

	glScalef(0.2f, 0.2f, 0.2f); // trial and error com dimensoes em baixo, era so para ser mais facil desenhar
	glRotatef(-90, 1, 0, 0); // o cone vem deitado, virar o referencial

	glColor3f(0.4f, 0.4f, 0.3f);
	glutSolidCone(3, 20, 40, 40);

	glTranslatef(0, 0, 10);

	glColor3f(0.4f, 0.6f, 0.2f);
	glutSolidCone(15, 40 + (double)randFactor, 40, 40);

	glPopMatrix();
}

void generateTrees(int num) {
	srand(2020);
	for (int i = 0; i < num; i++) {
		drawTree();
	}
}

void generateTorus() {
	glPushMatrix();

	glTranslatef(0, 0.75, 0);
	glColor3f(1.0f, 0.0f, 1.0f);
	glutSolidTorus(0.5, 1.5, 20, 20);

	glPopMatrix();
}

void innerTeapots(int n) {

	float ang = 360 / n; //angulo entre cada teapot

	glPushMatrix();

	glRotatef(in, 0, 1, 0);

	for (int j = 0; j < n; j++) {

		glPushMatrix();

		glTranslatef(rc, 1, 0); //afastar o teapot do centro e subir um bocado
		glColor3f(0.0f, 0.0f, 1.0f);
		glutSolidTeapot(1);

		glPopMatrix();

		// no final de cada teapot rodamos o eixo "ang" graus
		glRotatef(ang, 0, 1, 0);
	}

	glPopMatrix();
}

void outerTeapots(int n) {

	float ang = 360 / n; //angulo entre cada teapot


	glPushMatrix();

	glRotatef(out, 0, 1, 0);

	for (int j = 0; j < n; j++) {

		glPushMatrix();

		glTranslatef(0, 1, ri); //afastar o teapot do centro e subir um bocado
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidTeapot(1);

		glPopMatrix();

		// no final de cada teapot rodamos o eixo "ang" graus
		glRotatef(ang, 0, 1, 0);
	}
	glPopMatrix();
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	//PLANO
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);

	glVertex3f(100.0f, 0, -100.0f);
	glVertex3f(-100.0f, 0, 100.0f);
	glVertex3f(100.0f, 0, 100.0f);
	glEnd();

	//TORUS
	generateTorus();

	//ARVORES 
	generateTrees(250);

	//TEAPOTS
	innerTeapots(8);
	outerTeapots(16);

	in -= 2;
	out += 1.5;

	// End of frame
	glutSwapBuffers();
}


void zoom(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here
	switch (c) {
	case 'w':
		radius -= 10;
		break;
	case 's':
		radius += 10;
		break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void camera(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}

void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char** argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// Callback registration for keyboard processing
	glutKeyboardFunc(zoom);
	glutSpecialFunc(camera);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
	