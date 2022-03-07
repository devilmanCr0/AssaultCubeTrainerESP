#pragma once
#include <algorithm>


class Vector3L
{
public:
	float x,y,z;

	Vector3L() {};
	Vector3L(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3L operator + (const Vector3L& rhs) const { return Vector3L(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3L operator - (const Vector3L& rhs) const { return Vector3L(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3L operator * (const float& rhs) const { return Vector3L(x * rhs, y * rhs, z * rhs); }
	Vector3L operator / (const float& rhs) const { return Vector3L(x / rhs, y / rhs, z / rhs); }
	Vector3L& operator += (const Vector3L& rhs) { return *this = *this + rhs; };
	Vector3L& operator -= (const Vector3L& rhs) { return *this = *this - rhs; };
	Vector3L& operator *= (const float& rhs) { return *this = *this * rhs; };
	Vector3L& operator /= (const float& rhs) { return *this = *this / rhs; };

	float length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3L Normalize() const { return *this * (1 / length()); }
	float Distance(const Vector3L& rhs) const { return (*this - rhs).length(); }
};

struct Vec4
{
	float x, y, z, w;
};

bool WorldToScreen(Vector3L pos, Vector3L& screen, float matrix[16], int windWidth, int windowHeight);