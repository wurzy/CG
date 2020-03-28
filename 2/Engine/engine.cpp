#include "engine.h"
float cam_radius = 200.0f; 
float cam_alpha = 0, cam_beta = 0;
float cx, cy, cz; // camera as a sphere of radius r
float lastX, lastY; // last position on window
bool start = true; // to calculate first iteration of lastX lastY
bool zoom = false; // to calculate zoom

string dir = "./files/";
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

void draw(){
	for (Transformations t : *transformations) {
		t.drawAll();
	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cx, cy, cz,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f);
	
	draw();

	glutSwapBuffers();
}

void getTransformations(string f) {
	transformations = new vector<Transformations>();
	xmlReader(f, transformations);
	cout << "Scene: " << f << " loaded!" << endl;
}

/*
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

	}
	spherical2Cartesian();
	glutPostRedisplay();
}
*/

void mouseMove(int x, int y) {
	if (start) {
		lastX = x;
		lastY = y;
		start = false;
	}
	float x_off = x - lastX;
	float y_off = y - lastY;
	// only executes if button was clicked
	if (lastX >= 0) {
		if (zoom) {
			if (cam_radius + y_off > 0) { // so it doesnt invert
				cam_radius += y_off;
			}
		}
		else {
			cam_alpha -= x_off * 0.001f; // calculate x offset for camera
			float camOffset = y_off * 0.001f;
			if (cam_beta + camOffset < -1.5f ) {
				cam_beta = -1.5f;
			}
			else if (cam_beta + camOffset > 1.5f) {
				cam_beta = 1.5f;
			}
			else {
				cam_beta += camOffset; //calculate y offset for camera
			}
		}
		lastX = x;
		lastY = y;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}

// detect if mouse clicked
void mouseButton(int button, int state, int x, int y) {
	
	if (state == GLUT_UP) {
		lastX = -1; // mouse is 0 or positive, negative = not being clicked
	}
	else {
		lastX = x;
		lastY = y;
	}

	if (button == GLUT_RIGHT_BUTTON) {
		state == GLUT_UP ? zoom = false : zoom = true;
	}

}

int main(int argc, char **argv) {
// init GLUT and the window
	getTransformations(dir + argv[1]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(1200,800);
	glutCreateWindow("Engine - Phase 2");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	//glutIdleFunc(renderScene);

// put here the registration of the keyboard callbacks
	//glutKeyboardFunc(processKeys);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_POINTS);
	spherical2Cartesian();
// enter GLUT's main cycle
	glutMainLoop();
	return 1;
}
