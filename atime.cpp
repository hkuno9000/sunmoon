/// @file
/// 天文時刻
/// @author hkuno9000@gmail.com
/// @copyright 1997,1998,2014,2015,2018,2026 Hiroshi Kuno. MIT License
/// @see <https://github.com/hkuno9000/sunmoon.git>
#include <ctime>
#include "jday.h"
#include "degree.h"	//!@bug VC6 bug? 直前に"namespace astro {}"を置くと degree.hの ostream参照でコンパイルエラーとなる.
#include "atime.h"
#if defined(WIN32)
#include <windows.h>
#elif defined(BSD) || defined(__APPLE__) || defined(__CYGWIN__) || defined(__linux__)
#include <sys/time.h>
#endif
using namespace std;
using namespace util;
using namespace astro;
namespace astro {

//------------------------------------------------------------------------
//.----- class AstroTime : 天文時刻 --------------------------------------
//------------------------------------------------------------------------

//----- コンストラクタ ---------------------------------------------------
// dut1とleapSecは適当な初期値にする。
// 厳密な計算のためには、setDUT1(),setLeapSec()で設定すること。

/// 閏秒の初期値 [TAI-UTC]
int AstroTime::initLeapSec = 37;
const char* AstroTime::initLeapText = "+37(2017 January 1). see https://hpiers.obspm.fr/iers/bul/bulc/bulletinc.dat";

/// 現在時刻で生成する
AstroTime::AstroTime()
	: d(0), s(0), dut1(0), leapSec(initLeapSec)
{
	update();
}

/// 指定の日付と時刻で生成する
/// @param jd  世界時正午の日付
/// @param utc 世界時０時からの経過秒数
AstroTime::AstroTime(const Jday& jday, double utc)
	: d(jday), s(utc - 12*3600L), dut1(0), leapSec(initLeapSec)
{
	adjust();
}


//----- 日付と時刻の調整 -------------------------------------------------
/// AstroTime::adjust - 秒が日を越えないようにする
void
AstroTime::adjust()
{
	while (s >= 86400.0)
		d += 1, s -= 86400.0;
	while (s < 0)
		d -= 1, s += 86400.0;
}

//----- 現在時刻に更新する -----------------------------------------------
/// AstroTime::update - 現在時刻(秒単位)に更新する
void
AstroTime::update()
{
	time_t tt = time(0);
	struct tm& t = *gmtime(&tt);
	d.setGdate(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	s = hms2hs(t.tm_hour, t.tm_min, t.tm_sec)
		- 12 * 3600.0; // 世界時正午が0になるように補正する
	adjust();
}

/// AstroTime::updateSystemTime - 現在時刻(ミリ秒単位)に更新する
void
AstroTime::updateSystemTime()
{
#if defined(WIN32)
	::SYSTEMTIME t;
	::GetSystemTime(&t); // 世界時で日付時刻を得る
	d.setGdate(t.wYear, t.wMonth, t.wDay);
	s = hms2hs(t.wHour, t.wMinute, t.wSecond + t.wMilliseconds / 1000.0)
		- 12 * 3600.0; // 世界時正午が0になるように補正する
	adjust();
#elif defined(BSD) || defined(__APPLE__) || defined(__CYGWIN__) || defined(__linux__)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	d.setGdate(1970, 1, 1);
	d += (tv.tv_sec / 86400);
	s =  (tv.tv_sec % 86400) + (tv.tv_usec / 1e6) - 12 * 3600;
	adjust();
#else
#error implements to your OS
#endif
}

//----- 恒星時 -----------------------------------------------------------
/// AstroTime::gmst - グリニジ平均恒星時[秒] [0～86400)
double
AstroTime::gmst() const
{
	// 理科年表 世界時の項より
	// GMST = UT1 - 12h + αm
	// αm  = 18h 41m 50.54841s
	//      + 8640184.812866s Tu + 0.093104s Tu^2 - 0.0000062s Tu^3
	// Tu = 2000年1月1日12時(UT1)からユリウス世紀単位で測った世界時
	// UT1 = UTC + DUT1 (世界時)
	// ※αmはグリニジ基準天体(平均太陽)
	// この式でほぼ 0.001秒の精度がある
	// しかし DUT1の精度は 0.1秒であるため、実質は 0.1秒の精度となる
	// さらにDUT1が未設定なら UT1とUTCの最大差である±0.9秒が精度となる
	const double Tu = (jd1() - Jday::AD2000_1_1_12UT) / 36525U;	// ユリウス世紀
	const double Tu2 = Tu * Tu;
	const double Tu3 = Tu * Tu * Tu;
	const double am = hms2hs(18, 41, 50.54841)
	            + 8640184.812866 * Tu + 0.093104 * Tu2 - 0.0000062 * Tu3;
	return hs2mod1(ut1() + am); // ut1() = UT1 - 12h
}

}//.endnamespace astro
//------------------------------------------------------------------------
#ifdef TEST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
using namespace util;
using namespace astro;
int main(int argc, char** argv)
{
	if (argc == 2 && strcmp(argv[1], "update") == 0) {
		// 現在時間更新.
		int y, m, d; double utc;
		AstroTime at;
		at.get(y, m, d, utc);
		printf("%04d.%02d.%02d %5.8g\n", y, m, d, utc);
		at.updateSystemTime();
		at.get(y, m, d, utc);
		printf("%04d.%02d.%02d %5.8g\n", y, m, d, utc);
		return EXIT_SUCCESS;
	}

	Jday dd(0);
	char buf[256];
	int y, m=1, d=0, sec=0;
	fputs("atime test. imput y,m,d,sec\n",stderr);
	while (fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "%d.%d.%d.%d", &y, &m, &d, &sec) < 3)
			continue;
		dd.setGdate(y, m, d);
		AstroTime at(dd, sec);
		double gmst = at.gmst();
		char c; int hh, mm; double ss;
		sec2ims(gmst, c, hh, mm, ss);
		printf("%4d.%02d.%02d.%05d, jd:%10.8g, gmst:%10.8g (%02dh %02dm %.3fs)\n",
			y, m, d, sec, at.jd(), gmst, hh, mm, ss);
	}
	return EXIT_SUCCESS;
}
/*
1997.01.00.00000, jd: 2450448.5, gmst: 23927.588 (06h 38m 47.588s)
1997.03.01.00000, jd: 2450508.5, gmst:  38120.91 (10h 35m 20.910s)
1997.04.01.00000, jd: 2450539.5, gmst: 45454.126 (12h 37m 34.126s)
1997.08.01.00000, jd: 2450661.5, gmst: 74313.881 (20h 38m 33.881s)
1997.12.00.43198, jd:   2450783, gmst: 59853.353 (16h 37m 33.353s)
1997.12.00.43199, jd:   2450783, gmst: 59854.355 (16h 37m 34.355s)
1997.12.00.43200, jd:   2450783, gmst: 59855.358 (16h 37m 35.358s)
1997.12.00.43201, jd:   2450783, gmst: 59856.361 (16h 37m 36.361s)
1997.12.00.86397, jd: 2450783.5, gmst: 16770.628 (04h 39m 30.628s)
1997.12.00.86398, jd: 2450783.5, gmst:  16771.63 (04h 39m 31.630s)
1997.12.00.86399, jd: 2450783.5, gmst: 16772.633 (04h 39m 32.633s)
1997.12.01.00000, jd: 2450783.5, gmst: 16773.636 (04h 39m 33.636s)
1978.06.10.00000, jd: 2443669.5, gmst: 61918.753 (17h 11m 58.753s)
*/
#endif
//. atime.cpp - end
