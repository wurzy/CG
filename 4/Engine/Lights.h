#pragma once
#ifndef __MY__LIGHTS__V__
#define __MY__LIGHTS__V__

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <direct.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>

class Light {
protected:
	GLenum id;
	float* pos;
	float* diff;
	float* amb;
	float* spec;
	bool toggled;

public:
	Light(GLenum i, float* p, float* d, float* a, float* s) {
		this->pos = p;
		this->diff = d;
		this->amb = a;
		this->spec = s;
		this->id = i;
		this->toggled = true;
	}

	void toggle() {
		if (toggled) {
			glDisable(id);
			toggled = false;
		}
		else {
			glEnable(id);
			toggled = true;
		}
	}

	void create() {
		glLightfv(this->id, GL_POSITION, this->pos);
		glLightfv(this->id, GL_AMBIENT, this->amb);
		glLightfv(this->id, GL_DIFFUSE, this->diff);
		glLightfv(this->id, GL_SPECULAR, this->spec);
	}

	virtual void apply(){}
};

class DirectionalLight: public Light{
public:
	DirectionalLight(GLenum i, float* p, float* d, float* a, float* s) : Light(i, p, d, a, s){}

	void apply() {
		this->create();
	}
};

class PointLight: public Light {
protected:
	float quad_att; // quadratic attenuation
public:
	PointLight(GLenum i, float* p, float* d, float* a, float* s, float at) : Light(i, p, d, a, s){
		this->quad_att = at;
	}

	void apply() {
		this->create();
		glLightfv(this->id, GL_QUADRATIC_ATTENUATION, &(this->quad_att));
	}
};

class SpotLight: public Light {
protected:
	float* dir;
	float cutoff, exponent; // cut-off angle, exponent
public:
	SpotLight(GLenum i, float* p, float* d, float* a, float* s, float* di, float cut, float exp) : Light(i, p, d, a, s) {
		this->dir = di;
		this->cutoff = cut;
		this->exponent = exp;
	}
	void apply() {
		this->create();
		glLightfv(this->id, GL_SPOT_DIRECTION, this->dir);
		glLightfv(this->id, GL_SPOT_EXPONENT, &(this->exponent));
		glLightfv(this->id, GL_SPOT_CUTOFF, &(this->cutoff));
	}
};

#endif