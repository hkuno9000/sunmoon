/// @file
/// 角度と時間の変換
/// @author hkuno9000@gmail.com
/// @copyright 1997,1998,1999,2000,2014,2026 Hiroshi Kuno. MIT License
/// @see <https://github.com/hkuno9000/sunmoon.git>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "degree.h"
namespace util {

//------------------------------------------------------------------------

double fmod1(double x, double r)
{
	// m = fmod(x, r) にて mの符号はxと同じ
	x = std::fmod(x, r);
	if (x < 0)
		x += r;
	return x;
}

double fmod2(double x, double r)
{
	// m = fmod(x, r) にて mの符号はxと同じ
	x = std::fmod(x, r);
	if (x < 0)
		x += r;
	if (x >= r/2)
		x -= r;
	return x;
}

//------------------------------------------------------------------------

double round(double x)
{
#if 0 // for OLD C++
	return (x < 0) ? std::ceil(x - 0.5) : std::floor(x + 0.5);
#else // ISO C++11 or later
	return std::round(x);
#endif
}

double round(double x, int point)
{
	double base = std::pow(10.0, point);
	return round(x * base) / base;
}

//------------------------------------------------------------------------

void sec2ims(double sec, char& sign, double&i, double& m, double& s)
{
	sign = (sec < 0) ? '-' : '+';
	double f = std::fabs(sec);
	i = std::floor(f / 3600); f = f - i * 3600;
	m = std::floor(f / 60  ); s = f - m * 60;
}

void sec2ims(double sec, char& sign, int&i, int& m, double& s)
{
	sign = (sec < 0) ? '-' : '+';
	double f = std::fabs(sec);
	i = (int)std::floor(f / 3600); f = f - i * 3600.0;
	m = (int)std::floor(f / 60  ); s = f - m * 60;
}

void min2im(double min, char& sign, double&i, double& m)
{
	sign = (min < 0) ? '-' : '+';
	double f = std::fabs(min);
	i = std::floor(f / 60); m = f - i * 60;
}

void min2im(double min, char& sign, int&i, double& m)
{
	sign = (min < 0) ? '-' : '+';
	double f = std::fabs(min);
	i = (int)std::floor(f / 60); m = f - i * 60;
}

//------------------------------------------------------------------------

std::ostream&
Degree::printDegree(std::ostream& o) const
{
	return o << degree();
}

std::ostream&
Degree::printRadian(std::ostream& o) const
{
	return o << radian();
}

std::ostream&
Degree::printDms(std::ostream& o) const
{
	char c;
	double d, m, s;
	getDms(c, d, m, s);
	return o << c << d << "d" << m << "m" << s << "s";
}

std::ostream&
Degree::printHms(std::ostream& o) const
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
	if (d == 0 && m == 0 && s == 0) sign = ' '; // ゼロのときは符号なし
	if (!fmt) fmt = "%c%02dd %02dm %02.0fs";
	return ::sprintf(buf, fmt, sign, d, m, s);
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
	if (d == 0 && m == 0) sign = ' '; // ゼロのときは符号なし
	if (!fmt) fmt = "%c%02dd %04.1fm";
	return ::sprintf(buf, fmt, sign, d, m);
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
	return ::sprintf(buf, fmt, h, m, s);
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
	return ::sprintf(buf, fmt, h, m);
}

//------------------------------------------------------------------------

Degree
Degree::parseDms(const char* p)
{
	char sign = '+';
	char sep[5];
	double d = 0, m = 0, s = 0;
	unsigned i;

	// 空白を読み飛ばす
	while (*p && std::isspace((uchar) *p))
		++p;

	// 符号を得る
	if (*p == '+' || *p == '-')
		sign = *p++;

	// 各書式でd,m,sの取得を試みる
	if (std::sscanf(p, "%lf%c%lf%1[m']%lf", &d, sep, &m, sep, &s) == 5) {
		// DD?MM'SS["] or DDdMMmSS[s]
	}
	else if (s = 0, std::sscanf(p, "%lf %lf %lf", &d, &m, &s) >= 2) {
		// DD MM SS or DD MM
	}
	else if (s = 0, std::sscanf(p, "%lf%c%lf", &d, sep, &m) == 3) {
		// DD?MM
	}
	else if (std::strchr(p, '.') == nullptr && std::strlen(p) > 4
			&& std::sscanf(p, "%u", &i) == 1) {
		// DDMMSS
		d = (i / 10000);
		m = (i / 100) % 100;
		s = i % 100;
	}
	else if (std::sscanf(p, "%lf", &d) == 1) {
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

	// 空白を読み飛ばす
	while (*p && std::isspace((uchar) *p))
		++p;

	// 符号を得る
	if (*p == '+' || *p == '-')
		sign = *p++;

	// 各書式でh,m,sの取得を試みる
	if (std::sscanf(p, "%lf%1[h:]%lf%1[m:]%lf", &h, sep, &m, sep, &s) == 5) {
		// HH:MM:SS or HHhMMmSS[s]
	}
	else if (s = 0, std::sscanf(p, "%lf %lf %lf", &h, &m, &s) >= 2) {
		// HH MM SS or HH MM
	}
	else if (s = 0, std::sscanf(p, "%lf%c%lf", &h, sep, &m) == 3) {
		// HH?MM
	}
	else if (std::strchr(p, '.') == nullptr && std::strlen(p) > 4
			&& std::sscanf(p, "%u", &i) == 1) {
		// HHMMSS
		h = (i / 10000);
		m = (i / 100) % 100;
		s = i % 100;
	}
	else if (std::sscanf(p, "%lf", &h) == 1) {
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

}//endnamespace util
//. degree.cpp - end.
