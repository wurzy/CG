#include "engine.h"
const float cam_radius = 200.0f; // default que tinhamos no skeleton
float cam_alpha = 0, cam_beta = 0;
float cx, cy, cz; // meter a camara a bater certo com esfera de radio radius
//Rotate rotate(1, 2, 3, 4);    object declaration

string dir = "./files/";
//vector<Point> pontos;
vector<Transformations>* transformations;

void spherical2Cartesian() {
	cx = cam_radius * cos(cam_beta) * sin(cam_alpha);
	cy = cam_radius * sin(cam_beta);
	cz = cam_radius * cos(cam_beta) * cos(cam_alpha);
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

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cx, cy, cz,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	for (Transformations t : *transformations) {
		t.drawAll();
	}

	glutSwapBuffers();
}

void getTransformations(string f) {
	transformations = new vector<Transformations>();
	xmlReader(f, transformations);
	cout << "Scene: " << f << " loaded!" << endl;
}

void processKeys(unsigned char c, int xx, int yy) {
	switch (c) {
	case 'w':
		cam_beta += 0.1f;
		if (cam_beta > 1.5f)
			cam_beta = 1.5f;
		break;
	case 's':
		cam_beta -= 0.1f;
		if (cam_beta < -1.5f)
			cam_beta = -1.5f;
		break;
	case 'a':
		cam_alpha += 0.1;
		break;
	case 'd':
		cam_alpha -= 0.1;
		break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}

int main(int argc, char **argv) {
// init GLUT and the window
	getTransformations(dir + argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Engine - Phase 2");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

// put here the registration of the keyboard callbacks
	glutKeyboardFunc(processKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_POINTS);
	spherical2Cartesian();
// enter GLUT's main cycle
	glutMainLoop();
	return 1;
}
