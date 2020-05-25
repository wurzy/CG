#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

const float radius = 10.0f; // default que tinhamos no skeleton
const float limit = 1.5f; // abs(beta) == 1.5
float alpha = 0, beta = 0;
float cx = radius, cy = 0.0f, cz = 0.0f; // meter a camara a bater certo com esfera de radio radius

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawCylinder(float radius, float height, int slices) {

	// nao usar floats como variaveis de controlo de ciclo
	// put code to draw cylinder in here
	float x1, x2, z1, z2, angle, h;
	//para desenhar circulo
	for (int n = 0; n < slices; n++) {
		angle = (2 * M_PI) / slices;
		x1 = radius * sinf(angle * n);
		x2 = radius * sinf(angle * (n+1.0f));

		z1 = radius * cosf(angle * n);
		z2 = radius * cosf(angle * (n+1.0f));

		h = height / 2;

		glBegin(GL_TRIANGLES);
		//bases
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(0, -h, 0);
		glVertex3f(x2, -h, z2);
		glVertex3f(x1, -h, z1);

		glColor3f(0.1f, 0.4f, 0.9f);
		glVertex3f(0, h, 0); // mudar o h para ser cilindro
		glVertex3f(x1, h, z1);
		glVertex3f(x2, h, z2);

		//lateral
		glColor3f(0.5f, 0.8f, 0.2f);
		glVertex3f(x1, -h, z1);
		glVertex3f(x2, h, z2);
		glVertex3f(x1, h, z1);

		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(x1, -h, z1);
		glVertex3f(x2, -h, z2);
		glVertex3f(x2, h, z2);
		glEnd();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cx,cy,cz, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawCylinder(1,2,25);

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char c, int xx, int yy) {
	switch (c) {
		case 'w':
			if (beta <= limit) {
				beta += 0.1;
			}
			break;
		case 's':
			if (beta >= -limit) {
				beta -= 0.1;
			}
			break;
		case 'a':
			alpha -= 0.1;
			break;
		case 'd':
			alpha -= 0.1;
			break;
	}
	cx = radius * cosf(beta) * sinf(alpha);
	cy = radius * sinf(beta);
	cz = radius * cosf(beta) * cosf(alpha);
	glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
