//clasa camera
#pragma once
#include "MathLibrary.h"
#include "math.h"
#include <cmath>


class Camera{
	public:
		Camera();
		~Camera();

		void init(Vector3D position, Vector3D up, Vector3D right, Vector3D ref);

		void translate_Forward(float dist);
		void translate_Right(float dist);

		void rotateFPS_OY(float angle);
		void rotateTPS(float longitude, float latitude);

		void render();

	public:

		Vector3D forward;
		Vector3D up;
		Vector3D right;
		Vector3D position;
		Vector3D center;
		float time;
};