#include "generator.h"

string dir = "./files/";

void drawSphere(double r, int stacks, int slices, string f) {
	ofstream file(dir + f);
	const float phi = 2 * M_PI / (float)slices, theta = M_PI / (float)stacks;
	float theta1, theta2, phi1, phi2;

	for (int j = 0; j < stacks; j++) {
		theta1 = theta * j;
		theta2 = theta * (j + 1.0f);

		for (int i = 0; i < slices; i++) {
			phi1 = phi * i;
			phi2 = phi * (i + 1.0f);

			/*pontos   1 . ---- . 4 theta1
						 |      |
						 |      |
					   2 . ---- . 3 theta2
					  phi1     phi2
			*/
			file << r * sinf(theta1) * sinf(phi1) << " " << r * cosf(theta1) << " " << r * sinf(theta1) * cosf(phi1) << endl;
			file << r * sinf(theta2) * sinf(phi1) << " " << r * cosf(theta2) << " " << r * sinf(theta2) * cosf(phi1) << endl;
			file << r * sinf(theta2) * sinf(phi2) << " " << r * cosf(theta2) << " " << r * sinf(theta2) * cosf(phi2) << endl;

			file << r * sinf(theta2) * sinf(phi2) << " " << r * cosf(theta2) << " " << r * sinf(theta2) * cosf(phi2) << endl;
			file << r * sinf(theta1) * sinf(phi2) << " " << r * cosf(theta1) << " " << r * sinf(theta1) * cosf(phi2) << endl;
			file << r * sinf(theta1) * sinf(phi1) << " " << r * cosf(theta1) << " " << r * sinf(theta1) * cosf(phi1) << endl;
		}
	}
	file.close();
}

void drawBox(float x, float y, float z, int d, string f) {
	ofstream file(dir + f);
	int div = d + 1; // 3 camadas, 2 divisoes
	float* xs = new float[div + 1]; // 5 pontos = 3 divisoes e 2 nas pontas
	float* ys = new float[div + 1];
	float* zs = new float[div + 1];
	float stepX = x / div; // steps ao longo do eixo pos -> neg
	float stepY = y / div;
	float stepZ = z / div;
	x = x / 2; //centrar
	y = y / 2;
	z = z / 2;
	for (int i = 0; i < div + 1; i++) {
		xs[i] = x - (stepX * i); // inicializar array com todos os pontos possiveis
		ys[i] = y - (stepY * i);
		zs[i] = z - (stepZ * i);
	}

	for (int i = 0; i < div; i++) {
		for (int j = 0; j < div; j++) {
			//face de cima, construir assim: para cada X, fazer uma fila de Z's, locked y
			file << xs[i + 1] << " " << ys[0] << " " << zs[j] << endl;
			file << xs[i] << " " << ys[0] << " " << zs[j] << endl;
			file << xs[i] << " " << ys[0] << " " << zs[j + 1] << endl;

			file << xs[i] << " " << ys[0] << " " << zs[j + 1] << endl;
			file << xs[i + 1] << " " << ys[0] << " " << zs[j + 1] << endl;
			file << xs[i + 1] << " " << ys[0] << " " << zs[j] << endl;

			//face de baixo, locked z

			file << xs[i+1] << " " << ys[div] << " " << zs[j] << endl;
			file << xs[i] << " " << ys[div] << " " << zs[j + 1] << endl;
			file << xs[i] << " " << ys[div] << " " << zs[j] << endl;

			file << xs[i + 1] << " " << ys[div] << " " << zs[j] << endl;
			file << xs[i + 1] << " " << ys[div] << " " << zs[j + 1] << endl;
			file << xs[i] << " " << ys[div] << " " << zs[j+1] << endl;
			//face da frente, locked z

			file << xs[i + 1] << " " << ys[j+1] << " " << zs[div] << endl;
			file << xs[i + 1] << " " << ys[j] << " " << zs[div] << endl;
			file << xs[i] << " " << ys[j+1] << " " << zs[div] << endl;

			file << xs[i + 1] << " " << ys[j] << " " << zs[div] << endl;
			file << xs[i] << " " << ys[j] << " " << zs[div] << endl;
			file << xs[i] << " " << ys[j + 1] << " " << zs[div] << endl;

			//face de tras, locked z

			file << xs[i] << " " << ys[j] << " " << zs[0] << endl;
			file << xs[i+1] << " " << ys[j] << " " << zs[0] << endl;
			file << xs[i+1] << " " << ys[j + 1] << " " << zs[0] << endl;
			
			file << xs[i] << " " << ys[j] << " " << zs[0] << endl;
			file << xs[i + 1] << " " << ys[j+1] << " " << zs[0] << endl;
			file << xs[i] << " " << ys[j + 1] << " " << zs[0] << endl;

			//face do lado dir, locked x

			file << xs[0] << " " << ys[i + 1] << " " << zs[j] << endl;
			file << xs[0] << " " << ys[i + 1] << " " << zs[j + 1] << endl;
			file << xs[0] << " " << ys[i] << " " << zs[j] << endl;

			file << xs[0] << " " << ys[i+1] << " " << zs[j+1] << endl;
			file << xs[0] << " " << ys[i] << " " << zs[j+1] << endl;
			file << xs[0] << " " << ys[i] << " " << zs[j] << endl;

			//face do lado esq, locked x

			file << xs[div] << " " << ys[i + 1] << " " << zs[j+1] << endl;
			file << xs[div] << " " << ys[i+1] << " " << zs[j] << endl;
			file << xs[div] << " " << ys[i] << " " << zs[j] << endl;

			file << xs[div] << " " << ys[i + 1] << " " << zs[j+1] << endl;
			file << xs[div] << " " << ys[i] << " " << zs[j] << endl;
			file << xs[div] << " " << ys[i] << " " << zs[j+1] << endl;
		}
	}
	file.close();
}

void drawPlane(float side1, float side2, int axis, string f) {
	ofstream file(dir + f);
	float c1 = side1 / 2.0f;
	float c2 = side2 / 2.0f;
	switch (axis) {
		case 0: // default do enunciado, xOz
			//triangulo 1

			file << -c1 << " " << 0 << " " << -c2 << endl;
			file << c1 << " " << 0 << " " << c2 << endl;
			file << c1 << " " << 0 << " " << -c2 << endl;
			////triangulo 2

			file << -c1 << " " << 0 << " " << -c2 << endl;
			file << -c1 << " " << 0 << " " << c2 << endl;
			file << c1 << " " << 0 << " " << c2 << endl;
			break;
		case 1: // eixo x0y
			//triangulo 1

			file << c1 << " " << c2 << " " << 0 << endl;
			file << -c1 << " " << c2 << " " << 0 << endl;
			file << c1 << " " << -c2 << " " << 0 << endl;
			////triangulo 2

			file << -c1 << " " << c2 << " " << 0 << endl;
			file << -c1 << " " << -c2 << " " << 0 << endl;
			file << c1 << " " << -c2 << " " << 0 << endl;
			break;
		case 2: // eixo yOz
			//triangulo 1

			file << 0 << " " << c1 << " " << c2 << endl;
			file << 0 << " " << -c1 << " " << c2 << endl;
			file << 0 << " " << -c1 << " " << -c2 << endl;
			////triangulo 2

			file << 0 << " " << c1 << " " << c2 << endl;
			file << 0 << " " << -c1 << " " << -c2 << endl;
			file << 0 << " " << c1 << " " << -c2 << endl;
			break;
		default: 
			break;
	}
	file.close();
}

void drawCone(float radius, float h, int slices, int stacks, string f) {
	ofstream file(dir + f);
	// fat -> angulo entre de cada fatia
	// cam -> altura de cada camada
	float fat = 2 * M_PI / slices, cam = h / stacks;
	float fat1, fat2, cam1, cam2, k, j;
	float alt = -(h/2); // centrado
	float ang;

	for (int n = 0; n < slices; n++) {
		ang = fat * n; // angulo fatia atual
		k = ang + fat; // angulo fatia seguinte

		file << 0 << " " << alt << " " << 0 << endl;
		file << radius * sin(k) << " " << alt << " " << radius * cos(k) << endl;
		file << radius * sin(ang) << " " << alt << " " << radius * cos(ang) << endl;
	}

	for (int i = 0; i < stacks; i++) {

		cam1 = alt + (i * cam); //altura camada atual
		cam2 = alt + ((i + 1) * cam); //altura camada seguinte

		fat1 = radius - ((radius / stacks) * i);  //raio da fatia atual
		fat2 = radius - ((radius / stacks) * (i + 1));  //raio da fatia seguinte

		for (int n = 0; n < slices; n++) {
			j = fat * n; //angulo fatia atual
			k = j + fat; //angulo fatia seguinte 

			file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << endl;
			file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << endl;
			file << fat2 * sin(j) << " " << cam2 << " " << fat2 * cos(j) << endl;

			file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << endl;
			file << fat1 * sin(k) << " " << cam1 << " " << fat1 * cos(k) << endl;
			file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << endl;

		}
	}
	file.close();
}

void drawCylinder(float radius, float height, int slices, int stacks, string f) {
	ofstream file(dir + f);
	float x1, x2, z1, z2, angle, h, h_stack, h_it1, h_it2;
	angle = (2 * M_PI) / slices;
	h = height / 2;
	h_stack = height / stacks;

	for (int n = 0; n < slices; n++) {
		x1 = radius * sinf(angle * n);
		x2 = radius * sinf(angle * (n + 1.0f));

		z1 = radius * cosf(angle * n);
		z2 = radius * cosf(angle * (n + 1.0f));

		//base (para so alternar as cores, podia ficar no outro ciclo
		file << 0 << " " << -h << " " << 0 << endl;
		file << x2 << " " << -h << " " << z2 << endl;
		file << x1 << " " << -h << " " << z1 << endl;
	}
	for (int n = 0; n < slices; n++) {
		x1 = radius * sinf(angle * n);
		x2 = radius * sinf(angle * (n + 1.0f));

		z1 = radius * cosf(angle * n);
		z2 = radius * cosf(angle * (n + 1.0f));

		for (int j = 0; j < stacks; j++) {
			//lateral
			/*pontos   1 . ---- . 4 h_it1
						 |      |
						 |      |
					   2 . ---- . 3 h_it2
					   x1,z1  x2,z2

			*/
			h_it1 = h - (h_stack * j);
			h_it2 = h - (h_stack * (j + 1));

			file << x1 << " " << h_it1 << " " << z1 << endl;
			file << x1 << " " << h_it2 << " " << z1 << endl;
			file << x2 << " " << h_it2 << " " << z2 << endl;

			file << x1 << " " << h_it1 << " " << z1 << endl;
			file << x2 << " " << h_it2 << " " << z2 << endl;
			file << x2 << " " << h_it1 << " " << z2 << endl;
		}
	}
	// mesma historia que o de cima, podia ficar dentro mas e so para alternar cores
	for (int n = 0; n < slices; n++) {
		x1 = radius * sinf(angle * n);
		x2 = radius * sinf(angle * (n + 1.0f));

		z1 = radius * cosf(angle * n);
		z2 = radius * cosf(angle * (n + 1.0f));

		file << 0 << " " << h << " " << 0 << endl;
		file << x1 << " " << h << " " << z1 << endl;
		file << x2 << " " << h << " " << z2 << endl;
	}
	file.close();
}

void drawVase(float radius, float radius2, float h, int slices, int stacks, string f) {
	ofstream file(dir + f);
	//radius -> raio base cima
	//radius2 -> raio base baixo
	//h -> altura 

	float fat = 2 * M_PI / slices, cam = h / stacks;
	float fat1, fat2, cam1, cam2, ang, k, j;
	float alt = -(h / 2);

	//bases
	for (int n = 0; n < slices; n++) {
		ang = fat * n;
		k = ang + fat;

		file << 0 << " " << -alt << " " << 0 << endl;
		file << radius * sin(ang) << " " << -alt << " " << radius * cos(ang) << endl;
		file << radius * sin(k) << " " << -alt << " " << radius * cos(k) << endl;
	}

	for (int n = 0; n < slices; n++) {
		ang = fat * n;
		k = ang + fat;

		file << 0 << " " << alt << " " << 0 << endl;
		file << radius2 * sin(k) << " " << alt << " " << radius2 * cos(k) << endl;
		file << radius2 * sin(ang) << " " << alt << " " << radius2 * cos(ang) << endl;
	}

	//laterais
	for (int i = 0; i < stacks; i++) {

		cam1 = alt + (i * cam);
		cam2 = alt + ((i + 1) * cam);
		fat1 = radius2 + (radius - radius2) / stacks * i;
		fat2 = radius2 + (radius - radius2) / stacks * (i + 1);

		for (int n = 0; n < slices; n++) {
			j = fat * n;
			k = j + fat;

			file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << endl;
			file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << endl;
			file << fat2 * sin(j) << " " << cam2 << " " << fat2 * cos(j) << endl;

			file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << endl;
			file << fat1 * sin(k) << " " << cam1 << " " << fat1 * cos(k) << endl;
			file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << endl;
		}

	}
	file.close();
}


void parseInput(int argc, char** argv) {
	if (string(argv[1]) == "sphere") {
		if (argc != 6) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Sphere." << endl;
			drawSphere(stod(argv[2]), atoi(argv[3]), atoi(argv[4]), argv[5]);
		}
	}
	else if (string(argv[1]) == "cylinder") {
		if(argc!=7){
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Cylinder." << endl;
			drawCylinder(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
		}

	}
	else if (string(argv[1]) == "plane") {
		if (argc != 6) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Plane." << endl;
			drawPlane(atof(argv[2]), atof(argv[3]), atof(argv[4]), argv[5]);
		}
	}
	else if (string(argv[1]) == "cone") {
		if (argc != 7) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Cone." << endl;
			drawCone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
		}
	}
	else if (string(argv[1]) == "box") {
		if (argc != 7) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Box." << endl;
			drawBox(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), argv[6]);
		}
	}
	else if (string(argv[1]) == "vase") {
		if (argc!=8) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Vase." << endl;
			drawVase(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
		}
	}
}


int main(int argc, char** argv) {
	_mkdir(dir.c_str());
	if (argc == 0 || argc > 8 ) {
		cout << "ERROR" << endl;
	}
	else{
		parseInput(argc, argv);
	}
	cout << "DONE!" << endl;
	return 1;
}