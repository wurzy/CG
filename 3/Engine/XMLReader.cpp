#include "XMLReader.h"
namespace XMLReader {

	vector<Point> readModel(string f) {
		//string line;
		string line;
		vector<Point> points;
		int i = 1;
		ifstream file(f);
		Point p;
		if (file.is_open()) {
			while (file.good()) {
				file >> p.x >> p.y >> p.z;
				if (file.eof()) break;
				points.push_back(p);
				i++;
			}
		}
		else {
			cout << "ERROR READING FILE " << f <<" (Does not exist?)" << endl;
		}
		file.close();
		return points;
	}

	void parseModels(XMLElement* models, Transformations* transforms, unsigned int* nFig) {
		for (XMLElement* elem = models->FirstChildElement("model"); elem; elem = elem->NextSiblingElement("model")) {
			string file = elem->Attribute("file");
			vector<Point> points = readModel(DIR + file);
			transforms->addModel(new Model(points, *nFig));
			(*nFig)++;
		}
	}

	void parseTranslate(XMLElement* translate, Transformations* transforms) {
		if (translate->FindAttribute("time")) {
			float time = translate->FloatAttribute("time");
			vector<Point> controlPoints;
			for (XMLElement* controlPoint = translate->FirstChildElement("point"); controlPoint; controlPoint = controlPoint->NextSiblingElement("point")) {
				Point p;
				p.x = controlPoint->FloatAttribute("x");
				p.y = controlPoint->FloatAttribute("y");
				p.z = controlPoint->FloatAttribute("z");
				controlPoints.push_back(p);
			}
			transforms->addTranslate(new Translate(controlPoints,time));
		}
		else {
			transforms->addTranslate(new Translate(translate->FloatAttribute("x"), translate->FloatAttribute("y"), translate->FloatAttribute("z")));
		}
	}

	void parseGroup(XMLElement* group, Transformations* transforms, unsigned int* nFig) {
		for (XMLElement* elem = group->FirstChildElement(); elem; elem = elem->NextSiblingElement()) { //for each element on group
			string type = elem->Value();
			if (type.compare("translate") == 0){ 
				//transforms->addTranslate(new Translate(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
				parseTranslate(elem, transforms);
			}
			else if (type.compare("rotate") == 0) {
				transforms->addRotate(new Rotate(elem->FloatAttribute("angle"),elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
			}
			else if (type.compare("scale") == 0) {
				transforms->addScale(new Scale(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
			}
			else if (type.compare("models") == 0) {
				parseModels(elem, transforms, nFig);
			}
			else if (type.compare("group") == 0) {
				Transformations* subgroup = new Transformations();
				transforms->addSubgroup(subgroup);
				parseGroup(elem, subgroup,nFig);
			}
			else if (type.compare("colour") == 0) {
				float red = (elem->IntAttribute("r"))/255.0f;
				float green = (elem->IntAttribute("g"))/255.0f;
				float blue = (elem->IntAttribute("b"))/255.0f;
				transforms->addRGB(red, green, blue);
			}
			else {
				cout << "ERROR. Group has invalid element:" << elem->Value() << ". Skipping element..."<< endl;
			}
		}
	}
	
	void xmlReader(string f, vector<Transformations*>* transforms, unsigned int* nFig) {
		XMLDocument doc;
		XMLNode* root;

		vector<Point> model;
		Transformations* t;
		if (!(doc.LoadFile(f.c_str()))) {
			root = doc.FirstChild(); // scene is root
			// for each <group>...</group>
			for (XMLElement* group = root->FirstChildElement("group"); group; group = group->NextSiblingElement("group")) {
				t = new Transformations();
				parseGroup(group, t, nFig);
				transforms->push_back(t);
			}
			cout << "Done! Read scene from: " << f << endl;
		}
		else {
			cout << "ERROR!" << endl;
		}
	}
}
