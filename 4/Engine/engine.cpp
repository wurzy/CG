#include "engine.h"
//camera
Camera* camera;
float cam_alpha = 0, cam_beta = 0;
float lastX, lastY; // last position on window
bool start = true; // to calculate first iteration of lastX lastY
bool zoom = false; // to calculate zoom

//fps
int frame = 0;
int timebase = 0;

//time
float previousTime = 0;
float currentTime;
float elapsedTime = 0;

//toggles
bool toggleAxis = false;
bool toggleCentralAxis = false;
bool togglePause = false;

//transformations
string dir = "./files/";
vector<Transformations*>* transformations;
vector<Light*>* lights;

//vbo
GLuint *figures, *normals, *textures;

void spherical2Cartesian() {
	camera->updateCartesian(cam_beta, cam_alpha);
}

void drawCentralAxis() {
	glEnable(GL_COLOR_MATERIAL);
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(500.0f, 0.0f, 0.0f);
	// Y Axis in green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 500.0f, 0.0f);
	// Z Axis in green
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 500.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnd();
	glDisable(GL_COLOR_MATERIAL);
}

void showFPS() {
	frame++;
	float fps;
	float timedif;
	int time = glutGet(GLUT_ELAPSED_TIME);
	char s[64];
	if (time - timebase > 1000) {
		timedif = time - timebase;
		fps = frame * 1000.0 / timedif;
		timebase = time;
		frame = 0;
		sprintf_s(s, "Engine - Phase 4 ( FPS: %2.f )", fps);
		glutSetWindowTitle(s);
	}
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

void calculateTime() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	float delta = currentTime - previousTime;
	previousTime = currentTime;
	if (!togglePause) {
		elapsedTime += delta;
	}
}

void draw(){
	calculateTime();

	for (Light* l : *lights) {
		l->start();
	}

	if (toggleCentralAxis) drawCentralAxis();

	for (Transformations* t : *transformations) {
		t->drawAll(elapsedTime);
	}
}

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set the camera
	glLoadIdentity();
	gluLookAt(camera->pos[0],camera->pos[1], camera->pos[2],
		camera->la[0], camera->la[1], camera->la[2],
		camera->up[0], camera->up[1], camera->up[2]);

	showFPS();

	draw();

	glutSwapBuffers();
}

void readXML(string f) {
	camera = new Camera();
	GLuint nFig = 0;
	transformations = new vector<Transformations*>();
	lights = new vector<Light*>();

	xmlReader(f, transformations, lights, camera, &nFig); // nFig is the number of models and traces

	figures = new GLuint[nFig]();
	normals = new GLuint[nFig]();
	textures = new GLuint[nFig]();
	int* ids = new int[nFig]();

	glGenBuffers(nFig, figures);
	glGenBuffers(nFig, normals);
	glGenBuffers(nFig, textures);
	
	for (Light* l : *lights) {
		l->init();
	}

	for (Transformations* t : *transformations) {
		t->addReferenceBuffer(figures, normals, textures, ids);
		t->start(); 
	}

	cout << "Scene: " << f << " loaded!" << endl;
}


void processKeys(unsigned char c, int xx, int yy) {
	switch (c) {
	case 'a': // axis
		for (Transformations* t : *transformations) {
			t->toggleAxis();
		}
		break;
	case 's': // show triangles
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'd': // default polygonmode
		glPolygonMode(GL_FRONT, GL_FILL);
		break;
	case 'f': // show points
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case 't':
		for (Transformations* t : *transformations) {
			t->toggleTrace();
		}
		break;
	case 'c':
		toggleCentralAxis = toggleCentralAxis ? false : true;
		break;
	case 'p':
		togglePause = togglePause ? false : true;
		break;
	}
	glutPostRedisplay();
}

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
			if (camera->rad + y_off > 0) { // so it doesnt invert
				camera->rad += y_off;
			}
		}
		else {
			cam_alpha -= x_off * camera->sensitivity; // calculate x offset for camera
			float camOffset = y_off * camera->sensitivity;
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

void _glutInit(int argc, char **argv) {
	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(1200, 800);
	glutCreateWindow("Engine - Phase 4");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(processKeys);
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	//float ambient[4] = { 0,0,0,1 };
//	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
}


void _glewInit() {
	glewInit();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

int main(int argc, char **argv) {
	_glutInit(argc,argv);
	_glewInit();
	
	readXML(dir + argv[1]);
	spherical2Cartesian();

// enter GLUT's main cycle
	glutMainLoop();
	return 1;
}
