#ifndef CAMERA_H_
#define CAMERA_H_

#include "pch.h"
#include "tutorials.h"
#include "utils.h"
#include "glutils.h"
#include "matrix4x4.h"

class Camera{

public:
	Camera() { }

	Camera(const int width, const int height, const float fov_y,
		const Vector3 view_from, const Vector3 view_at);

	void Update();

	void buildViewMatrix();
	Matrix4x4 computeProjection();
	Matrix4x4 buildMVP(Matrix4x4 M, Matrix4x4 P);
	Matrix4x4 buildMN(Matrix4x4 M);
	Vector3 getViewFrom();

	void Inputs(GLFWwindow* window);
	void moveCameraAngle(float yaw, float pitch);
	void moveCameraX(bool positive);
	void moveCameraY(bool positive);
	void moveCameraZ(bool positive);

	Matrix3x3 Rx(float angle);
	Matrix3x3 Ry(float angle);
	Matrix3x3 Rz(float angle);

	int getWidth();
	int getHeight();

	Matrix4x4 M;
	Matrix4x4 V;
	Matrix4x4 P;
	Matrix4x4 MVP;
	Matrix4x4 MN;

private:
	int width_{ 640 }; // image width (px)
	int height_{ 480 };  // image height (px)
	//float fov_y_{ 0.785f }; // vertical field of view (rad)
	float fov_y_{ deg2rad(170) }; // vertical field of view (rad)

	float n; // near plane
	float f; // far plane

	Vector3 view_from_; // ray origin or eye or O
	Vector3 view_at_; // target T
	Vector3 up_{ Vector3(0.0f, 0.0f, 1.0f) }; // up vector

	float speed_{ 0.1f }; // 0.5
	float sensitivity_{ 1.0f };

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;
};

#endif
