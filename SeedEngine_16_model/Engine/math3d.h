
#pragma once

#include<cmath>
#include<cstdio>
#include<cstring>


namespace Seed
{

	const float eps = 1e-8;
	const float pi = acos(-1.0);

	template<class T> inline T Max(T a, T b) { return a > b ? a : b; }
	template<class T> inline T Min(T a, T b) { return a < b ? a : b; }




	inline int dcmp(float x) {
		if (fabs(x) < eps) return 0;
		return x < 0 ? -1 : 1;
	}

	inline float Deg2Rad(float ang) {
		return ang*pi / 180.0f;
	}

	inline float Rad2Deg(float rad) {
		return rad*180.0f / pi;
	}




	template<class T, unsigned n>
	class Vector
	{
	public:
		Vector() {}
		Vector(const Vector<T, n> &v) {
			for (unsigned i = 0; i < n; ++i)
				(*this)[i] = v[i];
		}
		Vector& operator=(const Vector<T, n> &v) {
			for (unsigned i = 0; i < n; ++i)
				(*this)[i] = v[i];
			return *this;
		}

		inline bool operator == (const Vector<T, n> &v) const {
			for (unsigned i = 0; i < n; ++i)
			if ((*this)[i] != v[i])  return false;
			return true;
		}
		inline bool operator != (const Vector<T, n> &v) const { return !operator==(v); }
		// vec+-vec
		Vector<T, n> operator+(const Vector<T, n> &v) const {
			Vector<T, n> res;
			for (unsigned i = 0; i < n; ++i)
				res[i] = (*this)[i] + v[i];
			return res;
		}
		Vector<T, n> operator-(const Vector<T, n> &v) const {
			Vector<T, n> res;
			for (unsigned i = 0; i < n; ++i)
				res[i] = (*this)[i] - v[i];
			return res;
		}

		// vec*/c
		Vector<T, n> operator*(const T& v) const {
			Vector<T, n> res;
			for (unsigned i = 0; i < n; ++i)
				res[i] = (*this)[i] * v;
			return res;
		}
		Vector<T, n> operator/(const T& v) const {
			Vector<T, n> res;
			for (unsigned i = 0; i < n; ++i)
				res[i] = (*this)[i] / v;
			return res;
		}
		// vec*/=c
		inline Vector<T, n>& operator*=(const T& v)  {
			for (unsigned i = 0; i < n; ++i)
				(*this)[i] *= v;
			return *this;
		}
		Vector<T, n>& operator/=(const T& v)  {
			for (unsigned i = 0; i < n; ++i)
				(*this)[i] /= v;
			return *this;
		}
		// -vec
		friend Vector<T, n> operator-(Vector<T, n> rhs)  {
			Vector<T, n> res;
			for (unsigned i = 0; i < n; ++i)
				res[i] = -rhs[i];
			return res;
		}
		

		inline T& operator[](unsigned int i) { return vec[i]; }
		inline T operator[] (unsigned int i) const { return vec[i]; }


		// calc
		inline T dot(const Vector<T, n> &v) const {
			T res = T(0);
			for (unsigned i = 0; i < n; ++i)
				res += (*this)[i] * v[i];
			return res;
		}
		inline T abs() const { return sqrt(this->dot(*this)); }
		inline T absSqr() const { return this->dot(*this); }
		inline Vector<T, n> normal() const { return (*this) / abs(); }
		inline Vector<T, n> reflect(const Vector<T, n> &Normal) const {
			return *this - Normal * 2 * this->dot(Normal);
		}
			

	protected:
		T vec[n];
	};


	// n*n方阵
	template<class T, unsigned n> 
	class Matrix
	{
	public:
		Matrix() {}
		Matrix(const Matrix<T, n> &a) { memcpy(m, a.m, sizeof(m)); }
		inline Matrix<T, n>& operator=(const Matrix<T, n>&a) {
			memcpy(m, a.m, sizeof(m));
			return *this;
		}

		inline T* operator[] (unsigned i) { return m[i]; }
		inline const T* operator[] (unsigned i) const { return m[i]; }

		// mat*/c
		inline Matrix<T, n> operator*(const T &c) const {
			Matrix<T, n> res;
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				res[i][j] = m[i][j] * c;
			return res;
		}
		inline Matrix<T, n> operator/(const T &c) const {
			Matrix<T, n> res;
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				res[i][j] = m[i][j] / c;
			return res;
		}

		// mat*/=c
		inline Matrix<T, n>& operator *= (const T &c)  {
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				m[i][j] *= c;
			return *this;
		}
		inline Matrix<T, n>& operator /= (const T &c)  {
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				m[i][j] /= c;
			return *this;
		}

		// mat+-*mat
		inline Matrix<T, n> operator+(const Matrix<T, n>&mat) const {
			Matrix<T, n> res;
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				res[i][j] = m[i][j] + mat[i][j];
			return res;
		}
		inline Matrix<T, n> operator-(const Matrix<T, n>&mat) const {
			Matrix<T, n> res;
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				res[i][j] = m[i][j] - mat[i][j];
			return res;
		}
		inline Matrix<T, n> operator*(const Matrix<T, n>&mat) const {
			unsigned i, j, k;
			Matrix<T, n> res;
			for (i = 0; i < n; ++i)  {
				for (j = 0; j < n; ++j)  {
					res[i][j] = T(0);
					for (k = 0; k < n; ++k)
						res[i][j] += m[i][k] * mat[k][j];
				}
			}
			return res;
		}
		// mat +-*= mat
		inline Matrix<T, n>& operator+=(const Matrix<T, n>&mat)  {
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				m[i][j] += mat[i][j];
			return *this;
		}
		inline Matrix<T, n>& operator-=(const Matrix<T, n>&mat)  {
			for (unsigned i = 0; i < n; ++i) for (unsigned j = 0; j < n; ++j)
				m[i][j] -= mat[i][j];
			return *this;
		}
		inline Matrix<T, n>& operator*=(const Matrix<T, n>&mat)  {
			unsigned i, j, k;
			Matrix<T, n> res;
			for (i = 0; i < n; ++i)  {
				for (j = 0; j < n; ++j)  {
					res[i][j] = T(0);
					for (k = 0; k < n; ++k)
						res[i][j] += m[i][k] * mat[k][j];
				}
			}
			return *this=res;
		}

		// identiy transpose inverse transform transformNormal
		inline Matrix<T, n>& initIdentity() {
			for (unsigned i = 0; i < n; ++i)
			for (unsigned j = 0; j < n; ++j)
				m[i][j] = T(i == j);
			return *this;
		}

		inline Matrix<T, n> transpose() const {
			Matrix<T, n> res;
			for (unsigned i = 0; i < n; ++i)
			for (unsigned j = 0; j < n; ++j)
				res[i][j] = m[j][i];
			return res;
		}

		inline Matrix<T, n> inverse() const
		{
			// augment matrix [a,b]
			Matrix<T, n> a(*this), b;
			b = b.initIdentity();

			int i, j, k;
			for (i = 0; i < n; ++i)
			{
				float mx = a[i][i];
				k = i;
				for (j = i + 1; j < n; ++j)  {
					if (mx < a[j][i]) {
						mx = a[j][i];
						k = j;
					}
				}

				// max pivot
				if (k - i)  {
					for (j = 0; j < n; ++j)
					{
						float tmp = a[i][j];
						a[i][j] = a[k][j];
						a[k][j] = tmp;

						tmp = b[i][j];
						b[i][j] = b[k][j];
						b[k][j] = tmp;
					}
				}

				for (j = i + 1; j < n; ++j) {
					float f = a[j][i] / a[i][i];
					for (k = 0; k < n; ++k)  {
						a[j][k] -= f*a[i][k];
						b[j][k] -= f*b[i][k];
					}
				}
			}

			// back substitute
			for (i = n - 1; i >= 0; --i) {
				float f = a[i][i];
				for (j = 0; j < n; ++j) {
					a[i][j] /= f;
					b[i][j] /= f;
				}

				for (j = i - 1; j >= 0; --j) {
					f = a[j][i];
					for (k = 0; k < n; ++k) {
						a[j][k] -= f*a[i][k];
						b[j][k] -= f*b[i][k];
					}
				}
			}
			return b;
		}

		inline Vector<T, n - 1> transform(const Vector<T, n - 1> &v) {
			// [vec, 1] * mat
			Vector<T, n - 1> res;
			T w = T(0);
			for (unsigned i = 0; i < n - 1; ++i) {
				res[i] = m[3][i];
				for (unsigned j = 0; j < n - 1; ++j)
					res[i] += v[j] * m[j][i];
				w += v[i] * m[i][3];
			} 
			w += m[3][3];
			return res / w;
		}

		inline Vector<T, n - 1> transformNormal(const Vector<T, n - 1> &v) {
			// [vec, 0] * mat
			Vector<T, n - 1> res;
			for (unsigned i = 0; i < n - 1; ++i) {
				res[i] = T(0);
				for (unsigned j = 0; j < n - 1; ++j)
					res[i] += v[j] * m[j][i];
			}
			return res;
		}

	protected:
		T m[n][n];
	};




	template<class T> 
	class vector2 : public Vector<T, 2>
	{
	public:
		vector2() {}
		vector2(T x, T y) { set(x, y); }
		vector2(const Vector<T, 2> & v) { set(v[0], v[1]); }
		inline vector2& operator =(const Vector<T, 2>& v) {
			set(v[0], v[1]);
			return *this;
		}

		inline void setX(T x) { vec[0] = x; }
		inline void setY(T y) { vec[1] = y; }
		inline void set(const T &x, const T &y) { setX(x), setY(y); }
		inline T getX() const { return (*this)[0]; }
		inline T getY() const { return (*this)[1]; }

		// calc
		inline T cross(const Vector<T, 2>& v) { return T(getX()*v[1] - getY()*v[0]); }


	};


	class Quaternion;
	// class vector3<T>
	template<class T>
	class vector3 : public Vector<T, 3>
	{
	public:
		vector3(){}
		vector3(T x, T y, T z){ vec[0] = x; vec[1] = y; vec[2] = z; }

		template<unsigned N>
		vector3(const Vector<T, N>& v) {
			unsigned n = Min<unsigned>(3, N);
			for (unsigned i = 0; i < n; ++i)
				vec[i] = v[i];
			if(N<3) {
				for (unsigned i = N; i < 3; ++i)
					vec[i] = 0;
			}
		}

		template<unsigned N>
		inline vector3& operator=(const Vector<T, N>& v) {
			unsigned n = Min<unsigned>(3, N);
			for (unsigned i = 0; i < n; ++i)
				vec[i] = v[i];
			if (N < 3) {
				for (unsigned i = N; i < 3; ++i)
					vec[i] = 0;
			}
			return *this;
		}

		inline void set(T x, T y, T z) {
			vec[0] = x; vec[1] = y; vec[2] = z;
		}
		inline void setX(T x) { vec[0] = x; }
		inline void setY(T y) { vec[1] = y; }
		inline void setZ(T z) { vec[2] = z; }
		inline T getX() const { return vec[0]; }
		inline T getY() const { return vec[1]; }
		inline T getZ() const { return vec[2]; }

		//calc
		inline vector3<T> cross(const Vector<T, 3>& v) const {
			vector3<T> res;
			res[0] = (*this)[1] * v[2] - (*this)[2] * v[1];
			res[1] = -((*this)[0] * v[2] - (*this)[2] * v[0]);
			res[2] = (*this)[0] * v[1] - (*this)[1] * v[0];
			return res;
		}
	};

	// 全特化 vector3f  不是模板类，函数定义可在cpp
	class vector3f : public vector3<float>
	{
	public:
		vector3f(){}
		vector3f(float x, float y, float z){ vec[0] = x; vec[1] = y; vec[2] = z; }

		template<class T, unsigned N>
		vector3f(const Vector<T, N>& v) {
			unsigned n = Min<unsigned>(N, 3);
			for (unsigned i = 0; i < n; ++i)
				vec[i] = v[i];
			if (N < 3) {
				for (unsigned i = N; i < 3; ++i)
					vec[i] = 0;
			}
		}

		template<class T, unsigned N>
		inline vector3f& operator=(const Vector<T, N>& v) {
			unsigned n = Min<unsigned>(N, 3);
			for (unsigned i = 0; i < n; ++i)
				vec[i] = v[i];
			if (N < 3) {
				for (unsigned i = N; i < 3; ++i)
					vec[i] = 0;
			}
			return *this;
		}

		vector3f rotate(const Quaternion &q) const;  // clockwise

	};


	template<class T> 
	class vector4 : public Vector<T, 4>
	{
	public:
		vector4(){}
		vector4(T x, T y, T z, T w){
			set(x, y, z, w);
		}
		vector4(const Vector<T, 4>& v) {
			for (unsigned i = 0; i < 4; ++i)
				vec[i] = v[i];
		}
		inline vector4& operator=(const Vector<T, 4>& v) {
			for (unsigned i = 0; i < 4; ++i)
				vec[i] = v[i];
			return *this;
		}

		inline void set(T x, T y, T z, T w) { vec[0] = x; vec[1] = y; vec[2] = z; vec[3] = w; }
		inline void setX(T x) { vec[0] = x; }
		inline void setY(T y) { vec[1] = y; }
		inline void setZ(T z) { vec[2] = z; }
		inline void setW(T w) { vec[3] = w; }
		inline T getX() const { return vec[0]; }
		inline T getY() const { return vec[1]; }
		inline T getZ() const { return vec[2]; }
		inline T getW() const { return vec[3]; }

	};


	// 全特化 vector4f
	class vector4f : public vector4<float>
	{
	public:
		vector4f(){}
		vector4f(float x, float y, float z, float w){
			set(x, y, z, w);
		}

		template<class T>
		vector4f(const Vector<T, 4>& v) {
			for (unsigned i = 0; i < 4; ++i)
				vec[i] = v[i];
		}
		template<class T>
		inline vector4f& operator=(const Vector<T, 4>& v) {
			for (unsigned i = 0; i < 4; ++i)
				vec[i] = v[i];
			return *this;
		}
	};


	// 四元数在lhs和在rhs下方向不同， 用旋转轴+角度初始化自动取反
	class Quaternion : public vector4f
	{
	public:
		Quaternion(){}
		Quaternion(float x, float y, float z, float w){
			set(x, y, z, w);
		}
		Quaternion(const Vector<float, 4>& q) {
			for (unsigned i = 0; i < 4; ++i)
				vec[i] = q[i];
		}
		inline Quaternion& operator=(const Vector<float, 4>& v) {
			for (unsigned i = 0; i < 4; ++i)
				vec[i] = v[i];
			return *this;
		}
		Quaternion(const vector3f & axis, float rad) {
			getRotate(axis, rad);
		}

		inline Quaternion conjugate() const {
			Quaternion res;
			res.setX(-getX());
			res.setY(-getY());
			res.setZ(-getZ());
			res.setW(getW());
			return res;
		}

		inline Quaternion& getRotate(const vector3f &axis, float rad)  {
			rad = -rad;
			//assert axis is normalized
			float a = cos(rad * 0.5);
			float b = sin(rad * 0.5);
			setW(a);
			setX(b*axis.getX());
			setY(b*axis.getY());
			setZ(b*axis.getZ());
			return *this;
		}

		inline Quaternion operator  * (const Quaternion &q) const {
			Quaternion res;
			res.setW(getW()*q.getW() - getX()*q.getX() - getY()*q.getY() - getZ()*q.getZ());
			res.setX(getW()*q.getX() + q.getW()*getX() + getY()*q.getZ() - getZ()*q.getY());
			res.setY(getW()*q.getY() + q.getW()*getY() + q.getX()*getZ() - getX()*q.getZ());
			res.setZ(getW()*q.getZ() + q.getW()*getZ() + getX()*q.getY() - getY()*q.getX());
			return res;
		}

		inline Quaternion& operator *= (const Quaternion &q) { return *this = (*this)*q; }
			

		// getDirection
		// localdir.rotate(quaternion);   局部方向旋转
		//m[0][0] = 1 - 2 * (y*y + z*z); m[0][1] = 2 * x*y - 2 * w*z; m[0][2] = 2 * (w*y + x*z); m[0][3] = 0;
		//m[1][0] = 2 * x*y + 2 * w*z; m[1][1] = 1 - 2 * (x*x + z*z);	m[1][2] = -2 * w*x + 2 * y*z; m[1][3] = 0;
		//m[2][0] = -2 * w*y + 2 * x*z; m[2][1] = 2 * w*x + 2 * y*z;	m[2][2] = 1 - 2 * (x*x + y*y); m[2][3] = 0;
		// 从四元数获得 forward, up, right向量

		inline vector3f getForward() const {
			//return vector3f(-2 * getW()*getY() + 2 * getX()*getZ(), 2 * getW()*getX() + 2 * getY()*getZ(), 1 - 2 * (getX()*getX() + getY()*getY())).normal();
			return vector3f(0, 0, 1).rotate(*this);
		}

		inline vector3f getUp() const {
			return vector3f(2 * getX()*getY() + 2 * getW()*getZ(), 1 - 2 * (getX()*getX() + getZ()*getZ()), -2 * getW()*getX() + 2 * getY()*getZ()).normal();
		}

		inline vector3f getRight() const {
			return vector3f(1 - 2 * (getY()*getY() + getZ()*getZ()), 2 * getX()*getY() - 2 * getW()*getZ(), 2 * (getW()*getY() + getX()*getZ())).normal();
		}

	};

	template<class T>
	class matrix4 : public Matrix<T, 4>
	{
	public:
		matrix4() {}
		matrix4(const Matrix<T, 4> &a) { memcpy(m, a.m, sizeof(m)); }
		inline matrix4& operator=(const Matrix<T, 4> &a) {
			memcpy(m, a.m, sizeof(m));
			return *this;
		}
	};

	class matrix4f : public matrix4<float>
	{
	public:
		matrix4f() {}
		matrix4f(const Matrix<float, 4> &a) {
			for(unsigned i=0;i<4;++i) 
			for (unsigned j = 0; j < 4; ++j)
				(*this)[i][j] = a[i][j];
		}
		inline matrix4f& operator=(const Matrix<float, 4> &a) {
			for (unsigned i = 0; i<4; ++i)
			for (unsigned j = 0; j < 4; ++j)
				(*this)[i][j] = a[i][j];
			return *this;
		}


		// 平移矩阵
		inline matrix4f& initTranslate(float x, float y, float z) {
			this->initIdentity();
			m[3][0] = x;
			m[3][1] = y;
			m[3][2] = z;
			return *this;
		}

		inline matrix4f& initTranslate(const vector3f &pos) {
			return initTranslate(pos.getX(), pos.getY(), pos.getZ());
		}

		/*inline matrix4f& inverseTranslate() {
			m[3][0] = -m[3][0];
			m[3][1] = -m[3][1];
			m[3][2] = -m[3][2];
			return *this;
		}*/

		inline matrix4f& initScale(float x, float y, float z) {
			m[0][0] = x; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
			m[1][0] = 0; m[1][1] = y;	m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0; m[2][1] = 0;	m[2][2] = z; m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
			return *this;
		}

		inline matrix4f& initScale(const vector3f &scale) {
			return initScale(scale.getX(), scale.getY(), scale.getZ());
		}

		// 旋转矩阵 Rad单位
		inline matrix4f& initRotateX(float r)
		{
			m[0][0] = 1; m[0][1] = 0;       m[0][2] = 0;      m[0][3] = 0;
			m[1][0] = 0; m[1][1] = cos(r);  m[1][2] = sin(r); m[1][3] = 0;
			m[2][0] = 0; m[2][1] = -sin(r); m[2][2] = cos(r); m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0;       m[3][2] = 0;      m[3][3] = 1;
			return *this;
		}

		inline matrix4f& initRotateY(float r)
		{
			m[0][0] = cos(r); m[0][1] = 0;  m[0][2] = -sin(r); m[0][3] = 0;
			m[1][0] = 0;      m[1][1] = 1;  m[1][2] = 0;       m[1][3] = 0;
			m[2][0] = sin(r); m[2][1] = 0;  m[2][2] = cos(r);  m[2][3] = 0;
			m[3][0] = 0;		  m[3][1] = 0;  m[3][2] = 0;       m[3][3] = 1;
			return *this;
		}

		inline matrix4f& initRotateZ(float r)
		{
			m[0][0] = cos(r);  m[0][1] = sin(r);  m[0][2] = 0; m[0][3] = 0;
			m[1][0] = -sin(r); m[1][1] = cos(r);  m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0;			 m[2][1] = 0;       m[2][2] = 1; m[2][3] = 0;
			m[3][0] = 0;			 m[3][1] = 0;       m[3][2] = 0; m[3][3] = 1;
			return *this;
		}

		inline matrix4f& initRotateEuler(const vector3f &r)
		{
			matrix4f rotz, roty, rotx;
			rotz.initRotateZ(r.getZ());
			roty.initRotateY(r.getY());
			rotx.initRotateX(r.getX());
			return *this = rotz * roty * rotx;
		}

		// 单位四元数求旋转矩阵
		inline matrix4f& initRotateQuaternion(const Quaternion &r)
		{

			m[0][0] = 1 - 2 * (r.getY()*r.getY() + r.getZ()*r.getZ()); m[0][1] = 2 * r.getX()*r.getY() - 2 * r.getW()*r.getZ(); m[0][2] = 2 * (r.getW()*r.getY() + r.getX()*r.getZ()); m[0][3] = 0;
			m[1][0] = 2 * r.getX()*r.getY() + 2 * r.getW()*r.getZ(); m[1][1] = 1 - 2 * (r.getX()*r.getX() + r.getZ()*r.getZ());	m[1][2] = -2 * r.getW()*r.getX() + 2 * r.getY()*r.getZ(); m[1][3] = 0;
			m[2][0] = -2 * r.getW()*r.getY() + 2 * r.getX()*r.getZ(); m[2][1] = 2 * r.getW()*r.getX() + 2 * r.getY()*r.getZ();	m[2][2] = 1 - 2 * (r.getX()*r.getX() + r.getY()*r.getY()); m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;

			return *this;
		}


		inline matrix4f& initView(const vector3f &pos, const vector3f & forward, const vector3f & up)
		{
			vector3f right = up.cross(forward).normal();

			m[0][0] = right.getX(); m[0][1] = up.getX();  m[0][2] = forward.getX(); m[0][3] = 0;
			m[1][0] = right.getY(); m[1][1] = up.getY();	m[1][2] = forward.getY(); m[1][3] = 0;
			m[2][0] = right.getZ(); m[2][1] = up.getZ();	m[2][2] = forward.getZ(); m[2][3] = 0;
			m[3][0] = -pos.dot(right); m[3][1] = -pos.dot(up); m[3][2] = -pos.dot(forward);   m[3][3] = 1;

			return *this;
		}

		inline matrix4f& initProjection(float fov, float ratio, float zNear, float zFar)
		{
			/*xScale     0          0               0
			0        yScale       0               0
			0          0       zf / (zf - zn)     1
			0          0 - zn*zf / (zf - zn)     0*/

			float cotHalfFOV = 1.0f / tan(fov*0.5);
			float Q = zFar / (zFar - zNear);
			m[0][0] = cotHalfFOV / ratio; m[0][1] = 0;								  m[0][2] = 0;			  m[0][3] = 0;
			m[1][0] = 0;									m[1][1] = cotHalfFOV ;	m[1][2] = 0;				m[1][3] = 0;
			m[2][0] = 0;									m[2][1] = 0;									m[2][2] = Q;				m[2][3] = 1;
			m[3][0] = 0;									m[3][1] = 0;									m[3][2] = -zNear*Q; m[3][3] = 0;

			return *this;
		}

	};


	template class vector2<float>;
	typedef vector2<float> vector2f;

}
