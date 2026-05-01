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
	m11 += m.m11; m12 += m.m12; m13 += m.m13;
	m21 += m.m21; m22 += m.m22; m23 += m.m23;
	m31 += m.m31; m32 += m.m32; m33 += m.m33;
	return *this;
}

/// 減算
Mat3x3&
Mat3x3::operator-=(const Mat3x3& m)
{
	m11 -= m.m11; m12 -= m.m12; m13 -= m.m13;
	m21 -= m.m21; m22 -= m.m22; m23 -= m.m23;
	m31 -= m.m31; m32 -= m.m32; m33 -= m.m33;
	return *this;
}

/// 係数乗算
Mat3x3&
Mat3x3::operator*=(double k)
{
	m11 *= k; m12 *= k; m13 *= k;
	m21 *= k; m22 *= k; m23 *= k;
	m31 *= k; m32 *= k; m33 *= k;
	return *this;
}

/// 係数除算
Mat3x3&
Mat3x3::operator/=(double k)
{
	m11 /= k; m12 /= k; m13 /= k;
	m21 /= k; m22 /= k; m23 /= k;
	m31 /= k; m32 /= k; m33 /= k;
	return *this;
}

/// 乗算
Mat3x3&
Mat3x3::operator*=(const Mat3x3& m)
{
	const Mat3x3 a(*this);
	m11 = a.m11 * m.m11 + a.m12 * m.m21 + a.m13 * m.m31;
	m12 = a.m11 * m.m12 + a.m12 * m.m22 + a.m13 * m.m32;
	m13 = a.m11 * m.m13 + a.m12 * m.m23 + a.m13 * m.m33;
	m21 = a.m21 * m.m11 + a.m22 * m.m21 + a.m23 * m.m31;
	m22 = a.m21 * m.m12 + a.m22 * m.m22 + a.m23 * m.m32;
	m23 = a.m21 * m.m13 + a.m22 * m.m23 + a.m23 * m.m33;
	m31 = a.m31 * m.m11 + a.m32 * m.m21 + a.m33 * m.m31;
	m32 = a.m31 * m.m12 + a.m32 * m.m22 + a.m33 * m.m32;
	m33 = a.m31 * m.m13 + a.m32 * m.m23 + a.m33 * m.m33;
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
