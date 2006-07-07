//. degree.cpp - 角度と時間の変換
//	Copyright (C) 1997,1998,1999,2000 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include <iostream>
#include "degree.h"
using namespace util;

//------------------------------------------------------------------------
//.----- 定数 PI (π) ----------------------------------------------------
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
//.----- fmod1, fmod2 : 丸め変換 -----------------------------------------
//------------------------------------------------------------------------
//. fmod1(x, r) - x を r で剰余を得て [0...r)      の範囲に丸める
//. fmod2(x, r) - x を r で剰余を得て [-r/2...r/2) の範囲に丸める
//	※rは正の数であること

double
util::fmod1(double x, double r)
{
	// m = fmod(x, r) にて mの符号はxと同じ
	if ((x = fmod(x, r)) < 0)
		x += r;
	return x;
}

double
util::fmod2(double x, double r)
{
	// m = fmod(x, r) にて mの符号はxと同じ
	if ((x = fmod(x, r)) < 0)
		x += r;
	if (x >= r/2)
		x -= r;
	return x;
}

//------------------------------------------------------------------------
//.----- round : 四捨五入 ------------------------------------------------
//------------------------------------------------------------------------
//. round(x) - 小数点第一位で四捨五入する
//. round(x, point) - 小数部point-1桁を四捨五入し、小数部をpoint桁に丸める
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
//.----- sec2ims, min2im : 分秒変換 --------------------------------------
//------------------------------------------------------------------------
//. sec2ims(sec, sign, i, m, s) - 秒(sec)の値を符号、度分秒(i,m,s)に分ける
//. min2im(min, sign, i, m)     - 分(min)の値を符号、度分(i,m)に分ける
//	※符号は０以上を '+' とする
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
//.----- class Degree : 角度 ---------------------------------------------
//------------------------------------------------------------------------

//----- 文字列化 ---------------------------------------------------------

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
	if (d == 0 && m == 0 && s == 0) sign = ' '; // ゼロのときは符号なし
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
	if (d == 0 && m == 0) sign = ' '; // ゼロのときは符号なし
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

//----- 文字列解析 -------------------------------------------------------
Degree
Degree::parseDms(const char* p)
{
	char sign = '+';
	char sep[5];
	double d = 0, m = 0, s = 0;
	unsigned i;

	// 空白を読み飛ばす
	while (*p && isspace((uchar) *p))
		++p;

	// 符号を得る
	if (*p == '+' || *p == '-')
		sign = *p++;

	// 各書式でd,m,sの取得を試みる
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

	// 空白を読み飛ばす
	while (*p && isspace((uchar) *p))
		++p;

	// 符号を得る
	if (*p == '+' || *p == '-')
		sign = *p++;

	// 各書式でh,m,sの取得を試みる
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
