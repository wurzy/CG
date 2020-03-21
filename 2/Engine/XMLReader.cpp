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
			cout << "ERROR" << endl;
		}
		file.close();
		return points;
	}

	void parseModel(XMLElement* models) {

	}

	Transformations parseGroup(XMLElement* group) {
		for (XMLElement* elem = group->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {

		}
	}
	
	vector<Transformations> xmlReader(string f) {
		XMLDocument doc;
		XMLNode* root;

		vector<Point> model;
		vector<Transformations> transforms;
		if (!(doc.LoadFile(f.c_str()))) {
			root = doc.FirstChild(); // scene is root
			// for each <group>...</group>
			for (XMLElement* group = root->FirstChildElement("group"); group; group = group->NextSiblingElement("group")) {
				transforms.push_back(parseGroup(group));
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
