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
				file >> p.x >> p.y >> p.z >> p.nx >> p.ny >> p.nz;
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

	void parseRotate(XMLElement* rotate, Transformations* transforms) {
		float x, y, z;
		x = y = z = 0;
		rotate->QueryFloatAttribute("x",&x);
		rotate->QueryFloatAttribute("y",&y);
		rotate->QueryFloatAttribute("z",&z);
		Rotate* r;
		if (rotate->FindAttribute("time")) {
			float time = rotate->FloatAttribute("time");
			r = new Rotate(time, x, y, z, true);
		}
		else {
			float ang = rotate->FloatAttribute("angle");
			r = new Rotate(ang, x, y, z);
		}
		transforms->addRotate(r);
	}

	void parseTranslate(XMLElement* translate, Transformations* transforms, unsigned int* nFig) {
		if (translate->FindAttribute("time")) {
			float time = translate->FloatAttribute("time");
			int seg;
			bool traced = false;
			bool follows = false;
			if (translate->FindAttribute("trace")) { // does it have traced lines
				seg = translate->Int64Attribute("trace");
				traced = true;
			}
			else {
				seg = 0;
			}

			if (translate->FindAttribute("follow")) {
				string str = translate->Attribute("follow");
				follows = str.compare("yes") == 0 ? true : false;
			}

			vector<Point> controlPoints;
			for (XMLElement* controlPoint = translate->FirstChildElement("point"); controlPoint; controlPoint = controlPoint->NextSiblingElement("point")) {
				Point p;
				p.x = controlPoint->FloatAttribute("x");
				p.y = controlPoint->FloatAttribute("y");
				p.z = controlPoint->FloatAttribute("z");
				controlPoints.push_back(p);
			}
			if (traced) {
				float* rgb = new float[3]();
				rgb[0] = rgb[1] = rgb[2] = 255;
				translate->QueryFloatAttribute("r", &rgb[0]);
				translate->QueryFloatAttribute("g", &rgb[1]);
				translate->QueryFloatAttribute("b", &rgb[2]);
				transforms->addTranslate(new Translate(controlPoints, time, seg, *nFig, rgb, follows));
				(*nFig)++;
			}
			else {
				transforms->addTranslate(new Translate(controlPoints, time, seg, follows));
			}
		}
		else {
			float x, y, z;
			x = y = z = 0;
			translate->QueryFloatAttribute("x", &x);
			translate->QueryFloatAttribute("y", &y);
			translate->QueryFloatAttribute("z", &z);
			transforms->addTranslate(new Translate(x, y, z));
		}
	}

	void parseGroup(XMLElement* group, Transformations* transforms, unsigned int* nFig) {
		for (XMLElement* elem = group->FirstChildElement(); elem; elem = elem->NextSiblingElement()) { //for each element on group
			string type = elem->Value();
			if (type.compare("translate") == 0){ 
				parseTranslate(elem, transforms,nFig);
			}
			else if (type.compare("rotate") == 0) {
				parseRotate(elem, transforms);
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
				int rgb[3];
				rgb[0] = rgb[1] = rgb[2] = 255;
				elem->QueryIntAttribute("r", &rgb[0]);
				elem->QueryIntAttribute("g", &rgb[1]);
				elem->QueryIntAttribute("b", &rgb[2]);
				transforms->addRGB(rgb);
			}
			else {
				cout << "ERROR. <Group> has an invalid element: " << elem->Value() << ". Skipping element..."<< endl;
			}
		}
	}


	void parseLights(XMLElement* elem, vector<Light*>* lights) {
		Light* l;
		string type;
		float pos[4], diff[4], amb[4], spec[4];
		pos[0] = pos[1] = pos[2] = pos[3] = 0.0f;
		diff[0] = diff[1] = diff[2] = diff[3] = 1.0f;
		amb[0] = amb[1] = amb[2] = 0.2f; amb[3] = 1.0f;
		spec[0] = spec[1] = spec[2] = 0.0f; spec[3] = 1.0f;
		unsigned int id = 0;

		for (XMLElement* light = elem->FirstChildElement(); light; light = light->NextSiblingElement()) {
			if (id == 8) {
				cout << "WARNING: too many lights (>8). Ignoring the remaining lights..." << endl;
				break; 
			}
			light->QueryFloatAttribute("posX", &pos[0]);
			light->QueryFloatAttribute("posY", &pos[1]);
			light->QueryFloatAttribute("posZ", &pos[2]);

			light->QueryFloatAttribute("diffR", &diff[0]);
			light->QueryFloatAttribute("diffG", &diff[1]);
			light->QueryFloatAttribute("diffB", &diff[2]);

			light->QueryFloatAttribute("ambR", &amb[0]);
			light->QueryFloatAttribute("ambG", &amb[1]);
			light->QueryFloatAttribute("ambB", &amb[2]);

			light->QueryFloatAttribute("specR", &spec[0]);
			light->QueryFloatAttribute("specG", &spec[1]);
			light->QueryFloatAttribute("specB", &spec[2]);

			type = light->Attribute("type");

			if (type.compare("DIRECTIONAL")) {
				l = new DirectionalLight(id,pos,diff,amb,spec);
				lights->push_back(l);
			}
			else if (type.compare("POINT")) {
				float att = light->FloatAttribute("attenuation");
				l = new PointLight(id, pos, diff, amb, spec, att);
				lights->push_back(l);
			}
			else if (type.compare("SPOT")) {
				float dir[3];
				float cutoff = light->FloatAttribute("cutoff");
				float exp = light->FloatAttribute("exponent");

				dir[0] = dir[1] = dir[2] = 0;
				light->QueryFloatAttribute("dirX", &dir[0]);
				light->QueryFloatAttribute("dirY", &dir[1]);
				light->QueryFloatAttribute("dirZ", &dir[2]);

				l = new SpotLight(id, pos, diff, amb, spec, dir, cutoff, exp);
				lights->push_back(l);
			}
			else {
				cout << "ERROR. On <Lights>, element Light has an invalid type: " << type << ". Skipping current Light..." << endl;
			}
			id++;
		}
	}
	
	void xmlReader(string f, vector<Transformations*>* transforms, vector<Light*>* ls, unsigned int* nFig) {
		XMLDocument doc;
		XMLNode* root;
		XMLElement* lights;

		Transformations* t;
		if (!(doc.LoadFile(f.c_str()))) {
			root = doc.FirstChild(); // scene is root

			// for the lights
			lights = root->FirstChildElement("lights");
			if (lights) {
				parseLights(lights, ls);
			}

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
