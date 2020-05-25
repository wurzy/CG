#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

// translaçoes, ir para dado ponto
double move_x = 0, move_y = 0, move_z = 0;
// rotaçao em torno do eixo vertical (o angulo tem de ser incrementado sempre porque e sempre de acordo com a perspetiva original, ou seja, 
//									  rodar duas vezes no mesmo angulo e a mesma coisa que rodar apenas uma unica vez)
double angle=0, rotate_x=0, rotate_y=1, rotate_z=0;
// escalar (os valores são percentagens, se for 2 é 200%, 1 é 100%, 0.5 é 50%...
double scale_x=1, scale_y=1, scale_z=1;


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


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	// put the geometric transformations here
	glTranslatef(move_x, move_y, move_z);
	glRotatef(angle, rotate_x, rotate_y, rotate_z);
	glScalef(scale_x, scale_y, scale_z);

	// put drawing instructions here
	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f(100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();

	glBegin(GL_TRIANGLES);
		//os pontos têm ordem, usando a regra da mao direita, assim garantimos que uma das faces é mostrada e a outra é invisível (default).
		//base 1
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		//base 2
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		//frente
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		//tras
		glColor3f(0.1f, 1.0f, 1.0f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		//esq
		glColor3f(1.0f, 0.0f, 0.1f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(-1.0f, 0.0f, -1.0f);
		glVertex3f(-1.0f, 0.0f, 1.0f);
		//dir
		glColor3f(0.3f, 1.0f, 0.4f);
		glVertex3f(1.0f, 0.0f, -1.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 1.0f);
	glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events

// teclas normais = translaçao
void translate(unsigned char key, int x, int y) {
	switch (key) {
		case 'a':
			move_x -= 0.5;
			break;
		case 'd':
			move_x += 0.5;
			break;
		case 's':
			move_z -= 0.5;
			break;
		case 'w':
			move_z += 0.5;
			break;
		case 'e':
			move_y += 0.5;
			break;
		case 'q':
			move_y -= 0.5;
		default:
			break;
	}
	glutPostRedisplay(); // colocar a frame em queue para ser processada
	return;
}

// rato = rotaçoes em torno do eixo vertical
void rotate(int button, int state, int x, int y) {
	if (state == GLUT_UP) {
		switch (button) {
			case GLUT_LEFT_BUTTON:
				//rodar para esquerda
				angle += 30;
				break;
			case GLUT_RIGHT_BUTTON:
				//rodar para direita
				angle -= 30;
				break;
			default:
				break;
		}
		glutPostRedisplay();
	}
	return;
}

// teclas especiais = escalar || mudar estilo de renderização
void scale(int key_code, int x, int y) {
	switch (key_code) {
		case GLUT_KEY_F1:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // para renderizar linhas
			break;
		case GLUT_KEY_F2:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); // para renderizar pontos
			break;
		case GLUT_KEY_F3:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // para renderizar tudo (default se nao for usada esta funcao)
			break;
		case GLUT_KEY_UP:
			scale_y += 0.1; // maior na "vertical"
			break;
		case GLUT_KEY_DOWN:
			scale_y -= 0.1; // menor na "vertical"
			break;
		// no enunciado so pedia altura, isto e pro resto
		case GLUT_KEY_RIGHT:
			scale_x += 0.1; // maior para a "direita"
			break;
		case GLUT_KEY_LEFT:
			scale_x -= 0.1; // menor para a "direita"
			break;
		case GLUT_KEY_PAGE_UP:
			scale_z += 0.1; // maior para a "esquerda"
			break;
		case GLUT_KEY_PAGE_DOWN:
			scale_z -= 0.1; // menor para a "esquerda"
			break;
		default:
			break;
	}
	glutPostRedisplay();
	return;
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

// put here the registration of the keyboard callbacks
	glutKeyboardFunc(translate);
	glutMouseFunc(rotate);
	glutSpecialFunc(scale);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
