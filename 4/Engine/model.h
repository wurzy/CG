#pragma once
#ifndef __MY__MODEL__
#define __MY__MODEL__

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
#include <IL/il.h>
#endif
#define _USE_MATH_DEFINES
#include <math.h>
class Model {
	vector<Point> points;
	float *diff, *amb, *spec, *emiss;
	float shininess;
	string file;
	GLuint vboID;
	
public:
	Model(vector<Point> p, GLuint id, float* d, float* a, float* s, float* e, float sh) {
		this->points = p;
		this->diff = d;
		this->amb = a;
		this->spec = s;
		this->emiss = e;
		this->shininess = sh;
		vboID = id;
		this->file = "";
	}

	Model(vector<Point> p, GLuint id, float* d, float* a, float* s, float* e, float sh, string tex) {
		this->points = p;
		this->diff = d;
		this->amb = a;
		this->spec = s;
		this->emiss = e;
		this->shininess = sh;
		vboID = id;
		this->file = "./textures/" + tex;
	}

	int getTexture() {
		unsigned int t, tw, th;
		unsigned char* texData;
		unsigned int texID;

		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		ilGenImages(1, &t);
		ilBindImage(t);
		ilLoadImage((ILstring)this->file.c_str());
		tw = ilGetInteger(IL_IMAGE_WIDTH);
		th = ilGetInteger(IL_IMAGE_HEIGHT);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		texData = ilGetData();

		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D, texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texID;
	}

	void prepareModel(GLuint* buffer, GLuint* normal, GLuint* textures, int* ids) {
		int size = this->points.size();
		float* p = new float[size * 3];
		float* n = new float[size * 3];
		float* t = new float[size * 2];
		int i = 0, tx = 0;
		for (int j = 0; j < size; j++) {
			p[i] = this->points.at(j).x;
			p[i + 1] = this->points.at(j).y;
			p[i + 2] = this->points.at(j).z;
					
			n[i] = this->points.at(j).nx;
			n[i + 1] = this->points.at(j).ny;
			n[i + 2] = this->points.at(j).nz;
			
			t[tx] = this->points.at(j).t1;
			t[tx + 1] = this->points.at(j).t2;
			i += 3;
			tx += 2;
		}
	
		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, p, GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, normal[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, n, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, textures[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 2, t, GL_STATIC_DRAW);

		if (this->file.compare("")!=0) {
			int txid = getTexture();
			ids[vboID] = txid;
		}
		else {
			ids[vboID] = -1;
		}
	}

	void drawModel(GLuint* buffer, GLuint* normal, GLuint* textures, int* ids) {

		float* amb2 = new float[4];
		float* diff2 = new float[4];
		float* spec2 = new float[4];
		float* emiss2 = new float[4];
		amb2[0] = amb2[1] = amb2[2] = 0.2f; amb2[3] = 1.0f;
		diff2[0] = diff2[1] = diff2[2] = 0.8f; diff2[3] = 1.0f;
		spec2[0] = spec2[1] = spec2[2] = 0.0f; spec2[3] = 1.0f;
		emiss2[0] = emiss2[1] = emiss2[2] = 0.0f; emiss2[3] = 1.0f;

		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normal[vboID]);
		glNormalPointer(GL_FLOAT, 0, 0);
		
		glBindBuffer(GL_ARRAY_BUFFER, textures[vboID]);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);

		glMaterialfv(GL_FRONT, GL_AMBIENT, this->amb);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diff); // non-default values
		glMaterialfv(GL_FRONT, GL_SPECULAR, this->spec);
		glMaterialfv(GL_FRONT, GL_EMISSION, this->emiss);

		if (ids[vboID] == -1) {
			glDrawArrays(GL_TRIANGLES, 0, points.size());
		}
		else {
			glBindTexture(GL_TEXTURE_2D, ids[vboID]);
			glDrawArrays(GL_TRIANGLES, 0, points.size());
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diff2); // default values to reset
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
		glMaterialfv(GL_FRONT, GL_EMISSION, emiss2);
	}
};
#endif