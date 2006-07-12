//. vec3.cpp - 3D vector
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include "vec3.h"
#include "degree.h"

namespace util {
//------------------------------------------------------------------------
//.----- class Mat3x3 ----------------------------------------------------
//------------------------------------------------------------------------

//. Mat3x3::setE() - �P�ʍs��ݒ�
void
Mat3x3::setE()
{
	m11 = m22 = m33 = 1;
	m12 = m13 = m21 = m23 = m31 = m32 = 0;
}

//. Mat3x3::setRotate - ��]�s��ݒ�
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


//. Mat3x3::operator+=() - ���Z
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

//. Mat3x3::operator-=() - ���Z
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

//. Mat3x3::operator*=() - �W����Z
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

//. Mat3x3::operator/=() - �W�����Z
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

//. Mat3x3::operator*=() - ��Z
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
//.----- class Vec3 : �P�s�R��̃x�N�g�� ---------------------------------
//------------------------------------------------------------------------

//. Vec3::setDirectionCosines - �����]����
void
Vec3::setDirectionCosines()
{
	const double r = radius();
	if (r != 0)
		*this /= r;
}

//. Vec3::radius - ���a�A�x�N�g���̒���
double
Vec3::radius() const
{
	return ::sqrt(square());
}

//. Vec3::getPolar - �ɍ��W�𓾂�
// @param phi     �V���p�� (0�`180��)
// @param theta   ���ʊp�� (-180���`+180��)
// @return        ���a��
double
Vec3::getPolar(Degree& phi, Degree& theta) const
{
	// ���ʊp�Ƃ𓾂�
	theta.setArcTan2(y, x); // -180���`+180��

	// �V���p�ӂ𓾂�
	phi.setArcTan2(::sqrt(x * x + y * y), z);

	// �������v�Z����
	return radius();
}

//. Vec3::getLtLg - �ܓx�o�x�𓾂�
// @param latitude  �ܓx
// @param longitude �o�x(���o�𐳂Ƃ���)
// @return          ���a��
double
Vec3::getLtLg(Degree& latitude, Degree& longitude) const
{
	// �o�x(==���ʊp��)�𓾂�
	longitude.setArcTan2(y, x); // -180���`+180��

	// �ܓx(==90��- �V���p��)�𓾂�
	latitude.setArcTan2(z, ::sqrt(x * x + y * y));

	// �������v�Z����
	return radius();
}

//. Vec3::setPolar - �ɍ��W��ݒ肷��
// @param radius  ���a��
// @param phi     �V���p��
// @param theta   ���ʊp��
void
Vec3::setPolar(double radius, const Degree& phi, const Degree& theta)
{
	const double sin_phi = sin(phi);
	x = radius * sin_phi * cos(theta);
	y = radius * sin_phi * sin(theta);
	z = radius * cos(phi);
}

//. Vec3::setLtLg - �ܓx�o�x��ݒ肷��
// @param radius    ���a��
// @param latitude  �ܓx
// @param longitude �o�x(���o�𐳂Ƃ���)
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
