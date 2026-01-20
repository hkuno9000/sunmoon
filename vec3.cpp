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

/// Mat3x3::setE() - 単位行列設定
void
Mat3x3::setE()
{
	m11 = m22 = m33 = 1;
	m12 = m13 = m21 = m23 = m31 = m32 = 0;
}

/// Mat3x3::setRotate - 回転行列設定
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


/// Mat3x3::operator+=() - 加算
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

/// Mat3x3::operator-=() - 減算
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

/// Mat3x3::operator*=() - 係数乗算
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

/// Mat3x3::operator/=() - 係数除算
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

/// Mat3x3::operator*=() - 乗算
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

/// Vec3::setDirectionCosines - 方向余弦化
void
Vec3::setDirectionCosines()
{
	const double r = radius();
	if (r != 0)
		*this /= r;
}

/// Vec3::radius - 動径、ベクトルの長さ
double
Vec3::radius() const
{
	return ::sqrt(square());
}

/// Vec3::getPolar - 極座標を得る
///@param phi     天頂角φ (0～180°)
///@param theta   方位角θ (-180°～+180°)
///@return        動径ｒ
double
Vec3::getPolar(Degree& phi, Degree& theta) const
{
	// 方位角θを得る
	theta.setArcTan2(y, x); // -180°～+180°

	// 天頂角φを得る
	phi.setArcTan2(::sqrt(x * x + y * y), z);

	// 長さを計算する
	return radius();
}

/// Vec3::getLtLg - 緯度経度を得る
///@param latitude  緯度
///@param longitude 経度(東経を正とする)
///@return          動径ｒ
double
Vec3::getLtLg(Degree& latitude, Degree& longitude) const
{
	// 経度(==方位角θ)を得る
	longitude.setArcTan2(y, x); // -180°～+180°

	// 緯度(==90°- 天頂角φ)を得る
	latitude.setArcTan2(z, ::sqrt(x * x + y * y));

	// 長さを計算する
	return radius();
}

/// Vec3::setPolar - 極座標を設定する
///@param radius  動径ｒ
///@param phi     天頂角φ
///@param theta   方位角θ
void
Vec3::setPolar(double radius, const Degree& phi, const Degree& theta)
{
	const double sin_phi = sin(phi);
	x = radius * sin_phi * cos(theta);
	y = radius * sin_phi * sin(theta);
	z = radius * cos(phi);
}

/// Vec3::setLtLg - 緯度経度を設定する
///@param radius    動径ｒ
///@param latitude  緯度
///@param longitude 経度(東経を正とする)
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
