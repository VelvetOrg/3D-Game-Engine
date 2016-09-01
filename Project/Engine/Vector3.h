#pragma once
class Vec3
{
public:
	float x;
	float y;
	float z;

	Vec3() : x(0), y(0), z(0) {}
	Vec3(float nx, float ny, float nz) : x(nx), y(ny), z(nz) {}
};