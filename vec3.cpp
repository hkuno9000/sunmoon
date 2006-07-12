//. vec3.cpp - 3D vector
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include "vec3.h"
#include "degree.h"

namespace util {
//------------------------------------------------------------------------
//.----- class Mat3x3 ----------------------------------------------------
//------------------------------------------------------------------------

//. Mat3x3::setE() - ’PˆÊs—ñİ’è
void
Mat3x3::setE()
{
	m11 = m22 = m33 = 1;
	m12 = m13 = m21 = m23 = m31 = m32 = 0;
}

//. Mat3x3::setRotate - ‰ñ“]s—ñİ’è
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


//. Mat3x3::operator+=() - ‰ÁZ
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

//. Mat3x3::operator-=() - Œ¸Z
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

//. Mat3x3::operator*=() - ŒW”æZ
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

//. Mat3x3::operator/=() - ŒW”œZ
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

//. Mat3x3::operator*=() - æZ
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
//.----- class Vec3 : ‚Ps‚R—ñ‚ÌƒxƒNƒgƒ‹ ---------------------------------
//------------------------------------------------------------------------

//. Vec3::setDirectionCosines - •ûŒü—]Œ·‰»
void
Vec3::setDirectionCosines()
{
	const double r = radius();
	if (r != 0)
		*this /= r;
}

//. Vec3::radius - “®ŒaAƒxƒNƒgƒ‹‚Ì’·‚³
double
Vec3::radius() const
{
	return ::sqrt(square());
}

//. Vec3::getPolar - ‹ÉÀ•W‚ğ“¾‚é
// @param phi     “V’¸ŠpƒÓ (0`180‹)
// @param theta   •ûˆÊŠpƒÆ (-180‹`+180‹)
// @return        “®Œa‚’
double
Vec3::getPolar(Degree& phi, Degree& theta) const
{
	// •ûˆÊŠpƒÆ‚ğ“¾‚é
	theta.setArcTan2(y, x); // -180‹`+180‹

	// “V’¸ŠpƒÓ‚ğ“¾‚é
	phi.setArcTan2(::sqrt(x * x + y * y), z);

	// ’·‚³‚ğŒvZ‚·‚é
	return radius();
}

//. Vec3::getLtLg - ˆÜ“xŒo“x‚ğ“¾‚é
// @param latitude  ˆÜ“x
// @param longitude Œo“x(“ŒŒo‚ğ³‚Æ‚·‚é)
// @return          “®Œa‚’
double
Vec3::getLtLg(Degree& latitude, Degree& longitude) const
{
	// Œo“x(==•ûˆÊŠpƒÆ)‚ğ“¾‚é
	longitude.setArcTan2(y, x); // -180‹`+180‹

	// ˆÜ“x(==90‹- “V’¸ŠpƒÓ)‚ğ“¾‚é
	latitude.setArcTan2(z, ::sqrt(x * x + y * y));

	// ’·‚³‚ğŒvZ‚·‚é
	return radius();
}

//. Vec3::setPolar - ‹ÉÀ•W‚ğİ’è‚·‚é
// @param radius  “®Œa‚’
// @param phi     “V’¸ŠpƒÓ
// @param theta   •ûˆÊŠpƒÆ
void
Vec3::setPolar(double radius, const Degree& phi, const Degree& theta)
{
	const double sin_phi = sin(phi);
	x = radius * sin_phi * cos(theta);
	y = radius * sin_phi * sin(theta);
	z = radius * cos(phi);
}

//. Vec3::setLtLg - ˆÜ“xŒo“x‚ğİ’è‚·‚é
// @param radius    “®Œa‚’
// @param latitude  ˆÜ“x
// @param longitude Œo“x(“ŒŒo‚ğ³‚Æ‚·‚é)
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
