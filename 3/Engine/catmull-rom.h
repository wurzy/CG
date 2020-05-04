#include <stdlib.h> //clash com GLUT
#include <stdio.h>
#include <fstream>
#include "Point.h"
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>
#define _USE_MATH_DEFINES
#include <math.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

class CatmullRom {
	int points;
	vector<Point> segPoints;

	float gt;
	float animationTime;

	float pos[3];
	float deriv[3];

	int segments;
	float* rgb;
	GLuint vboID;
	GLuint* buffer;

private:
	void multMatrixVector(float* m, float* v, float* res) {
		for (int j = 0; j < 4; ++j) {
			res[j] = 0;
			for (int k = 0; k < 4; ++k) {
				res[j] += v[k] * m[j * 4 + k];
			}
		}
	}

	void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, float* pos, float* deriv) {

		// catmull-rom matrix
		float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
							{ 1.0f, -2.5f,  2.0f, -0.5f},
							{-0.5f,  0.0f,  0.5f,  0.0f},
							{ 0.0f,  1.0f,  0.0f,  0.0f} };

		// Compute A = M * P
		float px[4] = { p0.x,p1.x,p2.x,p3.x };
		float py[4] = { p0.y,p1.y,p2.y,p3.y };
		float pz[4] = { p0.z,p1.z,p2.z,p3.z };

		float ax[4];
		float ay[4];
		float az[4];
		multMatrixVector(*m, px, ax);
		multMatrixVector(*m, py, ay);
		multMatrixVector(*m, pz, az);

		// Compute pos = T * A
		float T[4] = { pow(t,3) , pow(t,2) , t , 1 };
		pos[0] = T[0] * ax[0] + T[1] * ax[1] + T[2] * ax[2] + T[3] * ax[3];
		pos[1] = T[0] * ay[0] + T[1] * ay[1] + T[2] * ay[2] + T[3] * ay[3];
		pos[2] = T[0] * az[0] + T[1] * az[1] + T[2] * az[2] + T[3] * az[3];
		// compute deriv = T' * A
		float T_d[4] = { 3 * pow(t,2) , 2 * t , 1 , 0 };
		deriv[0] = T_d[0] * ax[0] + T_d[1] * ax[1] + T_d[2] * ax[2] + T_d[3] * ax[3];
		deriv[1] = T_d[0] * ay[0] + T_d[1] * ay[1] + T_d[2] * ay[2] + T_d[3] * ay[3];
		deriv[2] = T_d[0] * az[0] + T_d[1] * az[1] + T_d[2] * az[2] + T_d[3] * az[3];
		// ...
	}

	// given  global t, returns the point in the curve
	void getGlobalCatmullRomPoint(float gt, float* pos, float* deriv) {

		float t = gt * this->points; // this is the real global t
		int index = floor(t);  // which segment
		t = t - index; // where within  the segment

		// indices store the points
		int indices[4];
		indices[0] = (index + this->points - 1) % this->points;
		indices[1] = (indices[0] + 1) % this->points;
		indices[2] = (indices[1] + 1) % this->points;
		indices[3] = (indices[2] + 1) % this->points;

		getCatmullRomPoint(t, this->segPoints.at(indices[0]), this->segPoints.at(indices[1]), this->segPoints.at(indices[2]), this->segPoints.at(indices[3]), pos, deriv);
	}

public:
	CatmullRom(vector<Point> p, float time, int seg) {
		this->segPoints = p;
		this->points = p.size();
		this->animationTime = time * 1000; // seconds to milliseconds
		this->segments = seg;
		this->vboID = 0;
		this->rgb = NULL;
	}

	CatmullRom(vector<Point> p, float time, int seg, GLuint id, float* color) {
		this->segPoints = p;
		this->points = p.size();
		this->animationTime = time * 1000; // seconds to milliseconds
		this->segments = seg;
		this->vboID = id;
		this->rgb = color;
	}

	void addReferenceBuffer(GLuint* b) {
		this->buffer = b;
	}

	void animatedTranslate() {
		this->gt = glutGet(GLUT_ELAPSED_TIME) / this->animationTime; // elapsed time
		this->gt -= floor(gt); // has to be between 0 and 1

		getGlobalCatmullRomPoint(gt, pos, deriv);

		glTranslatef(pos[0], pos[1], pos[2]); // put the model in place
	}
	
	bool isValid() {
		return (this->points >= 4);
	}

	void prepareCurve() {
		float* loopPoints = new float[segments * 3]; 
		int j = 0;
		float t;
		for (int i = 0; i < this->segments; i++) {
			t = i / (float) segments;
			getGlobalCatmullRomPoint(t, pos, deriv);
			loopPoints[j] = pos[0];
			loopPoints[j + 1] = pos[1];
			loopPoints[j + 2] = pos[2];
			j += 3;
		}

		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * segments * 3, loopPoints, GL_STATIC_DRAW);
	}

	void traceCurve() {
		glColor3f(this->rgb[0], this->rgb[1], this->rgb[2]);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_LINE_LOOP, 0, segments);
	}
};