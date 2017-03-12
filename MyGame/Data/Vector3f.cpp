#include "Vector3f.h"

#define _USE_MATH_DEFINES
#include <math.h>

CVector3f::CVector3f()
{
	x = 0;
	y = 0;
	z = 0;
}

CVector3f::CVector3f(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

CVector3f::CVector3f(int x_, int y_, int z_)
{
	x = (float) x_;
	y = (float) y_;
	z = (float) z_;
}

CVector3f::CVector3f(double x_, double y_, double z_) 
{
	x = (float) x_;
	y = (float) y_;
	z = (float) z_;
}

CVector3f::CVector3f(const CVector3f &that) 
{
	*this = that;
}

CVector3f::~CVector3f() 
{
}

CVector3f CVector3f::operator+(const CVector3f &that) const 
{
	return CVector3f(x + that.x, y + that.y, z + that.z);
}

CVector3f CVector3f::operator-(const CVector3f &that) const 
{
	return CVector3f(x - that.x, y - that.y, z - that.z);
}

CVector3f &CVector3f::operator+=(const CVector3f &that) 
{
	*this = *this + that;
	return *this;
}

CVector3f &CVector3f::operator-=(const CVector3f &that) 
{
	*this = *this - that;
	return *this;
}

CVector3f CVector3f::operator*(const float scalar) const 
{
	return CVector3f(x * scalar, y * scalar, z * scalar);
}

CVector3f CVector3f::operator/(const float scalar) const 
{
	return CVector3f(x / scalar, y / scalar, z / scalar);
}

CVector3f &CVector3f::operator*=(const float scalar) 
{
	*this = *this * scalar;
	return *this;
}

CVector3f &CVector3f::operator/=(const float scalar) 
{
	*this = *this / scalar;
	return *this;
}

bool CVector3f::operator==(const CVector3f &that) const 
{
	return x == that.x && y == that.y && z == that.z;
}

bool CVector3f::operator!=(const CVector3f &that) const 
{
	return x != that.x || y != that.y || z != that.z;
}

float CVector3f::operator*(const CVector3f &that) const 
{
	return x * that.x + y * that.y + z * that.z;
}

CVector3f CVector3f::operator%(const CVector3f &that) const 
{
	return CVector3f(y * that.z - z * that.y, z * that.x - x * that.z, x * that.y - y * that.x);
}

CVector3f &CVector3f::operator%=(const CVector3f &that) 
{
	*this = *this % that;
	return *this;
}

float CVector3f::Length() 
{
	return sqrtf(*this * *this);
}

void CVector3f::Normalise() 
{
	if(x == 0 && y == 0 && z == 0)
		return;
	*this = *this / Length();
}


void CVector3f::ClipX(float xmin, float xmax) 
{
	if (x < xmin)
		x = xmin;
	if (x > xmax)
		x = xmax;
}

void CVector3f::ClipY(float ymin, float ymax) 
{
	if (y < ymin)
		y = ymin;
	if (y > ymax)
		y = ymax;
}

void CVector3f::ClipZ(float zmin, float zmax) 
{
	if (z < zmin)
		z = zmin;
	if (z > zmax)
		z = zmax;
}
 

void CVector3f::Truncate(const float max)
{
	if(x > max) x = max;
	if(y > max) y = max;
	if(z > max) z = max;
}

CVector3f CVector3f::RotateAboutAxis(float angle, CVector3f axis)
{
	// Convert angle to radians
	angle = angle * (float) M_PI / 180.0f;

	float cos_theta = (float) cos(angle);
	float sin_theta = (float) sin(angle);

	CVector3f result(0, 0, 0);

	result.x = (cos_theta + (1 - cos_theta) * axis.x * axis.x) * x;
	result.x += ((1 - cos_theta) * axis.x * axis.y - axis.z * sin_theta) * y;
	result.x += ((1 - cos_theta) * axis.x * axis.z + axis.y * sin_theta) * z;

	result.y = ((1 - cos_theta) * axis.x * axis.y + axis.z * sin_theta) * x;
	result.y += (cos_theta + (1 - cos_theta) * axis.y * axis.y) * y;
	result.y += ((1 - cos_theta) * axis.y * axis.z - axis.x * sin_theta) * z;

	result.z = ((1 - cos_theta) * axis.x * axis.z - axis.y * sin_theta) * x;
	result.z += ((1 - cos_theta) * axis.y * axis.z + axis.x * sin_theta) * y;
	result.z += (cos_theta + (1 - cos_theta) * axis.z * axis.z) * z;

	return result;
}

float CVector3f::XZDistance(CVector3f &other)
{
	return sqrt(pow(other.x - x, 2) + pow(other.z - z, 2));
}

