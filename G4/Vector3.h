#pragma once

class Matrix;
class Vector2;

class Vector3 
{
public:
	Vector3();
	Vector3(const Vector3& ref);
	Vector3(float x, float y, float z);
	
	static Vector3 Transform(const Vector3& vec, const Matrix& mat);
	
	static float     Dot  (const Vector3& v0, const Vector3& v1);
	static Vector3 Cross(const Vector3& v0, const Vector3& v1);
	static Vector3 Normal(const Vector3& v0);
	
	void Normalize();
	
	static Vector3 Up;
	
	float Length();
	
	Vector3& operator=(const Vector3& vec);
	
	Vector3 operator-(const Vector3& vec) const;
	Vector3 operator+(const Vector3& vec) const;
	
	void operator-=(const Vector3& vec);
	void operator+=(const Vector3& vec);
	
	void operator*=(float f);
	void operator/=(float f);
	
	Vector3 operator*(float f);
	Vector3 operator/(float f);

	Vector2 Vec2() const;
	
	float x,y,z;
};