/// @file
/// 3D vector
/// @author hkuno9000@gmail.com
/// @copyright 1997,1998,2026 Hiroshi Kuno. MIT License
/// @see <https://github.com/hkuno9000/sunmoon.git>
#include "vec3.h"
#include "degree.h"

namespace util {
//------------------------------------------------------------------------
//.----- class Mat3x3 ----------------------------------------------------
//------------------------------------------------------------------------

///
void
Mat3x3::setE()
{
	m11 = m22 = m33 = 1;
	m12 = m13 = m21 = m23 = m31 = m32 = 0;
}

/// @details 設定される行列を以下に示す.
///	\n\n ```setRotate(a, 'X')``` => @f$ \begin{pmatrix} 1 & 0 & 0 \\ 0 & +\cos a & +\sin a \\ 0 & -\sin a & +\cos a \end{pmatrix} @f$
///	\n\n ```setRotate(a, 'Y')``` => @f$ \begin{pmatrix} +\cos a & 0 & -\sin a \\ 0 & 1 & 0 \\ -\sin a & 0 & +\cos a \end{pmatrix} @f$
///	\n\n ```setRotate(a, 'Z')``` => @f$ \begin{pmatrix} +\cos a & +\sin a & 0 \\ -\sin a & +\cos a & 0 \\ 0 & 0 & 1 \end{pmatrix} @f$
void
Mat3x3::setRotate(const Degree& angle, int axis)
{
	const double s = sin(angle);
	const double c = cos(angle);
	switch (axis) {
	case 'X':
		m11 =  1; m12 =  0; m13 =  0;
		m21 =  0; m22 =  c; m23 =  s;
		m31 =  0; m32 = -s; m33 =  c;
		break;
	case 'Y':
		m11 =  c; m12 =  0; m13 = -s;
		m21 =  0; m22 =  1; m23 =  0;
		m31 =  s; m32 =  0; m33 =  c;
		break;
	case 'Z':
		m11 =  c; m12 =  s; m13 =  0;
		m21 = -s; m22 =  c; m23 =  0;
		m31 =  0; m32 =  0; m33 =  1;
		break;
	default:
		throw "Mat3x3::setRotate(angle, axis) - bad axis";
	}//endswitch axis
}


/// 加算
Mat3x3&
Mat3x3::operator+=(const Mat3x3& m)
{
	#define M(N)	m##N += m.m##N
	M(11); M(12); M(13);
	M(21); M(22); M(23);
	M(31); M(32); M(33);
	#undef  M
	return *this;
}

/// 減算
Mat3x3&
Mat3x3::operator-=(const Mat3x3& m)
{
	#define M(N)	m##N -= m.m##N
	M(11); M(12); M(13);
	M(21); M(22); M(23);
	M(31); M(32); M(33);
	#undef  M
	return *this;
}

/// 係数乗算
Mat3x3&
Mat3x3::operator*=(double k)
{
	#define M(N)	m##N *= k
	M(11); M(12); M(13);
	M(21); M(22); M(23);
	M(31); M(32); M(33);
	#undef  M
	return *this;
}

/// 係数除算
Mat3x3&
Mat3x3::operator/=(double k)
{
	#define M(N)	m##N /= k
	M(11); M(12); M(13);
	M(21); M(22); M(23);
	M(31); M(32); M(33);
	#undef  M
	return *this;
}

/// 乗算
Mat3x3&
Mat3x3::operator*=(const Mat3x3& m)
{
	#define M(L,C) \
		m##L##C = a.m##L##1 * m.m1##C \
		        + a.m##L##2 * m.m2##C \
		        + a.m##L##3 * m.m3##C
	Mat3x3 a(*this);
	M(1,1); M(1,2); M(1,3);
	M(2,1); M(2,2); M(2,3);
	M(3,1); M(3,2); M(3,3);
	#undef  M
	return *this;
}


//------------------------------------------------------------------------
//.----- class Vec3 : １行３列のベクトル ---------------------------------
//------------------------------------------------------------------------

/// 方向余弦化
void
Vec3::setDirectionCosines()
{
	const double r = radius();
	if (r != 0)
		*this /= r;
}

/// 動径、ベクトルの長さ
double
Vec3::radius() const
{
	return std::sqrt(square());
}

/// 極座標を得る
double
Vec3::getPolar(Degree& phi, Degree& theta) const
{
	// 方位角θを得る
	theta.setArcTan2(y, x); // -180°～+180°

	// 天頂角φを得る
	phi.setArcTan2(std::sqrt(x * x + y * y), z);

	// 長さを計算する
	return radius();
}

/// 緯度経度を得る
double
Vec3::getLtLg(Degree& latitude, Degree& longitude) const
{
	// 経度(==方位角θ)を得る
	longitude.setArcTan2(y, x); // -180°～+180°

	// 緯度(==90°- 天頂角φ)を得る
	latitude.setArcTan2(z, std::sqrt(x * x + y * y));

	// 長さを計算する
	return radius();
}

/// 極座標を設定する
void
Vec3::setPolar(double radius, const Degree& phi, const Degree& theta)
{
	const double sin_phi = sin(phi);
	x = radius * sin_phi * cos(theta);
	y = radius * sin_phi * sin(theta);
	z = radius * cos(phi);
}

/// 緯度経度を設定する
void
Vec3::setLtLg(double radius, const Degree& latitude, const Degree& longitude)
{
	const double cos_lt = cos(latitude);
	x = radius * cos_lt * cos(longitude);
	y = radius * cos_lt * sin(longitude);
	z = radius * sin(latitude);
}

}//.endnamespace util
//. vec3.cpp - end
