
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
float camX = 0, camY = 3, camZ = 0;
float ang = 0;
float Lx = 0, Ly = 3, Lz = 0.000001;

//teapots & torus
float in = 0, out = 0;

float rad = 50.0f, ri = 35.0f, rc = 15.0f;

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

// i * tw = linha, j = posicao nessa linha, a divisao em vez de 80 seria 30, mas 30 fica demasiado pequeno (enunciado pedia 0-30).
float h(int i, int j) {
	return imageData[i * tw + j] / (255.0f / 80.0f);
}

float hf(float px, float pz) {
	px += tw/2;
	pz += th/2;

	int x1 = floor(px);
	int x2 = x1 + 1;
	int z1 = floor(pz);
	int z2 = z1 + 1;

	float fx = px - x1;
	float fz = pz - z1;

	float h_x1_z = h(z1, x1) * (1 - fz) + h(z2, x1) * fz;
	float h_x2_z = h(z1, x2) * (1 - fz) + h(z2, x2) * fz;

	return (h_x1_z * (1 - fx) + h_x2_z * fx);
}

void drawTree() {
	int randFactor = rand() % 20 - 9; // [-9,10], para serem arvores com alturas diferentes
	float cx, cy, cz;

	cx = rand() % 99;
	cz = rand() % 99;
	if (cx * cx + cz * cz <= rad * rad) { // x^2 + z^2 <= r^2 (se está dentro do circulo grande)
		float multiplier = (rand() % 10 + 10) / 10.0f; // [1,2]
		if (rand() % 2 == 0) {
			cx = rad * multiplier;
		}
		else {
			cz = rad * multiplier;
		}
	}
	if (rand() % 2 == 0) { cx = -cx; }
	if (rand() % 2 == 0) { cz = -cz; }
	cx = cx + 0.5f;
	cz = cz - 0.5f;
	//h((z+tw/2),256-(x+th/2))
	cy = hf(cx,cz);
	//cout << cy << endl;

	glPushMatrix();

	glTranslatef(cx, cy, cz); // para ficar na coord certa

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

	glTranslatef(0, 0.75 + hf(0,0), 0);
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

		glTranslatef(rc, 1 + hf(rc, 0), 0); //afastar o teapot do centro e subir um bocado de acordo com o terreno
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

		glTranslatef(0, 1 + hf(0, ri), ri); //afastar o teapot do centro e subir um bocado de acordo com o terreno
		glColor3f(1.0f, 0.0f, 0.0f);
		glutSolidTeapot(1);

		glPopMatrix();

		// no final de cada teapot rodamos o eixo "ang" graus
		glRotatef(ang, 0, 1, 0);
	}
	glPopMatrix();
}

void drawScenario() {
	//TORUS
	generateTorus();

	//ARVORES 
	generateTrees(300);

	//TEAPOTS
	innerTeapots(8);
	outerTeapots(16);

	in -= 2;
	out += 1.5;
}


void drawTerrain() {
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glPushMatrix();
	glColor3f(0.0f, 0.6f, 0.3f);
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
	//gluLookAt(camX, camY, camZ, 
	//	      0.0,0.0,0.0,
	//		  0.0f,1.0f,0.0f);

	gluLookAt(camX, camY, camZ,
		Lx, Ly, Lz,
		0.0f, 1.0f, 0.0f);

	drawTerrain();
	drawScenario();

// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char key, int xx, int yy) {
	float k = 0;
	float k2 = 0;
	switch (key) {
		case 'w':
			k = 0.5;
			break;
		case 'q': 
			ang += 0.1;
			break;
		case 's':
			k = (-0.5);
			break;
		case 'e':
			ang -= 0.1;
			break;
		case 'a':
			k2 = 0.5;
			break;
		case 'd':
			k2 = -0.5;
			break;
		default: 
			break;
	}
	float dx = Lx - camX;
	float dz = Lz - camZ;

	// d x (0,1,0) = (dz, _, dx).
	float rx = dz;
	float rz = dx;

	Lx = camX + sin(ang);
	Ly = camY;
	Lz = camZ + cos(ang);

	camX = camX + k * dx;
	camY = 3 + hf(camX, camZ);
	camZ = camZ + k * dz;

	Lx = Lx + k * dx + k2 * rx;
	Lz = Lz + k * dz + k2 * rz;

	camX += k2 * rx;
	camZ += k2 * rz;
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
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

