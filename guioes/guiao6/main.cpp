
#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <iostream>

//camera
float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

//devil
unsigned int t, tw, th;
unsigned char* imageData;

//vbos
int pontos=0;
float* vertexB;
GLuint buffer[1];

using namespace std;
void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void drawTerrain() {
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glPushMatrix();
	//glScalef(0.1, 0.1, 0.1);
	//Como estamos a desenhar no modo GL_TRIANGLE_STRIP temos de desenhar uma 'tira' de cada vez
	for (int i = 0; i < th; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, i * tw * 2, 2 * tw);
	}
	glPopMatrix();
}



void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawTerrain();

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	//glutWireTeapot(2.0);

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

// i * tw = linha, j = posicao nessa linha, a divisao em vez de 50 seria 30, mas 30 fica demasiado pequeno (enunciado pedia 0-30).
float h(int i, int j) {
	return imageData[i * tw + j] / (255.0f/50.0f); 
}

void createVBO() {
	glEnableClientState(GL_VERTEX_ARRAY);

	//espacamentos de 1 unidade entre os vertices, calculo do extremo da esquerda-cima
	float init_value_h = -(th / 2.0f - 0.5f);
	float init_value_w = -(tw / 2.0f - 0.5f);
	int grow = 6;

	for (int i = 0; i < th-1; i++) {
		for (int j = 0; j < tw; j++) {
			// dirty way de fazer isto
			vertexB = (float*)realloc(vertexB, sizeof(float) * grow);
			grow += 6;

			vertexB[pontos++] = init_value_w;
			vertexB[pontos++] = h(i,j);
			vertexB[pontos++] = init_value_h;

			vertexB[pontos++] = init_value_w;
			vertexB[pontos++] = h(i+1,j);
			vertexB[pontos++] = init_value_h + 1.0f;

			init_value_w++;
		}
		init_value_w = -(tw / 2.0f - 0.5f);
		init_value_h++;
	}
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, pontos * sizeof(float), vertexB, GL_STATIC_DRAW);
	free(vertexB);
}


void getImage() {
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	imageData = ilGetData();
}

void init() {
	glewInit();
	ilInit();
// 	Load the height map "terreno.jpg"
	getImage();
// 	Build the vertex arrays
	createVBO();
// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,600);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

