//. Vec3.h - 3D vector
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef VEC3_H_
#define VEC3_H_
#include "defs.h"

namespace util {
using namespace std;
class Degree;

//------------------------------------------------------------------------
//.----- class Mat3x3 ----------------------------------------------------
//------------------------------------------------------------------------
// �P�s�R��̃x�N�g�����A�t�B���ϊ�����R�s�R��̍s��ł���
//	������Z�̑��ɁA��]�s��ݒ�A�P�ʍs��ݒ肪�ł���B
class Mat3x3 {
public:
	//----- �R���X�g���N�^ -------------------------------------------

	// �P�ʍs��Ƃ��Đ�������
	Mat3x3() {
		setE();
	}

	// ��]�s��Ƃ��Đ�������
	// @param angle ��]�p�x
	// @param axis  ��]��('X','Y','Z' �̂����ꂩ���w�肷��)
	Mat3x3(const Degree& angle, int axis='Z') {
		setRotate(angle, axis);
	}

	// �w���3x3�v�f���������s��Ƃ��Đ�������
	Mat3x3(double m11, double m12, double m13,
	       double m21, double m22, double m23,
	       double m31, double m32, double m33)
		: m11(m11), m12(m12), m13(m13),
		  m21(m21), m22(m22), m23(m23),
		  m31(m31), m32(m32), m33(m33) {}


	//----- �P�ʍs��ݒ� ---------------------------------------------
	void setE();


	//----- ��]�s��ݒ� ---------------------------------------------
	// �x�N�g���̒������W�������A�E��n(�e�w=x�A�l�����w=y�A���w=z)�̂Ƃ�
	// ��]�s��́A�x�N�g�������̂悤�ɉ�]����B
	//	�E���́{�������猴�_�����āA���̉��𔽎��v���(���l�W)
	//	�E���_���玲�́{���������āA���̉������v���(�E�l�W)
	// �x�N�g�����Œ肵�č��W�n�̉�]�Ƃ݂�Ύ��̂悤�ɂȂ�B
	//	�E���́{�������猴�_�����āA���̉������v���(�E�l�W)
	// @param angle ��]�p�x
	// @param axis  ��]��('X','Y','Z' �̂����ꂩ���w�肷��)
	void setRotate(const Degree& angle, int axis='Z');


	//----- ������Z -------------------------------------------------
	Mat3x3& operator+=(const Mat3x3&);
	Mat3x3& operator-=(const Mat3x3&);
	Mat3x3& operator*=(const Mat3x3&);

	friend Mat3x3 operator+(const Mat3x3& a, const Mat3x3& b) {
		Mat3x3 c(a); return c += b;
	}

	friend Mat3x3 operator-(const Mat3x3& a, const Mat3x3& b) {
		Mat3x3 c(a); return c -= b;
	}

	friend Mat3x3 operator*(const Mat3x3& a, const Mat3x3& b) {
		Mat3x3 c(a); return c *= b;
	}


	//----- �W���揜�Z -----------------------------------------------
	Mat3x3& operator*=(double k);
	Mat3x3& operator/=(double k);

	friend Mat3x3 operator*(const Mat3x3& a, double k) {
		Mat3x3 c(a); return c *= k;
	}

	friend Mat3x3 operator*(double k, const Mat3x3& a) {
		Mat3x3 c(a); return c *= k;
	}

	friend Mat3x3 operator/(const Mat3x3& a, double k) {
		Mat3x3 c(a); return c /= k;
	}


	//----- �s��f�[�^�����o -----------------------------------------
	// ���O�����璼�ڕύX����Ă�����Ȃ��̂ŁA���J���Ă��܂�
	double m11, m12, m13,
	       m21, m22, m23,
	       m31, m32, m33;

};//endclass Mat3x3

// �R���X�g���N�^�ɓn���_�~�[�^�B�ʂ̈����œn�����l�^�̈Ӗ��t��������B
class asLtLg {};

//------------------------------------------------------------------------
//.----- class Vec3 : �P�s�R��̃x�N�g�� ---------------------------------
//------------------------------------------------------------------------
// �P�s�R��̎����x�N�g���B�R�����������W��ێ�����
class Vec3 {
public:
	//----- �R���X�g���N�^ -------------------------------------------

	// �S��0�Ő�������
	Vec3()
		: x(0), y(0), z(0) {}

	// �������W�l�Ő�������
	Vec3(double x, double y, double z)
		: x(x), y(y), z(z) {}

	// �ɍ��W�l�Ő�������
	// @param radius  ���a��
	// @param phi     �V���p��
	// @param theta   ���ʊp��
	Vec3(double radius, const Degree& phi, const Degree& theta) {
		setPolar(radius, phi, theta);
	}

	// �ܓx�o�x�l�Ő�������
	// @param radius    ���a��
	// @param latitude  �ܓx
	// @param longitude �o�x(���o�𐳂Ƃ���)
	Vec3(double radius, const Degree& latitude, const Degree& longitude, asLtLg) {
		setLtLg(radius, latitude, longitude);
	}

	// �������W�l�̔z�񂩂琶������
	Vec3(const int*    p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const short*  p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const long*   p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const double* p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const float*  p) : x(p[0]), y(p[1]), z(p[2]) {}


	//----- �����Z ---------------------------------------------------
	Vec3& operator+=(const Vec3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vec3& operator-=(const Vec3& a) {
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	friend Vec3 operator+(const Vec3& a, const Vec3& b) {
		return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend Vec3 operator-(const Vec3& a, const Vec3& b) {
		return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}


	//----- �W���揜�Z -----------------------------------------------
	Vec3& operator*=(double k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	Vec3& operator/=(double k) {
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	friend Vec3 operator*(const Vec3& a, double k) {
		return Vec3(a.x * k, a.y * k, a.z * k);
	}

	friend Vec3 operator*(double k, const Vec3& a) {
		return Vec3(a.x * k, a.y * k, a.z * k);
	}

	friend Vec3 operator/(const Vec3& a, double k) {
		return Vec3(a.x / k, a.y / k, a.z / k);
	}


	//----- �s�񉉎Z -------------------------------------------------
	Vec3& operator*=(const Mat3x3& m) {
		//             | m11 m12 m13 |
		// (x, y, z) * | m21 m22 m23 |
		//             | m31 m32 m33 |
		double xx = x, yy = y, zz = z;
		x = xx * m.m11 + yy * m.m21 + zz * m.m31;
		y = xx * m.m12 + yy * m.m22 + zz * m.m32;
		z = xx * m.m13 + yy * m.m23 + zz * m.m33;
		return *this;
	}

	friend Vec3 operator*(const Vec3& a, const Mat3x3& b) {
		return Vec3(
			a.x * b.m11 + a.y * b.m21 + a.z * b.m31,
			a.x * b.m12 + a.y * b.m22 + a.z * b.m32,
			a.x * b.m13 + a.y * b.m23 + a.z * b.m33
		);
	}


	//----- ���� -----------------------------------------------------
	// ���ϒl = |a|*|b|*cos�� = a.x * b.x + a.y * b.y + a.z * b.z
	double inner(const Vec3& a) const {
		return x * a.x + y * a.y + z * a.z;
	}


	//----- �����a ---------------------------------------------------
	double square() const {
		return x * x + y * y + z * z;
	}


	//----- �����]���� -----------------------------------------------
	void setDirectionCosines();


	//----- �ɍ��W�̐ݒ�E�擾 ---------------------------------------

	// ���a�A�x�N�g���̒���
	double radius() const;

	// �ɍ��W�𓾂�
	// @param phi     �V���p��
	// @param theta   ���ʊp��
	// @return        ���a��
	double getPolar(Degree& phi, Degree& theta) const;

	// �ɍ��W��ݒ肷��
	// @param radius  ���a��
	// @param phi     �V���p��
	// @param theta   ���ʊp��
	void setPolar(double radius, const Degree& phi, const Degree& theta);

	// �ܓx�o�x�𓾂�
	// @param latitude  �ܓx
	// @param longitude �o�x(���o�𐳂Ƃ���)
	// @return          ���a��
	double getLtLg(Degree& latitude, Degree& longitude) const;

	// �ܓx�o�x��ݒ肷��
	// @param radius    ���a��
	// @param latitude  �ܓx
	// @param longitude �o�x(���o�𐳂Ƃ���)
	void setLtLg(double radius, const Degree& latitude, const Degree& longitude);


	//----- �������W�l�f�[�^�����o -----------------------------------
	// ���O�����璼�ڕύX����Ă�����Ȃ��̂ŁA���J���Ă��܂�
	double x, y, z;

};//endclass Vec3

}//endnamespace util
#endif // VEC3_H_
