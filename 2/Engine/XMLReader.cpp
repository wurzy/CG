#include "../build/XMLReader.h"
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
			cout << "File: " << f << " done!" << endl;
		}
		else {
			cout << "ERROR READING FILE " << f <<" (Does not exist?)" << endl;
		}
		file.close();
		return points;
	}

	void parseModels(XMLElement* models, Transformations* transforms) {
		for (XMLElement* elem = models->FirstChildElement("model"); elem; elem = elem->NextSiblingElement("model")) {
			string file = elem->Attribute("file");
			float red = elem->FloatAttribute("r");
			float green = elem->FloatAttribute("g");
			float blue = elem->FloatAttribute("b");
			vector<Point> points = readModel(DIR + file);
			transforms->addModel(new Model(points, red, green, blue));
		}
	}

	void parseGroup(XMLElement* group, Transformations* transforms) {
		for (XMLElement* elem = group->FirstChildElement(); elem; elem = elem->NextSiblingElement()) { //for each element on group
			string type = elem->Value();
			if (type.compare("translate") == 0){ 
				transforms->addTranslate(new Translate(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
			}
			else if (type.compare("rotate") == 0) {
				transforms->addRotate(new Rotate(elem->FloatAttribute("angle"),elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
			}
			else if (type.compare("scale") == 0) {
				transforms->addScale(new Scale(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z")));
			}
			else if (type.compare("models") == 0) {
				parseModels(elem, transforms);
			}
			else if (type.compare("group") == 0) {
				Transformations* subgroup = new Transformations();
				transforms->addSubgroup(subgroup);
				parseGroup(elem, subgroup);
			}
			else {
				cout << "ERROR OCCURRED ON GROUP" << endl;
				cout << "Due to: " << elem->Value() << endl;
			}
		}
	}
	
	vector<Transformations> xmlReader(string f) {
		XMLDocument doc;
		XMLNode* root;

		vector<Point> model;
		vector<Transformations> transforms;
		Transformations* t;
		if (!(doc.LoadFile(f.c_str()))) {
			root = doc.FirstChild(); // scene is root
			// for each <group>...</group>
			for (XMLElement* group = root->FirstChildElement("group"); group; group = group->NextSiblingElement("group")) {
				t = new Transformations();
				parseGroup(group, t);
				transforms.push_back(*t);
				//string file = elem->Attribute("file");
				//cout << "Trying to read: " << file << "..." << endl;
				//model = modelReader(DIR + file);
			}
		}
		else {
			cout << "ERROR!" << endl;
		}
		return transforms;
	}
}
