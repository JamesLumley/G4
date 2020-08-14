#pragma once

class Vector3;
class Vector2;

class Matrix
{
public:
	Matrix(){}
	Matrix(float a00, float a01, float a02, float a03
			,float a10, float a11, float a12, float a13
			,float a20, float a21, float a22, float a23
			,float a30, float a31, float a32, float a33
			)
	{
		_11 = a00; _12 = a01; _13 = a02; _14 = a03;
		_21 = a10; _22 = a11; _23 = a12; _24 = a13;
		_31 = a20; _32 = a21; _33 = a22; _34 = a23;
		_41 = a30; _42 = a31; _43 = a32; _44 = a33;
		
	}
	
	static Matrix Identity;
		
	static Matrix Multiply(const Matrix& m0, const Matrix& m1);
	static Matrix CreateRotationZ(float radian);
	static Matrix CreateRotationY(float radian);
	static Matrix CreateRotationX(float radian);
	
	static Matrix CreateTranslation(float x, float y, float z);
	
	static Matrix CreateScale(float s);
	
	static Matrix CreateLookAt(const Vector3& pos, const Vector3& target, const Vector3& Up);
	static Matrix CreatePerspectiveFieldOfView(float fov, float aspectratio,float nearPlane, float farPlane);
	static Matrix CreateOrthoOffCenter(float left, float right, float bottom, float top,float nearPlane, float farPlane);
	
	Vector3 Transform(const Vector3& v0) const;
	Vector2 Transform(const Vector2& v0) const;
	
	Matrix  operator*  (const Matrix& rMat) const;
	void    operator*= (const Matrix& rMat);
	
	void SetTranslation(const Vector3& r);
	Vector3 GetTranslation() const;
	
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};
