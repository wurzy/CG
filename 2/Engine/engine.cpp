#include "engine.h"
const float cam_radius = 20.0f; // default que tinhamos no skeleton
float cam_alpha = 0, cam_beta = 0;
float cx, cy, cz; // meter a camara a bater certo com esfera de radio radius

string dir = "./files/";
vector<Point> pontos;

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


void drawVector() {
	int size = pontos.size();
	Point p1, p2, p3;
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < size; i += 6) {
		p1 = pontos.at(i);
		p2 = pontos.at(i + 1);
		p3 = pontos.at(i + 2);

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);

		p1 = pontos.at(i + 3);
		p2 = pontos.at(i + 4);
		p3 = pontos.at(i + 5);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(p1.x, p1.y, p1.z);
		glVertex3f(p2.x, p2.y, p2.z);
		glVertex3f(p3.x, p3.y, p3.z);
	}
	glEnd();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(cx, cy, cz,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	drawVector();
	// End of frame
	glutSwapBuffers();
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

void modelReader(string f) {
	//string line;
	string line;
	int i = 1;
	ifstream file(f);
	Point p;
	if (file.is_open()) {
		while (file.good()) {
			file >> p.x >> p.y >> p.z;
			if(file.eof()) break;
			pontos.push_back(p);
			i++;
		}
		cout << "File: " << f << " done!" << endl;
	}
	else {
		cout << "ERROR" << endl;
	}
	file.close();
}

void xmlReader(string f) {
	XMLDocument doc;
	XMLElement* root;

	if (!(doc.LoadFile(f.c_str()))) {
		root = doc.FirstChildElement();
		for (XMLElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
			string file = elem->Attribute("file");
			cout << "Trying to read: " << file << "..." << endl;
			modelReader(dir + file);
		}
	}
	else {
		cout << "ERROR!" << endl;
	}
	return;
}

int main(int argc, char **argv) {
// init GLUT and the window
	//_mkdir("./files");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("Engine - Phase 1");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

// put here the registration of the keyboard callbacks
	glutKeyboardFunc(processKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	xmlReader(dir + argv[1]);
	spherical2Cartesian();
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
