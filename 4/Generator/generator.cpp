#include "generator.h"

string dir = "./files/";

void makeVetor(float* a, float* b, float* res) {
	res[0] = b[0] - a[0];
	res[1] = b[1] - a[1];
	res[2] = b[2] - a[2];
}

void cross(float* a, float* b, float* res) {

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}


void normalize(float* a) {

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}


void drawSphere(double r, int stacks, int slices, string f) {
	ofstream file(dir + f);
	const float phi = 2 * M_PI / (float)slices, theta = M_PI / (float)stacks;
	float theta1, theta2, phi1, phi2;
	float ladoInc = 1.0 / slices;
	float cimaInc = 1.0 / slices;
	float cima1, cima2, lado1, lado2;

	for (int j = 0; j < stacks; j++) {
		theta1 = theta * j;
		theta2 = theta * (j + 1.0f);
		cima1 = stacks - cimaInc * j;
		cima2 = stacks - cimaInc * (j + 1.0f);

		for (int i = 0; i < slices; i++) {
			phi1 = phi * i;
			phi2 = phi * (i + 1.0f);
			lado1 = ladoInc * i;
			lado2 = ladoInc * (i + 1.0f);

			float val1 = r * sinf(theta1) * sinf(phi1), val2 = r * sinf(theta1) * cosf(phi1);
			float val3 = r * sinf(theta2) * sinf(phi1), val4 = r * sinf(theta2) * cosf(phi1);
			float val5 = r * sinf(theta2) * sinf(phi2), val6 = r * sinf(theta2) * cosf(phi2);
			float val9 = r * sinf(theta1) * sinf(phi2), val10 = r * sinf(theta1) * cosf(phi2);
			float val7 = r * cosf(theta1), val8 = r * cosf(theta2);

			float l1 = sqrt(val1 * val1 + val7 * val7 + val2 * val2);
			float l2 = sqrt(val3 * val3 + val8 * val8 + val4 * val4);
			float l3 = sqrt(val5 * val5 + val8 * val8 + val6 * val6);

			float l4 = sqrt(val5 * val5 + val8 * val8 + val6 * val6);
			float l5 = sqrt(val9 * val9 + val7 * val7 + val10 * val10);
			float l6 = sqrt(val1 * val1 + val7 * val7 + val2 * val2);


			/*pontos   1 . ---- . 4 theta1
						 |      |
						 |      |
					   2 . ---- . 3 theta2
					  phi1     phi2
			*/
			file << val1 << " " << val7 << " " << val2 << " " << val1 / l1 << " " << val7 / l1 << " " << val2 / l1 << " " << cima1 << " " << lado1 << endl;
			file << val3 << " " << val8 << " " << val4 << " " << val3 / l2 << " " << val8 / l2 << " " << val4 / l2 << " " << cima2 << " " << lado1 << endl;
			file << val5 << " " << val8 << " " << val6 << " " << val5 / l3 << " " << val8 / l3 << " " << val6 / l3 << " " << cima2 << " " << lado2 << endl;

			file << val5 << " " << val8 << " " << val6 << " " << val5 / l4 << " " << val8 / l4 << " " << val6 / l4 << " " << cima2 << " " << lado2 << endl;
			file << val9 << " " << val7 << " " << val10 << " " << val9 / l5 << " " << val7 / l5 << " " << val10 / l5 << " " << cima1 << " " << lado2 << endl;
			file << val1 << " " << val7 << " " << val2 << " " << val1 / l6 << " " << val7 / l6 << " " << val2 / l6 << " " << cima1 << " " << lado1 << endl;
		}
	}
	file.close();
}
/*
void drawSphere(double r, int stacks, int slices, string f) {
	ofstream file(dir + f);
	const float phi = 2 * M_PI / (float)slices, theta = M_PI / (float)stacks;
	float theta1, theta2, phi1, phi2;
	float cima1, cima2, lado1, lado2;

	float ladoInc = 1.0 / slices;
	float cimaInc = 1.0 / stacks;

	for (int j = 0; j < stacks; j++) {
		theta1 = theta * j;
		theta2 = theta * (j + 1.0f);

		cima1 = stacks - cimaInc * j;
		cima2 = stacks - cimaInc * (j + 1.0f);

		for (int i = 0; i < slices; i++) {
			phi1 = phi * i;
			phi2 = phi * (i + 1.0f);
			lado1 = ladoInc * i;
			lado2 = ladoInc * (i + 1.0f);

			pontos   1 . ---- . 4 theta1
						 |      |
						 |      |
					   2 . ---- . 3 theta2
					  phi1     phi2
			

			float* n1 = new float[3];
			n1[0] = r * sinf(theta1) * sinf(phi1);
			n1[1] = r * cosf(theta1);
			n1[2] = r * sinf(theta1) * cosf(phi1);
			normalize(n1);

			float* n2 = new float[3];
			n2[0] = r * sinf(theta2) * sinf(phi1);
			n2[1] = r * cosf(theta2);
			n2[2] = r * sinf(theta2) * cosf(phi1);
			normalize(n2);

			float* n3 = new float[3];
			n3[0] = r * sinf(theta2) * sinf(phi2);
			n3[1] = r * cosf(theta2);
			n3[2] = r * sinf(theta2) * cosf(phi2);
			normalize(n3);

			file << r * sinf(theta1) * sinf(phi1) << " " << r * cosf(theta1) << " " << r * sinf(theta1) * cosf(phi1)
				<< " " << n1[0] << " " << n1[1] << " " << n1[2] << " " << cima1 << " " << lado1 << endl;
			file << r * sinf(theta2) * sinf(phi1) << " " << r * cosf(theta2) << " " << r * sinf(theta2) * cosf(phi1) 
				<< " " << n2[0] << " " << n2[1] << " " << n2[2] << " " << cima2 << " " << lado1 << endl;
			file << r * sinf(theta2) * sinf(phi2) << " " << r * cosf(theta2) << " " << r * sinf(theta2) * cosf(phi2) 
				<< " " << n3[0] << " " << n3[1] << " " << n3[2] << " " << cima2 << " " << lado2 << endl;

			n1[0] = r * sinf(theta2) * sinf(phi2);
			n1[1] = r * cosf(theta2);
			n1[2] = r * sinf(theta2) * cosf(phi2);
			normalize(n1);

			n2[0] = r * sinf(theta1) * sinf(phi2);
			n2[1] = r * cosf(theta1);
			n2[2] = r * sinf(theta1) * cosf(phi2);
			normalize(n2);

			n3[0] = r * sinf(theta1) * sinf(phi1);
			n3[1] = r * cosf(theta1);
			n3[2] = r * sinf(theta1) * cosf(phi1);
			normalize(n3);

			file << r * sinf(theta2) * sinf(phi2) << " " << r * cosf(theta2) << " " << r * sinf(theta2) * cosf(phi2)
				<< " " << n1[0] << " " << n1[1] << " " << n1[2] << " " << cima2 << " " << lado2 << endl;
			file << r * sinf(theta1) * sinf(phi2) << " " << r * cosf(theta1) << " " << r * sinf(theta1) * cosf(phi2)
				<< " " << n2[0] << " " << n2[1] << " " << n2[2] << " " << cima1 << " " << lado2 << endl;
			file << r * sinf(theta1) * sinf(phi1) << " " << r * cosf(theta1) << " " << r * sinf(theta1) * cosf(phi1)
				<< " " << n3[0] << " " << n3[1] << " " << n3[2] << " " << cima1 << " " << lado1 << endl;
		}
	}
	file.close();
}
*/
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
			file << xs[i + 1] << " " << ys[0] << " " << zs[j] << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[0] << " " << zs[j] << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[0] << " " << zs[j + 1] << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;

			file << xs[i] << " " << ys[0] << " " << zs[j + 1] << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[0] << " " << zs[j + 1] << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[0] << " " << zs[j] << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;

			//face de baixo, locked z

			file << xs[i + 1] << " " << ys[div] << " " << zs[j] << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[div] << " " << zs[j + 1] << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[div] << " " << zs[j] << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;

			file << xs[i + 1] << " " << ys[div] << " " << zs[j] << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[div] << " " << zs[j + 1] << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[div] << " " << zs[j + 1] << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
			//face da frente, locked z

			file << xs[i + 1] << " " << ys[j + 1] << " " << zs[div] << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[j] << " " << zs[div] << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[j + 1] << " " << zs[div] << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;

			file << xs[i + 1] << " " << ys[j] << " " << zs[div] << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[j] << " " << zs[div] << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[j + 1] << " " << zs[div] << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;

			//face de tras, locked z

			file << xs[i] << " " << ys[j] << " " << zs[0] << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[j] << " " << zs[0] << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[j + 1] << " " << zs[0] << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << endl;

			file << xs[i] << " " << ys[j] << " " << zs[0] << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << endl;
			file << xs[i + 1] << " " << ys[j + 1] << " " << zs[0] << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << endl;
			file << xs[i] << " " << ys[j + 1] << " " << zs[0] << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << endl;

			//face do lado dir, locked x

			file << xs[0] << " " << ys[i + 1] << " " << zs[j] << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[0] << " " << ys[i + 1] << " " << zs[j + 1] << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[0] << " " << ys[i] << " " << zs[j] << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;

			file << xs[0] << " " << ys[i + 1] << " " << zs[j + 1] << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[0] << " " << ys[i] << " " << zs[j + 1] << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[0] << " " << ys[i] << " " << zs[j] << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;

			//face do lado esq, locked x

			file << xs[div] << " " << ys[i + 1] << " " << zs[j + 1] << " " << -1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[div] << " " << ys[i + 1] << " " << zs[j] << " " << -1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[div] << " " << ys[i] << " " << zs[j] << " " << -1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;

			file << xs[div] << " " << ys[i + 1] << " " << zs[j + 1] << " " << -1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[div] << " " << ys[i] << " " << zs[j] << " " << -1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
			file << xs[div] << " " << ys[i] << " " << zs[j + 1] << " " << -1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
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

		file << -c1 << " " << 0 << " " << -c2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << c1 << " " << 0 << " " << c2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << c1 << " " << 0 << " " << -c2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		////triangulo 2

		file << -c1 << " " << 0 << " " << -c2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << -c1 << " " << 0 << " " << c2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << c1 << " " << 0 << " " << c2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		break;
	case 1: // eixo x0y
		//triangulo 1

		file << c1 << " " << c2 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
		file << -c1 << " " << c2 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
		file << c1 << " " << -c2 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
		////triangulo 2

		file << -c1 << " " << c2 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
		file << -c1 << " " << -c2 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
		file << c1 << " " << -c2 << " " << 0 << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << endl;
		break;
	case 2: // eixo yOz
		//triangulo 1

		file << 0 << " " << c1 << " " << c2 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << 0 << " " << -c1 << " " << c2 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << 0 << " " << -c1 << " " << -c2 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
		////triangulo 2

		file << 0 << " " << c1 << " " << c2 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << 0 << " " << -c1 << " " << -c2 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << 0 << " " << c1 << " " << -c2 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
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
	float alt = -(h / 2); // centrado
	float ang, rsin1, rsin2, rcos1, rcos2;

	for (int n = 0; n < slices; n++) {
		ang = fat * n; // angulo fatia atual
		k = ang + fat; // angulo fatia seguinte
		rsin1 = radius * sin(k);
		rsin2 = radius * sin(ang);
		rcos1 = radius * cos(k);
		rcos2 = radius * cos(ang);

		file << 0 << " " << alt << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << rsin1 << " " << alt << " " << rcos1 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << rsin2 << " " << alt << " " << rcos2 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
	}

	for (int i = 0; i < stacks; i++) {

		cam1 = alt + (i * cam); //altura camada atual
		cam2 = alt + ((i + 1) * cam); //altura camada seguinte

		fat1 = radius - ((radius / stacks) * i);  //raio da fatia atual
		fat2 = radius - ((radius / stacks) * (i + 1));  //raio da fatia seguinte

		for (int n = 0; n < slices; n++) {
			j = fat * n; //angulo fatia atual
			k = j + fat; //angulo fatia seguinte 

			file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << " " << sin(j) << " " << 0 << " " << cos(j) << " " << 0 << " " << 0 << endl;
			file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << " " << sin(k) << " " << 0 << " " << cos(k) << " " << 0 << " " << 0 << endl;
			file << fat2 * sin(j) << " " << cam2 << " " << fat2 * cos(j) << " " << sin(j) << " " << 0 << " " << cos(j) << " " << 0 << " " << 0 << endl;

			file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << " " << sin(j) << " " << 0 << " " << cos(j) << " " << 0 << " " << 0 << endl;
			file << fat1 * sin(k) << " " << cam1 << " " << fat1 * cos(k) << " " << sin(k) << " " << 0 << " " << cos(k) << " " << 0 << " " << 0 << endl;
			file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << " " << sin(k) << " " << 0 << " " << cos(k) << " " << 0 << " " << 0 << endl;

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
		file << 0 << " " << -h << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << x2 << " " << -h << " " << z2 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << x1 << " " << -h << " " << z1 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
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

			file << x1 << " " << h_it1 << " " << z1 << " " << sinf(angle * n) << " " << 0.0 << " " << cosf(angle * n) << " " << 0 << " " << 0 << endl;
			file << x1 << " " << h_it2 << " " << z1 << " " << sinf(angle * n) << " " << 0.0 << " " << cosf(angle * n) << " " << 0 << " " << 0 << endl;
			file << x2 << " " << h_it2 << " " << z2 << " " << sinf(angle * (n + 1.0f)) << " " << 0.0 << " " << cosf(angle * (n + 1.0f)) << " " << 0 << " " << 0 << endl;

			file << x1 << " " << h_it1 << " " << z1 << " " << sinf(angle * n) << " " << 0.0 << " " << cosf(angle * n) << " " << 0 << " " << 0 << endl;
			file << x2 << " " << h_it2 << " " << z2 << " " << sinf(angle * (n + 1.0f)) << " " << 0.0 << " " << cosf(angle * (n + 1.0f)) << " " << 0 << " " << 0 << endl;
			file << x2 << " " << h_it1 << " " << z2 << " " << sinf(angle * (n + 1.0f)) << " " << 0.0 << " " << cosf(angle * (n + 1.0f)) << " " << 0 << " " << 0 << endl;
		}
	}
	// mesma historia que o de cima, podia ficar dentro mas e so para alternar cores
	for (int n = 0; n < slices; n++) {
		x1 = radius * sinf(angle * n);
		x2 = radius * sinf(angle * (n + 1.0f));

		z1 = radius * cosf(angle * n);
		z2 = radius * cosf(angle * (n + 1.0f));

		file << 0 << " " << h << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << x1 << " " << h << " " << z1 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << x2 << " " << h << " " << z2 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
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

		file << 0 << " " << -alt << " " << 0 << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << radius * sin(ang) << " " << -alt << " " << radius * cos(ang) << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << radius * sin(k) << " " << -alt << " " << radius * cos(k) << " " << 0 << " " << 1 << " " << 0 << " " << 0 << " " << 0 << endl;
	}

	for (int n = 0; n < slices; n++) {
		ang = fat * n;
		k = ang + fat;

		file << 0 << " " << alt << " " << 0 << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << radius2 * sin(k) << " " << alt << " " << radius2 * cos(k) << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;
		file << radius2 * sin(ang) << " " << alt << " " << radius2 * cos(ang) << " " << 0 << " " << -1 << " " << 0 << " " << 0 << " " << 0 << endl;

		//laterais
		for (int i = 0; i < stacks; i++) {

			cam1 = alt + (i * cam);
			cam2 = alt + ((i + 1) * cam);
			fat1 = radius2 + (radius - radius2) / stacks * i;
			fat2 = radius2 + (radius - radius2) / stacks * (i + 1);

			for (int n = 0; n < slices; n++) {
				j = fat * n;
				k = j + fat;

				file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << " " << sin(j) << " " << 0 << " " << cos(j) << " " << 0 << " " << 0 << endl;
				file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << " " << sin(k) << " " << 0 << " " << cos(k) << " " << 0 << " " << 0 << endl;
				file << fat2 * sin(j) << " " << cam2 << " " << fat2 * cos(j) << " " << sin(j) << " " << 0 << " " << cos(j) << " " << 0 << " " << 0 << endl;

				file << fat1 * sin(j) << " " << cam1 << " " << fat1 * cos(j) << " " << sin(j) << " " << 0 << " " << cos(j) << " " << 0 << " " << 0 << endl;
				file << fat1 * sin(k) << " " << cam1 << " " << fat1 * cos(k) << " " << sin(k) << " " << 0 << " " << cos(k) << " " << 0 << " " << 0 << endl;
				file << fat2 * sin(k) << " " << cam2 << " " << fat2 * cos(k) << " " << sin(k) << " " << 0 << " " << cos(k) << " " << 0 << " " << 0 << endl;
			}
		}
	}
	file.close();
}

// rc = radius inside the ring, ro = radius of the ring
void drawRing(float rc, float ro, int sides, int rings, string f) {
	ofstream file(dir + f);
	float stepTheta = (2 * M_PI) / sides;
	float stepPhi = (2 * M_PI) / rings;
	float theta1, theta2, phi1, phi2, x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4, n1, n2, n3, n4;
	float r1[3], r2[3], r3[3], r4[3], r5[3], r6[3];

	for (int i = 0; i < rings; i++) {
		theta1 = stepTheta * i;
		theta2 = stepTheta * (i + 1.0f); // outer ring

		for (int j = 0; j < sides; j++) {
			phi1 = stepPhi * j;
			phi2 = stepPhi * (j + 1.0f); // inner ring

			x1 = (rc + ro * cosf(phi1)) * cosf(theta1);
			y1 = (rc + ro * cosf(phi1)) * sinf(theta1);
			z1 = ro * sin(phi1);

			x2 = (rc + ro * cosf(phi2)) * cosf(theta1);
			y2 = (rc + ro * cosf(phi2)) * sinf(theta1);
			z2 = ro * sin(phi2);

			x3 = (rc + ro * cosf(phi1)) * cosf(theta2);
			y3 = (rc + ro * cosf(phi1)) * sinf(theta2);
			z3 = ro * sin(phi1);

			x4 = (rc + ro * cosf(phi2)) * cosf(theta2);
			y4 = (rc + ro * cosf(phi2)) * sinf(theta2);
			z4 = ro * sin(phi2);

			float p1[3] = { x1, y1 , z1 };
			float p2[3] = { x2, y2 , z2 };
			float p3[3] = { x3, y3 , z3 };
			float p4[3] = { x4, y4 , z4 };

			makeVetor(p1, p3, r1);
			makeVetor(p1, p4, r2);
			cross(r1, r2, r5);
			normalize(r5);

			makeVetor(p4, p2, r3);
			makeVetor(p4, p1, r4);
			cross(r3, r4, r6);
			normalize(r6);

			file << x1 << " " << y1 << " " << z1 << " " << r5[0] << " " << r5[1] << " " << r5[2] << " " << 0 << " " << 0 << endl;
			file << x3 << " " << y3 << " " << z3 << " " << r5[0] << " " << r5[1] << " " << r5[2] << " " << 0 << " " << 0 << endl;
			file << x4 << " " << y4 << " " << z4 << " " << r5[0] << " " << r5[1] << " " << r5[2] << " " << 0 << " " << 0 << endl;

			file << x4 << " " << y4 << " " << z4 << " " << r6[0] << " " << r6[1] << " " << r6[2] << " " << 0 << " " << 0 << endl;
			file << x2 << " " << y2 << " " << z2 << " " << r6[0] << " " << r6[1] << " " << r6[2] << " " << 0 << " " << 0 << endl;
			file << x1 << " " << y1 << " " << z1 << " " << r6[0] << " " << r6[1] << " " << r6[2] << " " << 0 << " " << 0 << endl;
		}
	}
	file.close();
}

//------------------------------------------NEW CODE-------------------------------------------

void transpose(float* a, float* b) { // apenas para 4x4
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			b[4 * i + j] = a[4 * j + i];
		}
	}
}

//multiplica qualquer matriz x[a][c] * y[c][b] = res [a][b]
void multMatrix(float* m1, float* m2, float* res, int a, int b, int c) {
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			res[4 * i + j] = 0;
			for (int k = 0; k < c; k++) {
				if (a == 1 && b == 1) { res[4 * i + j] += m1[4 * i + k] * m2[4 * j + k]; }
				else { res[4 * i + j] += m1[4 * i + k] * m2[4 * k + j]; }
			}
		}
	}
}

void bezierPatches(float t, float v, int* indicesCP, float** valuesCP, float* coord, float* normal) {

	float cpX[4][4], cpY[4][4], cpZ[4][4];

	//coord
	float res[1][4], resX[1][4], resY[1][4], resZ[1][4];
	float resTX[1][4], resTY[1][4], resTZ[1][4];
	float x[1], y[1], z[1];

	//devU
	float resU[1][4], resUX[1][4], resUY[1][4], resUZ[1][4];
	float resUTX[1][4], resUTY[1][4], resUTZ[1][4];
	float Ux[1], Uy[1], Uz[1];

	//devV
	float resV[1][4], resVX[1][4], resVY[1][4], resVZ[1][4];
	float resVTX[1][4], resVTY[1][4], resVTZ[1][4];
	float Vx[1], Vy[1], Vz[1];


	float U[1][4] = { { pow(t,3) , pow(t,2) , t , 1 } };

	float devU[1][4] = { { 3 * pow(t,2) , 2 * pow(t,1) , 1 , 0 } };

	float M[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
					  { 3.0f, -6.0f,  3.0f,  0.0f},
					  {-3.0f,  3.0f,  0.0f,  0.0f},
					  { 1.0f,  0.0f,  0.0f,  0.0f} };

	float MT[4][4];
	transpose(*M, *MT);

	float V[4][1] = { {pow(v,3)}, {pow(v,2)}, {v}, {1} };

	float devV[4][1] = { {3 * pow(v,2)}, {2 * pow(v,1)}, {1}, {0} };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cpX[i][j] = valuesCP[indicesCP[i * 4 + j]][0];
			cpY[i][j] = valuesCP[indicesCP[i * 4 + j]][1];;
			cpZ[i][j] = valuesCP[indicesCP[i * 4 + j]][2];;
		}
	}


	// Formula -> coord = U * M * P * MT * V
	//agora que temos as componentes todas da formula estamos prontos para calcular

	// aXc * cXb
	//1X4 * 4X4  a=1;c=4;b=4
	multMatrix(*U, *M, *res, 1, 4, 4); //coord
	multMatrix(*devU, *M, *resU, 1, 4, 4); //devU
	multMatrix(*U, *M, *resV, 1, 4, 4); //devV

	//1X4 * 4X4  a=1;c=4;b=4
	//coord
	multMatrix(*res, *cpX, *resX, 1, 4, 4);
	multMatrix(*res, *cpY, *resY, 1, 4, 4);
	multMatrix(*res, *cpZ, *resZ, 1, 4, 4);

	//devU
	multMatrix(*resU, *cpX, *resUX, 1, 4, 4);
	multMatrix(*resU, *cpY, *resUY, 1, 4, 4);
	multMatrix(*resU, *cpZ, *resUZ, 1, 4, 4);

	//devV
	multMatrix(*resV, *cpX, *resVX, 1, 4, 4);
	multMatrix(*resV, *cpY, *resVY, 1, 4, 4);
	multMatrix(*resV, *cpZ, *resVZ, 1, 4, 4);

	//1X4 * 4X4  a=1;c=4;b=4
	//coord
	multMatrix(*resX, *MT, *resTX, 1, 4, 4);
	multMatrix(*resY, *MT, *resTY, 1, 4, 4);
	multMatrix(*resZ, *MT, *resTZ, 1, 4, 4);

	//devU
	multMatrix(*resUX, *MT, *resUTX, 1, 4, 4);
	multMatrix(*resUY, *MT, *resUTY, 1, 4, 4);
	multMatrix(*resUZ, *MT, *resUTZ, 1, 4, 4);

	//devV
	multMatrix(*resVX, *MT, *resVTX, 1, 4, 4);
	multMatrix(*resVY, *MT, *resVTY, 1, 4, 4);
	multMatrix(*resVZ, *MT, *resVTZ, 1, 4, 4);

	//1X4 * 4X1  a=1;c=4;b=1
	//coord
	multMatrix(*resTX, *V, x, 1, 1, 4);
	multMatrix(*resTY, *V, y, 1, 1, 4);
	multMatrix(*resTZ, *V, z, 1, 1, 4);

	//devU
	multMatrix(*resUTX, *V, Ux, 1, 1, 4);
	multMatrix(*resUTY, *V, Uy, 1, 1, 4);
	multMatrix(*resUTZ, *V, Uz, 1, 1, 4);

	//devV
	multMatrix(*resVTX, *devV, Vx, 1, 1, 4);
	multMatrix(*resVTY, *devV, Vy, 1, 1, 4);
	multMatrix(*resVTZ, *devV, Vz, 1, 1, 4);

	float vetorV[3] = { Vx[0], Vy[0], Vz[0] };
	float vetorU[3] = { Ux[0], Uy[0], Uz[0] };
	cross(vetorV, vetorU, normal);
	coord[0] = x[0]; coord[1] = y[0]; coord[2] = z[0];
}

//tesselation -> numero de divisoes
void drawBezier(string f1, string f2, int tesselation) {
	int numPatch = 0, numCP = 0, posicao = 0;
	string linha;
	int** indicesCP;
	float** valuesCP;


	ifstream readFile(dir + f1);
	ofstream writeFile(dir + f2);

	if (readFile.is_open()) {

		// primeira linha contém o numero de patches
		getline(readFile, linha);
		numPatch = stoi(linha);

		//as seguintes 'numPatch' linhas contém informação dos control points
		//cada linha terá 16 indices
		//para guardar a info usamos uma matriz -> indicesCP[numPatch][16]
		indicesCP = new int* [numPatch];
		for (int i = 0; i < numPatch; i++) {
			indicesCP[i] = new int[16];
			getline(readFile, linha);
			char* arrayLinha = strdup(linha.c_str());
			char* point = strtok(arrayLinha, ", ");
			for (int j = 0; point != NULL; j++) {
				indicesCP[i][j] = stoi(point);
				point = strtok(NULL, ", ");
			}
		}

		// esta linha contém o numero de control points
		getline(readFile, linha);
		numCP = stoi(linha);

		//as seguintes 'numCP' linhas contém as coordenadas dos control points
		//cada linha terá 3 coordenadas
		//para guardar a info usamos uma matriz -> valuesCP[numCP][3]
		valuesCP = new float* [numCP];
		for (int i = 0; i < numCP; i++) {
			valuesCP[i] = new float[3];
			getline(readFile, linha);
			char* arrayLinha = strdup(linha.c_str());
			char* point = strtok(arrayLinha, ", ");
			for (int j = 0; point != NULL; j++) {
				valuesCP[i][j] = stof(point);
				point = strtok(NULL, ", ");
			}
		}

		//com a informação gravada em memoria, falta agora calcular os pontos através de Bezier
		//e guardar esses em ficheiro

		if (writeFile.is_open()) {

			float incremento = 1.0 / tesselation;
			float coord0[3], coord1[3], coord2[3], coord3[3];
			float normal0[3], normal1[3], normal2[3], normal3[3];

			/*pontos   0 . ---- . 3
						 |      |
						 |      |
					   1 . ---- . 2
			*/

			// v-> horizontal / u-> vertical

			for (int i = 0; i < numPatch; i++) {
				for (float u = 0; u < 1; u += incremento) {
					for (float v = 0; v < 1; v += incremento) {
						float u1 = u + incremento;
						float v1 = v + incremento;

						bezierPatches(u, v, indicesCP[i], valuesCP, coord0, normal0);
						bezierPatches(u, v1, indicesCP[i], valuesCP, coord1, normal1);
						bezierPatches(u1, v1, indicesCP[i], valuesCP, coord2, normal2);
						bezierPatches(u1, v, indicesCP[i], valuesCP, coord3, normal3);




						writeFile << coord0[0] << " " << coord0[1] << " " << coord0[2] << " "
							<< normal0[0] << " " << normal0[1] << " " << normal0[2] << " " << 0 << " " << 0 << endl;
						writeFile << coord1[0] << " " << coord1[1] << " " << coord1[2] << " "
							<< normal1[0] << " " << normal1[1] << " " << normal1[2] << " " << 0 << " " << 0 << endl;
						writeFile << coord2[0] << " " << coord2[1] << " " << coord2[2] << " "
							<< normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << 0 << " " << 0 << endl;

						writeFile << coord0[0] << " " << coord0[1] << " " << coord0[2] << " "
							<< normal0[0] << " " << normal0[1] << " " << normal0[2] << " " << 0 << " " << 0 << endl;
						writeFile << coord2[0] << " " << coord2[1] << " " << coord2[2] << " "
							<< normal2[0] << " " << normal2[1] << " " << normal2[2] << " " << 0 << " " << 0 << endl;
						writeFile << coord3[0] << " " << coord3[1] << " " << coord3[2] << " "
							<< normal3[0] << " " << normal3[1] << " " << normal3[2] << " " << 0 << " " << 0 << endl;


					}
				}
			}
		}
		else {
			cout << "ERROR: Can't write file!" << endl;
		}
	}
	else {
		cout << "ERROR: Can't read file!" << endl;
	}
	readFile.close();
	writeFile.close();
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
		if (argc != 7) {
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
		if (argc != 8) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Vase." << endl;
			drawVase(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]), argv[7]);
		}
	}
	else if (string(argv[1]) == "ring") {
		if (argc != 7) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Ring." << endl;
			drawRing(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), argv[6]);
		}
	}
	else if (string(argv[1]) == "bezier") {
		if (argc != 5) {
			cout << "ERROR" << endl;
		}
		else {
			cout << "OK! Generating Bezier." << endl;
			drawBezier(argv[2], argv[3], atoi(argv[4]));
		}
	}
	else {
		cout << "ERROR! Unknown." << endl;
	}
}


int main(int argc, char** argv) {
	_mkdir(dir.c_str());
	if (argc == 0 || argc > 8) {
		cout << "ERROR! Few arguments." << endl;
	}
	else {
		parseInput(argc, argv);
	}
	cout << "DONE!" << endl;
	return 1;
}