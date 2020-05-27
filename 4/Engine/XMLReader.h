#pragma once
#ifndef __XMLREADER__
#define __XMLREADER__
#include <stdlib.h> //clash com GLUT
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>
#include "Transformations.h"
#include "tinyxml2.h"
#include "Point.h"
#include "Lights.h"

#define _USE_MATH_DEFINES
#define DIR "./files/"
#include <math.h>

using namespace std;
using namespace tinyxml2;

namespace XMLReader {
	void xmlReader(string f, vector<Transformations*>* transforms, vector<Light*>* ls, Camera* camera, unsigned int* nFig);
}

#endif