#pragma once
#ifndef __MY__TRANSFORMS__
#define __MY__TRANSFORMS__

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

using namespace std;

class Rotate {
public:
	float ang, x, y, z;
	Rotate() {
		this->x = this->y = this->z = this->ang = 0;
	}

	Rotate(float ang, float cx, float cy, float cz) {
		this->x = cx;
		this->y = cy;
		this->z = cz;
		this->ang = ang;
	}
	
	void apply() {
		glRotatef(this->ang, this->x, this->y, this->z);
	}

	void toString() {
		cout << "Rotate" << ang << " " << x << " " << y << " " << z << endl;
	}
};


class Translate {
public:
	bool animated;
	float x, y, z;
	vector<Point> controlpoints;
	float time, current;

	Translate() {
		this->x = this->y = this->z = 0;
		this->animated = false;
		this->current = time = 0;
	}

	Translate(float cx, float cy, float cz) {
		this->animated = false;
		this->x = cx;
		this->y = cy;
		this->z = cz;
		this->current = time = 0;
	}

	Translate(vector<Point> controlPoints, float t) {
		this->animated = true;
		this->current = 0;
		this->time = t;
		this->controlpoints = controlPoints;
	}

	void apply() {
		if (!this->animated) {
			glTranslatef(this->x, this->y, this->z);
		}
		else {
			if (this->controlpoints.size() >= 4) {
				// do catmull
			}
			else {
				cout << "ERROR. Minimum of 4 points required. Aborting this transformation..." << endl;
			}
		}
	}

	void toString() {
		cout << "Translate: " << x << " " << y << " " << z << endl;
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

	void toString() {
		cout << "Scale" << x << " " << y << " " << z << endl;
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
		if (p) {
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
	}

	void drawModel(float red, float green, float blue, GLuint* buffer) {
		glColor3f(red, green, blue);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, points.size());
	}

	void toString() {
		cout << points.size() << endl;
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
	}

	void addModel(Model* m) {
		//points.insert(this->points.end(),p.begin(),p.end());
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

		for (Transformations* t : this->subgroups) {
			t->start(); // subgroup transformations, recursive
		}
	}

	void drawAll() {
		glPushMatrix();
		if (this->rotate) this->rotate->apply();
		if (this->translate) this->translate->apply(); // parent transformations
		if (this->scale) this->scale->apply();
		

		for (Model* m : this->models) {
			m->drawModel(red,green,blue, this->buffer);
		}

		for (Transformations* t : this->subgroups) {
			t->drawAll(); // subgroup transformations, recursive
		}
		glPopMatrix();

	}

	void toString() {
		if(translate) translate->toString();
		if(rotate) rotate->toString();
		if (scale) scale->toString();
		
		cout << "Colour of these Models: " << red << " " << green << " " << blue << endl;
		
		cout << "Models: " << endl;
		for (Model* m : models) {
			m->toString();
		}
		cout << "End models. " << endl;
		cout << "//Subgroups//" << endl;
		for (Transformations* t : subgroups) {
			t->toString();
		}
		cout << "//End Subgroups//" << endl;
	}
};


#endif

