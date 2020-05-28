#pragma once
#ifndef __MY__CAMERA__
#define __MY__CAMERA__
#define _USE_MATH_DEFINES
#include <math.h>
class Camera {
public:
	float rad;
	float sensitivity;
	float* pos;
	float* la;
	float* up;

	Camera() {
		this->rad = 10.0f;
		this->pos = new float[3];
		this->la = new float[3];
		this->up = new float[3];
		pos[0] = pos[2] = 0.0f; pos[1] = 10.0f;
		la[0] = la[0] = la[0] = 0.0f;
		up[0] = up[2] = 0.0f; up[1] = 1.0f;
		this->sensitivity = 0.001f;
	}

	void updateCartesian(float cam_beta, float cam_alpha) {
		this->pos[0] = rad * cos(cam_beta) * sin(cam_alpha);
		this->pos[1] = rad * sin(cam_beta);
		this->pos[2] = rad * cos(cam_beta) * cos(cam_alpha);
	}
};
#endif