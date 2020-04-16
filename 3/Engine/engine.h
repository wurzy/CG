#pragma once
#include <stdlib.h> //clash com GLUT
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <direct.h>
#include "tinyxml2.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "Point.h"
#include "XMLReader.h"
#include "Transformations.h"

using namespace XMLReader;
using namespace tinyxml2;
