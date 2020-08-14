#pragma once
class Matrix;

class Vector2
{
	public:
		Vector2() {x=y=0;}
		Vector2(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		float x,y;
    
		static float Length(const Vector2& vec);
    
		float Length() const;
		void  Normalize();

		void Zero();
    
		void operator+=(const Vector2& vec);	    
		void operator-=(const Vector2& vec);
		bool operator!=(const Vector2& vec);

		Vector2  operator+(const Vector2& vec) const;
		Vector2  operator-(const Vector2& vec) const;

		static Vector2 Transform(const Vector2& vec, const Matrix& mat);

		void operator*=(float f);
		void operator/=(float f);

		Vector2 operator*(float f);
		Vector2 operator/(float f);


		Vector2 operator*(const Vector2& vec);
};
