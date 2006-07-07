//. degree.cpp - �p�x�Ǝ��Ԃ̕ϊ�
//	Copyright (C) 1997,1998,1999,2000 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include <iostream>
#include "degree.h"
using namespace util;

//------------------------------------------------------------------------
//.----- �萔 PI (��) ----------------------------------------------------
//------------------------------------------------------------------------
namespace util {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
extern const double PI = M_PI;
extern const double DD2RAD = M_PI / 180;
extern const double HH2RAD = M_PI /  12;
extern const double DS2RAD = M_PI / (180*3600L);
extern const double HS2RAD = M_PI / ( 12*3600L);
extern const double RAD2DD = 180 / M_PI;
extern const double RAD2HH = 12  / M_PI;
extern const double RAD2DS = (180*3600L) / M_PI;
extern const double RAD2HS = ( 12*3600L) / M_PI;
}//endnamespace util

//------------------------------------------------------------------------
//.----- fmod1, fmod2 : �ۂߕϊ� -----------------------------------------
//------------------------------------------------------------------------
//. fmod1(x, r) - x �� r �ŏ�]�𓾂� [0...r)      �͈̔͂Ɋۂ߂�
//. fmod2(x, r) - x �� r �ŏ�]�𓾂� [-r/2...r/2) �͈̔͂Ɋۂ߂�
//	��r�͐��̐��ł��邱��

double
util::fmod1(double x, double r)
{
	// m = fmod(x, r) �ɂ� m�̕�����x�Ɠ���
	if ((x = fmod(x, r)) < 0)
		x += r;
	return x;
}

double
util::fmod2(double x, double r)
{
	// m = fmod(x, r) �ɂ� m�̕�����x�Ɠ���
	if ((x = fmod(x, r)) < 0)
		x += r;
	if (x >= r/2)
		x -= r;
	return x;
}

//------------------------------------------------------------------------
//.----- round : �l�̌ܓ� ------------------------------------------------
//------------------------------------------------------------------------
//. round(x) - �����_���ʂŎl�̌ܓ�����
//. round(x, point) - ������point-1�����l�̌ܓ����A��������point���Ɋۂ߂�
double
util::round(double x)
{
	return (x < 0) ? ceil(x - 0.5) : floor(x + 0.5);
}

double
util::round(double x, int point)
{
	double base = pow(10, point);
	return round(x * base) / base;
}

//------------------------------------------------------------------------
//.----- sec2ims, min2im : ���b�ϊ� --------------------------------------
//------------------------------------------------------------------------
//. sec2ims(sec, sign, i, m, s) - �b(sec)�̒l�𕄍��A�x���b(i,m,s)�ɕ�����
//. min2im(min, sign, i, m)     - ��(min)�̒l�𕄍��A�x��(i,m)�ɕ�����
//	�������͂O�ȏ�� '+' �Ƃ���
void
util::sec2ims(double sec, char& sign, double&i, double& m, double& s)
{
	sign = (sec < 0) ? '-' : '+';
	double f = fabs(sec);
	i = floor(f / 3600); f = f - i * 3600;
	m = floor(f / 60  ); s = f - m * 60;
}

void
util::sec2ims(double sec, char& sign, int&i, int& m, double& s)
{
	sign = (sec < 0) ? '-' : '+';
	double f = fabs(sec);
	i = floor(f / 3600); f = f - i * 3600.0;
	m = floor(f / 60  ); s = f - m * 60;
}

void
util::min2im(double min, char& sign, double&i, double& m)
{
	sign = (min < 0) ? '-' : '+';
	double f = fabs(min);
	i = floor(f / 60); m = f - i * 60;
}

void
util::min2im(double min, char& sign, int&i, double& m)
{
	sign = (min < 0) ? '-' : '+';
	double f = fabs(min);
	i = floor(f / 60); m = f - i * 60;
}

//------------------------------------------------------------------------
//.----- class Degree : �p�x ---------------------------------------------
//------------------------------------------------------------------------

//----- ������ ---------------------------------------------------------

ostream&
Degree::printDegree(ostream& o) const
{
	return o << degree();
}

ostream&
Degree::printRadian(ostream& o) const
{
	return o << radian();
}

ostream&
Degree::printDms(ostream& o) const
{
	char c;
	double d, m, s;
	getDms(c, d, m, s);
	return o << c << d << "d" << m << "m" << s << "s";
}

ostream&
Degree::printHms(ostream& o) const
{
	char c;
	double h, m, s;
	getHms(c, h, m, s);
	return o << c << h << "h" << m << "m" << s << "s";
}

int
Degree::sprintDms(char* buf, const char* fmt, int point) const
{
	char sign;
	int d, m;
	double s;
	Degree w(*this);
	w.mod180();
	w.getDms(sign, d, m, s, point);
	if (d == 0 && m == 0 && s == 0) sign = ' '; // �[���̂Ƃ��͕����Ȃ�
	if (!fmt) fmt = "%c%02dd %02dm %02.0fs";
	return std::sprintf(buf, fmt, sign, d, m, s);
}

int
Degree::sprintDm(char* buf, const char* fmt, int point) const
{
	char sign;
	int d;
	double m;
	Degree w(*this);
	w.mod180();
	w.getDm(sign, d, m, point);
	if (d == 0 && m == 0) sign = ' '; // �[���̂Ƃ��͕����Ȃ�
	if (!fmt) fmt = "%c%02dd %04.1fm";
	return std::sprintf(buf, fmt, sign, d, m);
}

int
Degree::sprintHms(char* buf, const char* fmt, int point) const
{
	char c;
	int h, m;
	double s;
	Degree w(*this);
	w.mod360();
	w.getHms(c, h, m, s, point);
	if (!fmt) fmt = "%02dh %02dm %02.0fs";
	return std::sprintf(buf, fmt, h, m, s);
}

int
Degree::sprintHm(char* buf, const char* fmt, int point) const
{
	char c;
	int h;
	double m;
	Degree w(*this);
	w.mod360();
	w.getHm(c, h, m, point);
	if (!fmt) fmt = "%02dh %04.1fm";
	return std::sprintf(buf, fmt, h, m);
}

//----- �������� -------------------------------------------------------
Degree
Degree::parseDms(const char* p)
{
	char sign = '+';
	char sep[5];
	double d = 0, m = 0, s = 0;
	unsigned i;

	// �󔒂�ǂݔ�΂�
	while (*p && isspace((uchar) *p))
		++p;

	// �����𓾂�
	if (*p == '+' || *p == '-')
		sign = *p++;

	// �e������d,m,s�̎擾�����݂�
	if (sscanf(p, "%lf%c%lf%1[m']%lf", &d, sep, &m, sep, &s) == 5) {
		// DD?MM'SS["] or DDdMMmSS[s]
	}
	else if (s = 0, sscanf(p, "%lf %lf %lf", &d, &m, &s) >= 2) {
		// DD MM SS or DD MM
	}
	else if (s = 0, sscanf(p, "%lf%c%lf", &d, sep, &m) == 3) {
		// DD?MM
	}
	else if (strchr(p, '.') == NULL && strlen(p) > 4
			&& sscanf(p, "%u", &i) == 1) {
		// DDMMSS
		d = (i / 10000);
		m = (i / 100) % 100;
		s = i % 100;
	}
	else if (sscanf(p, "%lf", &d) == 1) {
		// DD.DDD
		m = s = 0;
	}
	else
		d = m = s = 0;
	Degree deg(d, m, s);
	if (sign == '-')
		deg.setNeg();
	return deg;
}

Degree
Degree::parseHms(const char* p)
{
	char sign = '+';
	char sep[5];
	double h = 0, m = 0, s = 0;
	unsigned i;

	// �󔒂�ǂݔ�΂�
	while (*p && isspace((uchar) *p))
		++p;

	// �����𓾂�
	if (*p == '+' || *p == '-')
		sign = *p++;

	// �e������h,m,s�̎擾�����݂�
	if (sscanf(p, "%lf%1[h:]%lf%1[m:]%lf", &h, sep, &m, sep, &s) == 5) {
		// HH:MM:SS or HHhMMmSS[s]
	}
	else if (s = 0, sscanf(p, "%lf %lf %lf", &h, &m, &s) >= 2) {
		// HH MM SS or HH MM
	}
	else if (s = 0, sscanf(p, "%lf%c%lf", &h, sep, &m) == 3) {
		// HH?MM
	}
	else if (strchr(p, '.') == NULL && strlen(p) > 4
			&& sscanf(p, "%u", &i) == 1) {
		// HHMMSS
		h = (i / 10000);
		m = (i / 100) % 100;
		s = i % 100;
	}
	else if (sscanf(p, "%lf", &h) == 1) {
		// HH.HHH .
		m = s = 0;
	}
	else
		h = m = s = 0;
	Degree deg;
	deg.setHms(h, m, s);
	if (sign == '-')
		deg.setNeg();
	return deg;
}

//. degree.cpp - end.
