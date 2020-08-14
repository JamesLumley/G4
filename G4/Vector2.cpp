#include "Vector2.h"
#include "Matrix.h"
#include <math.h>

Vector2 Vector2::Transform(const Vector2& vec, const Matrix& mat)
{
	return mat.Transform(vec);
}

void  Vector2::Normalize()
{
	float length = Length();
	
	x/= length;
	y/= length;
}

void Vector2::Zero()
{
	x = 0.0f;
	y = 0.0f;
}


float  Vector2::Length() const
{
	return sqrt((x*x) + (y*y));
}


void Vector2::operator+=(const Vector2& vec)
{
    this->x += vec.x;
    this->y += vec.y;
}

bool Vector2::operator!=(const Vector2& vec)
{
	Vector2 r = *this - vec;

	return r.Length() > 1;
}

Vector2  Vector2::operator+(const Vector2& vec) const
{
	Vector2 result = *this;
	
	result.x += vec.x;
	result.y += vec.y;
	
	return result;
}

Vector2  Vector2::operator-(const Vector2& vec) const
{
	Vector2 result = *this;
	
	result.x -= vec.x;
	result.y -= vec.y;
	
	return result;
}

void Vector2::operator-=(const Vector2& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
}

void Vector2::operator*=(float f)
{
	this->x *= f;
	this->y *= f;
}

void Vector2::operator/=(float f)
{
	this->x /= f;
	this->y /= f;
}


Vector2 Vector2::operator*(float f)
{
	Vector2 ret = *this;

	ret *= f;

	return ret;
}

Vector2 Vector2::operator/(float f)
{
	Vector2 ret = *this;

	ret /= f;

	return ret;
}

Vector2 Vector2::operator*(const Vector2& vec)
{
	Vector2 ret;

	ret.x = this->x * vec.x;
	ret.y = this->y * vec.y;

	return ret;
}


float Vector2::Length(const Vector2& vec)
{
	return vec.Length();
}