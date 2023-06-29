#include "pch.h"
#include "camera.h"

const float PI = 3.14159265359f;

Camera::Camera(const int width, const int height, const float fov_y,
	const Vector3 view_from, const Vector3 view_at)
{
	width_ = width;
	height_ = height;
	fov_y_ = fov_y;

	view_from_ = view_from; // e
	view_at_ = view_at;

	n = 0.5f; // near plane
	f = 500.0f; // far plane

	buildViewMatrix();
}
void Camera::Update() {
	buildViewMatrix();

	P = computeProjection();
	MVP = buildMVP(M, P);
	MN = buildMN(M);
	
}
Vector3 Camera::getViewFrom() {
	return view_from_;
}

Matrix4x4 Camera::buildMVP(Matrix4x4 M, Matrix4x4 P) {
	return P * V * M; 
}

Matrix4x4 Camera::buildMN(Matrix4x4 M) {
	Matrix4x4 MN = Matrix4x4() * M;

	MN.EuclideanInverse();
	MN.Transpose();

	return MN;
}

void Camera::buildViewMatrix() {
	Vector3 z_e = view_from_ - view_at_;
	z_e.Normalize();
	Vector3 x_e = up_.CrossProduct(z_e);
	x_e.Normalize();
	Vector3 y_e = z_e.CrossProduct(x_e);
	y_e.Normalize();

	V = Matrix4x4(x_e, y_e, z_e, view_from_);
	V.EuclideanInverse();
}

Matrix4x4 Camera::computeProjection() {
	float aspect = (float) width_ / (float)height_;

	float h = 2.0f * n * tanf(fov_y_ / 2.0f);
	float w = h * aspect;

	Matrix4x4 N = Matrix4x4();
	N.set(0, 0, 2.0f / w);
	N.set(1, 1, 2.0f / h);
	N.set(2, 2, 1.0f);
	N.set(3, 3, 1.0f);

	float a = (f + n) / (n - f);
	float b = (2.0f * f * n) / (n - f);

	Matrix4x4 M = Matrix4x4();
	M.set(0, 0, n);
	M.set(1, 1, n);
	M.set(2, 2, a);
	M.set(2, 3, b);
	M.set(3, 2, -1);
	M.set(3, 3, 0);

	return N * M;
}

void Camera::moveCameraX(bool positive) {
	Vector3 view_dir = (view_at_ - view_from_).CrossProduct(up_);
	view_dir.Normalize();
	view_dir *= speed_;

	if (!positive) {
		view_dir *= -1;
	}
	view_from_.x += view_dir.x;
	view_from_.y += view_dir.y;
	view_from_.z += view_dir.z;

	view_at_.x += view_dir.x;
	view_at_.y += view_dir.y;
	view_at_.z += view_dir.z;
}
void Camera::moveCameraY(bool positive) {
	Vector3 view_dir = view_at_ - view_from_;
	view_dir.Normalize();
	view_dir *= speed_;

	if (!positive) {
		view_dir *= -1;
	}
	view_from_.x += view_dir.x;
	view_from_.y += view_dir.y;
	view_from_.z += view_dir.z;

	view_at_.x += view_dir.x;
	view_at_.y += view_dir.y;
	view_at_.z += view_dir.z;
}
void Camera::moveCameraZ(bool positive) {
	Vector3 view_dir = up_;
	view_dir.Normalize();
	view_dir *= speed_;

	if (!positive) {
		view_dir *= -1;
	}
	view_from_.x += view_dir.x;
	view_from_.y += view_dir.y;
	view_from_.z += view_dir.z;

	view_at_.x += view_dir.x;
	view_at_.y += view_dir.y;
	view_at_.z += view_dir.z;
}
void Camera::moveCameraAngle(float yaw_add, float pitch_add) {
	Vector3 view_dir = view_at_ - view_from_;
	view_dir.Normalize();

	float yaw = atan2f(view_dir.y, view_dir.x) - M_PI_2;
	float pitch = acosf(-view_dir.z) - M_PI_2;

	Vector3 new_view_dir = Rz(yaw + yaw_add) * Rx(pitch + pitch_add) * Vector3(0, 1, 0);
	new_view_dir.Normalize();
	view_at_ = view_from_ + new_view_dir;
}

void Camera::Inputs(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveCameraY(true);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveCameraX(false);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveCameraX(true);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveCameraY(false);
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		moveCameraZ(true);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		moveCameraZ(false);
	}

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width_ / 2), (height_ / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;

		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity_ * (float)(mouseX - (width_ / 2)) / width_;
		float rotZ = sensitivity_ * (float)(mouseY - (height_ / 2)) / height_;

		moveCameraAngle(-rotX, -rotZ);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width_ / 2), (height_ / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}
	//view_from_.Print();
}

Matrix3x3 Camera::Rx(float angle) {
	Matrix3x3 matrix = Matrix3x3(1, 0, 0, 0, cosf(angle), -sinf(angle), 0, sinf(angle), cosf(angle));
	return matrix;
}

Matrix3x3 Camera::Ry(float angle) {
	Matrix3x3 matrix = Matrix3x3(cosf(angle), 0, sinf(angle), 0, 1, 0, -sinf(angle), 0, cosf(angle));
	return matrix;
}

Matrix3x3 Camera::Rz(float angle) {
	Matrix3x3 matrix = Matrix3x3(cosf(angle), -sinf(angle), 0, sinf(angle), cosf(angle), 0, 0, 0, 1);
	return matrix;
}

int Camera::getWidth() {
	return width_;
}
int Camera::getHeight() {
	return height_;
}
