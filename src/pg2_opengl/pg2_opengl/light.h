#ifndef LIGHT_H_
#define LIGHT_H_

#include "pch.h"
#include "tutorials.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"

class Light
{
public:
	Light() { }

	Light(const Vector3 l_position, const float l_intensity, const bool l_move = true);

	void Update(float counter);

	Vector3 position;
	float intensity;

private:
	float radius{ 100.0f };
	bool move { true };
};

#endif