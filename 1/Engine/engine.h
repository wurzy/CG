#pragma once
#include <stdlib.h> //clash com GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>
#include "tinyxml2.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;
using namespace tinyxml2;

struct Point {
	float x;
	float y;
	float z;
};
