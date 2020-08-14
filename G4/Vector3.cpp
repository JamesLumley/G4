#include "Vector3.h"
#include "Matrix.h"
#include "Vector2.h"
#include <math.h>

Vector3 Vector3::Up = Vector3(0,1,0);

Vector3::Vector3()
{
	x = y = z = 0.0f;
}

Vector3::Vector3(const Vector3& ref)
{
	*this = ref;
}

Vector3 Vector3::Transform(const Vector3& vec, const Matrix& mat)
{
	return mat.Transform(vec);
}

float Vector3::Dot(const Vector3& v0, const Vector3& v1)
{
	return (v0.x * v1.x) + (v0.y * v1.y) + (v0.z * v1.z);
}
Vector3  Vector3::Cross(const Vector3& v0, const Vector3& v1)
{
	Vector3 result;
	
	result.x = v0.y * v1.z - v0.z * v1.y;
	result.y = v0.z * v1.x - v0.x * v1.z;
	result.z = v0.x * v1.y - v0.y * v1.x;
	
	return result;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}



void  Vector3::Normalize()
{
	float length = Length();
	
	x/= length;
	y/= length;
	z/= length;
}


float  Vector3::Length()
{
	return sqrt((x*x) + (y*y) + (z*z));
}

Vector3&  Vector3::operator=(const Vector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	
	return *this;
}

Vector3  Vector3::operator-(const Vector3& vec) const
{
	Vector3 result = *this;
	
	result.x -= vec.x;
	result.y -= vec.y;
	result.z -= vec.z;
	
	return result;
}

Vector3 Vector3::operator+(const Vector3& vec) const
{
	Vector3 result = *this;
	
	result.x += vec.x;
	result.y += vec.y;
	result.z += vec.z;
	
	return result;
}

void Vector3::operator-=(const Vector3& vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;
}

void Vector3::operator+=(const Vector3& vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;
	
}

void Vector3::operator*=(float f)
{
	this->x *= f;
	this->y *= f;
	this->z *= f;
}

void Vector3::operator/=(float f)
{
	this->x /= f;
	this->y /= f;
	this->z /= f;
	
}


Vector3 Vector3::operator*(float f)
{
	Vector3 ret = *this;
	
	ret *= f;
	
	return ret;
}

Vector3 Vector3::operator/(float f)
{
	Vector3 ret = *this;
	
	ret /= f;
	
	return ret;
}



Vector3 Vector3::Normal(const Vector3& v0)
{
	Vector3 ret = v0;
	
	ret.Normalize();
	
	return ret;
}

Vector2 Vector3::Vec2() const
{
	Vector2 ret;
	ret.x = this->x;
	ret.y = this->y;

	return ret;
}