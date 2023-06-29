#include "pch.h"
#include "light.h"

const float PI = 3.14159265359f;

Light::Light(const Vector3 l_position, const float l_intensity, const bool l_move)
{
	position = l_position;
	intensity = l_intensity;
	move = l_move;
}
void Light::Update(float counter) {

	float x = cosf(counter * PI / 180.0f);
	float y = sinf(counter * PI / 180.0f);

	if (move) {
		position.x = radius * x;
		position.y = radius * y;
	}
}