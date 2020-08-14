#include "Matrix.h"
#include <math.h>
#include "Vector3.h"
#include "Vector2.h"

Matrix Matrix::Identity = Matrix(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);



Matrix Matrix::CreateRotationZ(float radian)
{
	Matrix ret;
	float		fCosine, fSine;

	fCosine	= (float)cos(radian);
	fSine	= (float)sin(radian);

	ret._11 =fCosine;	ret._12 =fSine;		ret._13 =0.0f;		ret._14 =0.0f;
	ret._21 =-fSine;	ret._22 =fCosine;	ret._23 =0.0f;		ret._24 =0.0f;
	ret._31 =0.0f;		ret._32 =0.0f;		ret._33 =1.0f;		ret._34 =0.0f;
	ret._41 =0.0f;		ret._42 =0.0f;		ret._43 =0.0f;		ret._44 =1.0f;

	return ret;
}

Matrix Matrix::CreateRotationY(float radian)
{
	Matrix ret;
	float		fCosine, fSine;

	fCosine	= (float)cos(radian);
	fSine	= (float)sin(radian);

	ret._11 =fCosine;	ret._12 =0.0f;	ret._13 =-fSine;	ret._14 =0.0f;
	ret._21 =0;			ret._22 =1.0f;	ret._23 =0.0f;		ret._24 =0.0f;
	ret._31 =fSine;		ret._32 =0.0f;	ret._33 =fCosine;	ret._34 =0.0f;
	ret._41 =0.0f;		ret._42 =0.0f;	ret._43 =0.0f;		ret._44 =1.0f;

	return ret;
}

Matrix Matrix::CreateRotationX(float radian)
{
	Matrix ret;
	float		fCosine, fSine;

	fCosine	= (float)cos(radian);
	fSine	= (float)sin(radian);

	ret._11 =1.0f;	ret._12 =0.0f;		ret._13 =0.0f;		ret._14 =0.0f;
	ret._21 =0.0f;	ret._22 =fCosine;	ret._23 =fSine;	ret._24 =0.0f;
	ret._31 =0.0f;	ret._32 =-fSine;	ret._33 =fCosine;	ret._34 =0.0f;
	ret._41 =0.0f;	ret._42 =0.0f;		ret._43 =0.0f;		ret._44 =1.0f;

	return ret;
}

Matrix Matrix::CreateTranslation(float x, float y, float z)
{
	Matrix ret = Identity;

	ret._41 = x;
	ret._42 = y;
	ret._43 = z;


	return ret;
}

Matrix Matrix::CreateScale(float s)
{
	Matrix ret = Identity;

	ret._11 = s;
	ret._22 = s;
	ret._33 = s;

	return ret;
}

Vector3 Matrix::Transform(const Vector3& v0) const
{
	Vector3 result;

	result.x = ((v0.x * _11) + (v0.y * _12) + (v0.z * _13))+_41;
	result.y = ((v0.x * _21) + (v0.y * _22) + (v0.z * _23))+_42;
	result.z = ((v0.x * _31) + (v0.y * _32) + (v0.z * _33))+_43;

	return result;
}

Vector2 Matrix::Transform(const Vector2& v0) const
{
	Vector2 result;

	result.x = ((v0.x * _11) + (v0.y * _12) + _41);
	result.y = ((v0.x * _21) + (v0.y * _22) + _42);

	return result;
}


Matrix Matrix::Multiply(const Matrix& m0, const Matrix& m1)
{
	return m0 * m1;
}

void  Matrix::operator*= (const Matrix& rMat)
{
	*this = *this * rMat;
}

Matrix  Matrix::operator* (const Matrix& rMat) const
{
	Matrix result;


	result._11 = _11 * rMat._11 + _12 * rMat._21 + _13 * rMat._31 + _14 * rMat._41;
	result._12 = _11 * rMat._12 + _12 * rMat._22 + _13 * rMat._32 + _14 * rMat._42;
	result._13 = _11 * rMat._13 + _12 * rMat._23 + _13 * rMat._33 + _14 * rMat._43;
	result._14 = _11 * rMat._14 + _12 * rMat._24 + _13 * rMat._34 + _14 * rMat._44;

	result._21 = _21 * rMat._11 + _22 * rMat._21 + _23 * rMat._31 + _24 * rMat._41;
	result._22 = _21 * rMat._12 + _22 * rMat._22 + _23 * rMat._32 + _24 * rMat._42;
	result._23 = _21 * rMat._13 + _22 * rMat._23 + _23 * rMat._33 + _24 * rMat._43;
	result._24 = _21 * rMat._14 + _22 * rMat._24 + _23 * rMat._34 + _24 * rMat._44;

	result._31 = _31 * rMat._11 + _32 * rMat._21 + _33 * rMat._31 + _34 * rMat._41;
	result._32 = _31 * rMat._12 + _32 * rMat._22 + _33 * rMat._32 + _34 * rMat._42;
	result._33 = _31 * rMat._13 + _32 * rMat._23 + _33 * rMat._33 + _34 * rMat._43;
	result._34 = _31 * rMat._14 + _32 * rMat._24 + _33 * rMat._34 + _34 * rMat._44;

	result._41 = _41 * rMat._11 + _42 * rMat._21 + _43 * rMat._31 + _44 * rMat._41;
	result._42 = _41 * rMat._12 + _42 * rMat._22 + _43 * rMat._32 + _44 * rMat._42;
	result._43 = _41 * rMat._13 + _42 * rMat._23 + _43 * rMat._33 + _44 * rMat._43;
	result._44 = _41 * rMat._14 + _42 * rMat._24 + _43 * rMat._34 + _44 * rMat._44;

	return result;
}


Matrix Matrix::CreateLookAt(const Vector3& Eye, const Vector3& At, const Vector3& Up)
{
	Matrix m;

	Vector3 zaxis = Vector3::Normal(Eye - At);
	Vector3 xaxis = Vector3::Normal(Vector3::Cross(Up, zaxis));
	Vector3	yaxis = Vector3::Cross(zaxis, xaxis);

	m._11 = xaxis.x;					m._12 = yaxis.x;					m._13 = zaxis.x;					m._14 = 0;
	m._21 = xaxis.y;					m._22 = yaxis.y;					m._23 = zaxis.y;					m._24 = 0;
	m._31 = xaxis.z;					m._32 = yaxis.z;					m._33 = zaxis.z;					m._34 = 0;
	m._41 = -Vector3::Dot(xaxis, Eye);  m._42 = -Vector3::Dot(yaxis, Eye);  m._43 = -Vector3::Dot(zaxis, Eye);	m._44 = 1.0f;

	return m;
}

Matrix Matrix::CreatePerspectiveFieldOfView(float fov, float aspectratio,float nearPlane, float farPlane)
{
	Matrix m;

	float f = 1.0f / tan( fov / 2.0f);

	m._11 = f/aspectratio;		m._12 = 0.0f;		m._13 = 0.0f;										m._14 =  0.0f;
	m._21 = 0.0f;				m._22 = f;			m._23 = 0.0f;										m._24 =  0.0f;
	m._31 = 0.0f;				m._32 = 0.0f;		m._33 = farPlane/(nearPlane-farPlane);				m._34 = -1.0f;
	m._41 = 0.0f;				m._42 = 0.0f;		m._43 = (nearPlane*farPlane)/(nearPlane-farPlane);	m._44 =  0.0f;

	return m;
}

Matrix Matrix::CreateOrthoOffCenter(float left, float right, float bottom, float top,float nearPlane, float farPlane)
{
	Matrix m;

	m._11 = 2 / (right-left);
	m._12 = 0;
	m._13 = 0;
	m._14 = 0;

	m._21 = 0;
	m._22 = 2 / (top-bottom);
	m._23 = 0;
	m._24 = 0;

	m._31 = 0;
	m._32 = 0;
	m._33 = 1 / (farPlane - nearPlane);
	m._34 = nearPlane / (farPlane - nearPlane);

	m._41 = -1;
	m._42 = 1;
	m._43 = 0;
	m._44 = 1;

	return m;
}


void Matrix::SetTranslation(const Vector3& r)
{
	_41 = r.x;
	_42 = r.y;
	_43 = r.z;
}

Vector3 Matrix::GetTranslation() const
{
	return Vector3(_41,_42,_43);
}
