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
	float x, y, z;
	Translate() {
		this->x = this->y = this->z = 0;
	}

	Translate(float cx, float cy, float cz) {
		this->x = cx;
		this->y = cy;
		this->z = cz;
	}

	void apply() {
		glTranslatef(this->x, this->y, this->z);
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

public:
	Model(vector<Point> p) {
		this->points = p;
	}

	void drawModel(float red, float green, float blue) {
		int size = this->points.size();
		Point p1, p2, p3;

		glColor3f(red, green, blue);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < size; i += 3) { //

			p1 = this->points.at(i);
			p2 = this->points.at(i + 1);
			p3 = this->points.at(i + 2);

			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
		}
		glEnd();
	}
	void toString() {
		cout << points.size() << endl;
	}
};

class Transformations {
	Translate* translate; // c++ pointers to objects :/ 
	Rotate* rotate;
	Scale* scale;
	vector<Model*> models; // models num grupo
	vector<Transformations*> subgroups;
	float red, green, blue;

public:
	Transformations() {
		this->translate = NULL;
		this->scale = NULL;
		this->rotate = NULL;
		this->models = vector<Model*>();
		this->subgroups = vector<Transformations*>();
		red = 1.0f;
		green = blue = 0.0f;
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

	void drawAll() {
		glPushMatrix();
		if (this->rotate) this->rotate->apply();
		if (this->translate) this->translate->apply(); // parent transformations
		if (this->scale) this->scale->apply();
		
		for (Model* m : this->models) {
			m->drawModel(red,green,blue);
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

