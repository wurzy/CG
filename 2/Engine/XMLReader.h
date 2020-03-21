#pragma once
#include <stdlib.h> //clash com GLUT
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>
#include "Transformations.h"
#include "../Engine/tinyxml2.h"

#define _USE_MATH_DEFINES
#define DIR "./files/"
#include <math.h>
#include "Point.h"

using namespace std;
using namespace tinyxml2;

namespace XMLReader {
	vector<Transformations> xmlReader(string f);
}