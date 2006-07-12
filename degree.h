//. degree.h - �p�x�Ǝ��Ԃ̕ϊ�
//	Copyright (C) 1997,1998,1999,2000 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef DEGREE_H_
#define DEGREE_H_
#include <iostream>
#include <cmath>
#include "defs.h"

namespace util {
using namespace std;

//------------------------------------------------------------------------
//.----- �萔 PI (��) ----------------------------------------------------
//------------------------------------------------------------------------
extern const double PI;    	// ��
extern const double DD2RAD;	// ��/180
extern const double HH2RAD;	// ��/12
extern const double DS2RAD;	// ��/180/3600
extern const double HS2RAD;	// ��/12/3600
extern const double RAD2DD;	// 180/��
extern const double RAD2HH;	// 12/��
extern const double RAD2DS;	// 180*3600/��
extern const double RAD2HS;	// 12*3600/��

//------------------------------------------------------------------------
//.----- fmod1, fmod2 : �ۂߕϊ� -----------------------------------------
//------------------------------------------------------------------------
//. fmod1(x, r) - x �� r �ŏ�]�𓾂� [0...r)      �͈̔͂Ɋۂ߂�
//. fmod2(x, r) - x �� r �ŏ�]�𓾂� [-r/2...r/2) �͈̔͂Ɋۂ߂�
//	��r�͐��̐��ł��邱��
double fmod1(double x, double r);
double fmod2(double x, double r);

//------------------------------------------------------------------------
//.----- round : �l�̌ܓ� ------------------------------------------------
//------------------------------------------------------------------------
//. round(x) - ���������l�̌ܓ����Đ����𓾂�
double round(double x);

//. round(x, point) - ������point-1�����l�̌ܓ����A��������point���Ɋۂ߂�
double round(double x, int point);

//------------------------------------------------------------------------
//.----- sec2ims, min2im : ���b�ϊ� --------------------------------------
//------------------------------------------------------------------------
//. sec2ims(sec, sign, i, m, s) - �b(sec)�̒l�𕄍��A�x���b(i,m,s)�ɕ�����
//. min2im(min, sign, i, m)     - ��(min)�̒l�𕄍��A�x��(i,m)�ɕ�����
//	�������͂O�ȏ�� '+' �Ƃ���
void sec2ims(double sec, char& sign, double& i, double& m, double& s);
void sec2ims(double sec, char& sign, int&    i, int&    m, double& s);
void min2im (double sec, char& sign, double& i, double& m);
void min2im (double sec, char& sign, int&    i, double& m);

//------------------------------------------------------------------------
//.----- dms2xx : �x���b�̕ϊ� -------------------------------------------
//------------------------------------------------------------------------
//. dms2dd(d,m,s)  - �x���b��x�̏����ɂ���
//. dms2ds(d,m,s)  - �x���b��x�b�ɂ���
//. dms2rad(d,m,s) - �x���b�����W�A���ɂ���
//	��d,m,s�͐��̒l�ł��邱��
inline double dms2dd(double d, double m=0, double s=0) {
	return d + m / 60 + s / 3600;
}
inline double dms2ds(double d, double m=0, double s=0) {
	return d * 3600 + m * 60 + s;
}
inline double dms2rad(double d, double m=0, double s=0) {
	return dms2ds(d, m, s) * DS2RAD;
}

//------------------------------------------------------------------------
//.----- hms2xx : �����b�̕ϊ� -------------------------------------------
//------------------------------------------------------------------------
//. hms2hh(h,m,s)  - �����b�����̏����ɂ���
//. hms2hs(h,m,s)  - �����b�����b�ɂ���
//. hms2rad(h,m,s) - �����b�����W�A���ɂ���
//	��h,m,s�͐��̒l�ł��邱��
inline double hms2hh(double h, double m=0, double s=0) {
	return h + m / 60 + s / 3600;
}
inline double hms2hs(double h, double m=0, double s=0) {
	return h * 3600 + m * 60 + s;
}
inline double hms2rad(double h, double m=0, double s=0) {
	return hms2hs(h, m, s) * HS2RAD;
}

//------------------------------------------------------------------------
//.----- ds2xxx, hs2xxx : �x�Ǝ��̒P�ʕϊ��A�ۂ� -------------------------
//------------------------------------------------------------------------
//. dd2rad(dd)     - �x   �� ���W�A�� �ɂ���
//. hh2rad(hh)     - ��   �� ���W�A�� �ɂ���
//. ds2rad(ds)     - �x�b �� ���W�A�� �ɂ���
//. hs2rad(hs)     - ���b �� ���W�A�� �ɂ���
//. rad2dd(dd)     - ���W�A�� �� �x   �ɂ���
//. rad2hh(hh)     - ���W�A�� �� ��   �ɂ���
//. rad2ds(ds)     - ���W�A�� �� �x�b �ɂ���
//. rad2hs(hs)     - ���W�A�� �� ���b �ɂ���
//. dd2mod1(dd)    - �x       �� [0...360)      �͈̔͂Ɋۂ߂�
//. hh2mod1(dd)    - ��       �� [0...24)       �͈̔͂Ɋۂ߂�
//. ds2mod1(ds)    - �x�b     �� [0...360*3600) �͈̔͂Ɋۂ߂�
//. hs2mod1(ds)    - ���b     �� [0...24*3600)  �͈̔͂Ɋۂ߂�
//. rad2mod1(rad)  - ���W�A�� �� [0...2��)      �͈̔͂Ɋۂ߂�
//. dd2mod2(dd)    - �x       �� [-180...180)   �͈̔͂Ɋۂ߂�
//. ds2mod2(ds)    - �x�b     �� [-180*3600...180*3600) �͈̔͂Ɋۂ߂�
//. rad2mod2(rad)  - ���W�A�� �� [-��...��)     �͈̔͂Ɋۂ߂�
inline double dd2rad(double dd) { return dd * DD2RAD; }
inline double hh2rad(double hh) { return hh * HH2RAD; }
inline double ds2rad(double ds) { return ds * DS2RAD; }
inline double hs2rad(double hs) { return hs * HS2RAD; }

inline double rad2dd(double rad) { return rad * RAD2DD; }
inline double rad2hh(double rad) { return rad * RAD2HH; }
inline double rad2ds(double rad) { return rad * RAD2DS; }
inline double rad2hs(double rad) { return rad * RAD2HS; }

inline double dd2mod1(double dd)   { return fmod1(dd, 360); }
inline double ds2mod1(double ds)   { return fmod1(ds, 360*3600L); }
inline double hh2mod1(double hh)   { return fmod1(hh, 24); }
inline double hs2mod1(double hs)   { return fmod1(hs, 24*3600L); }
inline double rad2mod1(double rad) { return fmod1(rad, 2*PI); }

inline double dd2mod2(double dd)   { return fmod2(dd, 360); }
inline double ds2mod2(double ds)   { return fmod2(ds, 360*3600L); }
inline double rad2mod2(double rad) { return fmod2(rad, 2*PI); }

//------------------------------------------------------------------------
//.----- class asXxxx : �_�~�[�^ -----------------------------------------
//------------------------------------------------------------------------
// �R���X�g���N�^�ɓn���_�~�[�^�B�ʂ̈����œn�����l�^�̈Ӗ��t��������B
class asRadian {};
class asHh {};
class asHm {};
class asHs {};
class asDs {};
class asDm {};
class asDd {};

//------------------------------------------------------------------------
//.----- class Degree : �p�x ---------------------------------------------
//------------------------------------------------------------------------
class Degree {
	double ds;		// �p�b[��]  �P����3600�{�B

public:
	//----- �R���X�g���N�^ -------------------------------------------
	Degree()
		: ds(0) {}

	explicit Degree(double ds)
		: ds(ds) {}

	Degree(double x, asRadian) { setRadian(x); }
	Degree(double x, asHh)  { setHh(x); }
	Degree(double x, asHm)  { setHm(x); }
	Degree(double x, asHs)  { setHs(x); }
	Degree(double x, asDd)  { setDegree(x); }
	Degree(double x, asDm)  { setMinute(x); }
	Degree(double x, asDs)  { setSec(x);    }

	Degree(double d, double m, double s)
		: ds(d * 3600 + m * 60 + s) { }

	Degree(const Degree& a)
		: ds(a.ds) {}

	Degree& operator=(const Degree& a) {
		ds = a.ds; return *this;
	}

	//----- ���� -----------------------------------------------------
	friend Degree operator-(const Degree& a) {
		return Degree(-a.ds);
	}

	//----- �����Z ---------------------------------------------------
	Degree& operator+=(const Degree& a) {
		ds += a.ds; return *this;
	}
	Degree& operator-=(const Degree& a) {
		ds -= a.ds; return *this;
	}
	friend Degree operator+(const Degree& a, const Degree& b) {
		return Degree(a.ds + b.ds);
	}
	friend Degree operator-(const Degree& a, const Degree& b) {
		return Degree(a.ds - b.ds);
	}

	//----- �W���揜�Z -----------------------------------------------
	Degree& operator*=(double k) {
		ds *= k; return *this;
	}
	Degree& operator/=(double k) {
		ds /= k; return *this;
	}
	friend Degree operator*(const Degree& a, double k) {
		return Degree(a.ds * k);
	}
	friend Degree operator*(double k, const Degree& a) {
		return Degree(a.ds * k);
	}
	friend Degree operator/(const Degree& a, double k) {
		return Degree(a.ds / k);
	}

	//----- �֌W���Z -------------------------------------------------
	bool operator==(const Degree& a) const { return ds == a.ds; }
	bool operator!=(const Degree& a) const { return ds != a.ds; }
	bool operator< (const Degree& a) const { return ds <  a.ds; }
	bool operator<=(const Degree& a) const { return ds <= a.ds; }
	bool operator> (const Degree& a) const { return ds >  a.ds; }
	bool operator>=(const Degree& a) const { return ds >= a.ds; }

	//----- �������� -------------------------------------------------
	// 0�ȏ�̔���� !isMinus()�ŉ\.
	// 0�ȉ��̔���� !isPlus()�ŉ\.
	bool isZero() const  { return ds == 0; }
	bool isPlus() const  { return ds > 0; } // 0����
	bool isMinus() const { return ds < 0; } // 0��菬

	//----- �������]�A�t�� -------------------------------------------
	void setNeg() { ds = -ds; }
	void setInv() { ds = 1 / ds; }

	//----- �p�x�̊ۂ� -----------------------------------------------
	//. mod360 - 360�x�͈̔͂Ŋp�x���ۂ߁A [0��...360��) �Ƃ���
	void mod360() { ds = fmod1(ds, 360*3600L); }

	//. mod180 - �}180�x�͈̔͂Ŋp�x���ۂ߁A[-180��...180��) �Ƃ���
	void mod180() { ds = fmod2(ds, 360*3600L); }

	//. mod90 - �}180�x�͈̔͂Ŋp�x���ۂ߁A����Ɂ}90�x�ȏ�͊p�x��܂�Ԃ���true��Ԃ�
	// �ܓx�ɑ΂���true���Ԃ�Ƃ��́A�o�x��180�x������K�v������
	bool mod90() {
		mod180();
		if (ds > 90*3600L) {
			ds =  180*3600L - ds; return true;
		}
		else if (ds < -90*3600L) {
			ds = -180*3600L - ds; return true;
		}
		else
			return false;
	}

	//----- �p�x�̒萔���Z -------------------------------------------
	void add180() { ds += 180 * 3600L; }
	void sub180() { ds -= 180 * 3600L; }
	void add90()  { ds +=  90 * 3600L; }
	void sub90()  { ds -=  90 * 3600L; }

	//----- �p�x�擾�E�ݒ� -------------------------------------------
	//. �p�x [��]
	double degree() const    { return ds / 3600; }
	void setDegree(double d) { ds = d * 3600; }

	//. �p�x�� [��]
	double minute() const    { return ds / 60; }
	void setMinute(double m) { ds = m * 60; }

	//. �p�x�b [��]
	double sec() const      { return ds; }
	void setSec(double s)   { ds = s; }

	//. ���p�� [h]
	double hh() const       { return ds / (15*3600L); }
	void setHh(double h)    { ds =   h  * (15*3600L); }

	//. ���p�� [m]
	double hm() const       { return ds / (15*60); }
	void setHm(double m)    { ds =   m  * (15*60); }

	//. ���p�b [s]
	double hs() const           { return ds / 15; }
	void setHs(double s)        { ds =    s * 15; }

	//. ���W�A��
	double radian() const       { return ds * DS2RAD; }
	void setRadian(double rad)  { ds =  rad * RAD2DS; }

	//----- ���b�𕪗����Đݒ�E�擾���� -----------------------------
	//. �x���b
	void setDms(double d, double m, double s) {
		ds = d * 3600 + m * 60 + s;
	}
	void getDms(char& sign, double& d, double& m, double& s) const {
		sec2ims(ds, sign, d, m, s);
	}
	void getDms(char& sign, int& d, int& m, double& s) const {
		sec2ims(ds, sign, d, m, s);
	}
	void getDms(char& sign, int& d, int& m, double& s, int point) const {
		sec2ims(round(ds, point), sign, d, m, s);
	}
	void getDm(char& sign, double& d, double& m) const {
		min2im(minute(), sign, d, m);
	}
	void getDm(char& sign, int& d, double& m) const {
		min2im(minute(), sign, d, m);
	}
	void getDm(char& sign, int& d, double& m, int point) const {
		min2im(round(minute(), point), sign, d, m);
	}

	//. �����b�i���p)
	void setHms(double h, double m, double s) {
		ds = (h * 3600 + m * 60 + s) * 15;
	}
	void getHms(char& sign, double& h, double& m, double& s) const {
		sec2ims(hs(), sign, h, m, s);
	}
	void getHms(char& sign, int& h, int& m, double& s) const {
		sec2ims(hs(), sign, h, m, s);
	}
	void getHms(char& sign, int& h, int& m, double& s, int point) const {
		sec2ims(round(hs(), point), sign, h, m, s);
	}
	void getHm(char& sign, double& h, double& m) const {
		min2im(hm(), sign, h, m);
	}
	void getHm(char& sign, int& h, double& m) const {
		min2im(hm(), sign, h, m);
	}
	void getHm(char& sign, int& h, double& m, int point) const {
		min2im(round(hm(), point), sign, h, m);
	}

	//----- ������ -------------------------------------------------
	ostream& printDegree(ostream& o) const;
	ostream& printRadian(ostream& o) const;
	ostream& printDms(ostream& o) const;
	ostream& printHms(ostream& o) const;
	friend ostream& operator<<(ostream& o, const Degree& a) {
		return a.printDegree(o);
	}
	int sprintDms(char* buf, const char* fmt, int point=0) const; // "%c%02dd %02dm %02.0fs"
	int sprintDm (char* buf, const char* fmt, int point=1) const; // "%c%02dd %04.1fm"
	int sprintHms(char* buf, const char* fmt, int point=0) const; // "%02dh %02dm %02.0fs"
	int sprintHm (char* buf, const char* fmt, int point=1) const; // "%02dh %04.1fm"

	//----- �������� -----------------------------------------------
	static Degree parseDms(const char* p);
	static Degree parseHms(const char* p);

	//----- �O�p�֐� -------------------------------------------------
	friend double sin(const Degree& a)   { return ::sin(a.radian()); }
	friend double cos(const Degree& a)   { return ::cos(a.radian()); }
	friend double tan(const Degree& a)   { return ::tan(a.radian()); }
	void setArcSin(double x)             { setRadian(asin(x)); }
	void setArcCos(double x)             { setRadian(acos(x)); }
	void setArcTan(double x)             { setRadian(atan(x)); }
	void setArcTan2(double y, double x)  { setRadian(atan2(y, x)); }
};//endclass Degree

// �ܓx�o�x�̒l���A�ܓx�}90�x�A�o�x�}180�x�ɐ��K������
inline void normLtLg(Degree& lt, Degree& lg) {
	if (lt.mod90())
		lg.add180();
	lg.mod180();
}

// �Ԍo�Ԉ܂̒l���A�Ԉ܁}90�x�A�Ԍo0�`360�x�ɐ��K������
inline void normRaDec(Degree& ra, Degree& dec) {
	if (dec.mod90())
		ra.add180();
	ra.mod360();
}

}//endnamespace util
#endif // DEGREE_H_
