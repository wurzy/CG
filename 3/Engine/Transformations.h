#pragma once
#ifndef __MY__TRANSFORMS__
#define __MY__TRANSFORMS__

#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>
#include "Point.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>
#include "catmull-rom.h"

using namespace std;

class Rotate {
public:
	bool animated;
	float time;
	float ang, x, y, z;
	Rotate() {
		this->x = this->y = this->z = this->ang = this->time = 0;
		this->animated = false;
	}

	Rotate(float ang, float cx, float cy, float cz) {
		this->x = cx;
		this->y = cy;
		this->z = cz;
		this->ang = ang;
		this->time = 0;
		this->animated = false;
	}

	Rotate(float t, float cx, float cy, float cz, bool a) { // bool is just to make the constructors different.
		this->x = cx;
		this->y = cy;
		this->z = cz;
		this->ang = 0;
		this->time = t * 1000; // seconds to milliseconds
		this->animated = a;
	}
	
	void apply(float elapsed_time) {
		if (this->animated) {
			this->ang = (elapsed_time  / this->time) * 360; // current angle related to elapsed time
		}
		glRotatef(this->ang, this->x, this->y, this->z);
	}
};


class Translate {
public:
	bool animated;
	bool traced;
	bool toggledTrace;
	float x, y, z;
	CatmullRom* cr;

	Translate() {
		this->cr = NULL;
		this->x = this->y = this->z = 0;
		this->animated = false;
		this->traced = false;
		this->toggledTrace = false;
	}

	Translate(float cx, float cy, float cz) {
		this->cr = NULL;
		this->animated = false;
		this->traced = false;
		this->toggledTrace = false;
		this->x = cx;
		this->y = cy;
		this->z = cz;
	}

	Translate(vector<Point> controlPoints, float t, int seg) {
		this->x = this->y = this->z = 0;
		this->animated = true;
		this->traced = false;
		this->toggledTrace = false;
		this->cr = new CatmullRom(controlPoints, t, seg);
	}

	Translate(vector<Point> controlPoints, float t, int seg, GLuint id, float* rgb) {
		this->x = this->y = this->z = 0;
		this->animated = true;
		this->traced = true;
		this->toggledTrace = true;
		this->cr = new CatmullRom(controlPoints, t, seg, id, rgb);
	}

	void prepareTranslate(GLuint* b) {
		if (this->traced) {
			this->cr->addReferenceBuffer(b);
			this->cr->prepareCurve();
		}
	}

	void toggleTrace() {
		this->toggledTrace = this->toggledTrace ? false : true;
	}

	void apply(float elapsed_time) {
		if (!this->animated) {
			glTranslatef(this->x, this->y, this->z);
		}
		else if (this->cr->isValid()) {
			if (this->traced && this->toggledTrace) {
				cr->traceCurve();
			}
			cr->animatedTranslate(elapsed_time);
		}
		else {
			cout << "ERROR. Minimum of 4 points required. Aborting this transformation..." << endl;
		}
	}
};


class Scale {
public:
	float x, y, z;
	Scale() {
		this->x = this->y = this->z = 0;
	}

	Scale(float cx, float cy, float cz) {
		this->x = cx;
		this->y = cy;
		this->z = cz;
	}

	void apply() {
		glScalef(this->x, this->y, this->z);
	}
};

class Model {
	vector<Point> points;
	GLuint vboID;

public:
	Model(vector<Point> p, GLuint id) {
		this->points = p;
		vboID = id;
	}

	void prepareModel(GLuint* buffer) {
		int size = this->points.size();
		float* p = new float[size * 3];
		int i = 0;
		for (int j = 0; j < size; j++) {
			p[i] = this->points.at(j).x;
			p[i + 1] = this->points.at(j).y; // unroll Point to floats
			p[i + 2] = this->points.at(j).z;
			i += 3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, p, GL_STATIC_DRAW);
	}

	void drawModel(float red, float green, float blue, GLuint* buffer) {
		glColor3f(red, green, blue);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, points.size());
	}
};

class Transformations {
	Translate* translate; 
	Rotate* rotate;
	Scale* scale;
	vector<Model*> models; // models in a group
	vector<Transformations*> subgroups;
	float red, green, blue;
	GLuint* buffer;
	bool axis; //if the axis is enabled

public:
	Transformations() {
		this->translate = NULL;
		this->scale = NULL;
		this->rotate = NULL;
		this->models = vector<Model*>();
		this->subgroups = vector<Transformations*>();
		red = 1.0f;
		green = blue = 0.0f;
		buffer = NULL;
		axis = false;
	}

	void addModel(Model* m) {
		models.push_back(m);
	}

	void addSubgroup(Transformations* sub) {
		subgroups.push_back(sub);
	}

	void addTranslate(Translate* t) {
		this->translate = t;
	}
	
	void addScale(Scale* s) {
		this->scale = s;
	}

	void addRotate(Rotate* r) {
		this->rotate = r;
	}

	void addRGB(float r, float g, float b) {
		this->red = r;
		this->green = g;
		this->blue = b;
	}

	void addReferenceBuffer(GLuint* b) {
		this->buffer = b;
		for (Transformations* t : this->subgroups) {
			t->addReferenceBuffer(b); // subgroup references to VBOs
		}
	}

	void start() {
		for (Model* m : this->models) {
			m->prepareModel(this->buffer);
		}

		if(this->translate) this->translate->prepareTranslate(this->buffer);

		for (Transformations* t : this->subgroups) {
			t->start(); // subgroup transformations, recursive
		}
	}

	void toggleAxis() {
		this->axis = this->axis ? false : true;
		for (Transformations* t : this->subgroups) {
			t->toggleAxis(); // subgroup transformations, recursive
		}
	}

	void toggleTrace() {
		if (this->translate) this->translate->toggleTrace();
		for (Transformations* t : this->subgroups) {
			t->toggleTrace(); // subgroup transformations, recursive
		}
	}

	void drawAxis() {
		glPushMatrix();
		glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		// Y Axis in green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 10.0f, 0.0f);

		// Z Axis in green
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 10.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glEnd();
		glPopMatrix();
	}

	void drawAll(float elapsed_time) {
		glPushMatrix();
		if (this->translate) this->translate->apply(elapsed_time); // parent transformations
		if (this->rotate) this->rotate->apply(elapsed_time);
		if (this->scale) this->scale->apply();

		for (Model* m : this->models) {
			m->drawModel(red,green,blue, this->buffer);
			if(axis) this->drawAxis();
		}

		for (Transformations* t : this->subgroups) {
			t->drawAll(elapsed_time); // subgroup transformations, recursive
		}
		glPopMatrix();

	}
};


#endif

