/**@file sunmoon - 太陽、月の高度計算
 * @copyright 2006,2014 hkuno@willsoft.co.jp
 * $Id: sunmoon.cpp,v 1.3 2006-08-08 07:01:16 hkuno Exp $
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "degree.h"
#include "acoord.h"
#include "planets.h"

using namespace std;
using namespace util;
using namespace astro;

//------------------------------------------------------------------------
static const char* gAzimuthName[] = {
	"South","SSW","SW","WSW","West","WNW","NW","NNW",
	"North","NNE","NE","ENE","East","ESE","SE","SSE","South"
};

int sprintAzAlt(char* buf, const Vec3& v)
{
	Degree az, alt;
	v.getLtLg(alt, az);
	az.setNeg(); az.mod360();   // 南を0°とし時計周り(西90°、北180°、東270°)
	double azDeg = az.degree();
	const char* azName = gAzimuthName[(int)util::round(azDeg * 16 / 360)];
	return sprintf(buf, "%06.2fd(%s)  %+05.2fd", azDeg, azName, alt.degree());
}

void utc2localtime(int y, int m, int d, int hh, int mm, int sec, struct tm& t)
{
	t.tm_sec = sec;
	t.tm_min = mm;
	t.tm_hour = hh;
	t.tm_mday = d;
	t.tm_mon = m - 1;
	t.tm_year = y - 1900;
	t.tm_wday = 0;
	t.tm_yday = 0;
	t.tm_isdst = 0;
#ifdef WIN32
	time_t tt = _mkgmtime(&t);
#else
	time_t tt = timegm(&t);
#endif
	t = *localtime(&tt);
}

//------------------------------------------------------------------------
void print(const AstroCoordinate& acoord, double sea, const Vec3& sunH, const Vec3& moonH, double moonPhase)
{
	char buf[256];
	char c;
	int y, m, d, hh, mm;
	double utc, sec;
	Degree az, alt;

	acoord.latitude().sprintDms(buf, NULL);
	printf("LT: %s\n", buf);

	acoord.longitude().sprintDms(buf, NULL);
	printf("LG: %c%s\n", (buf[0]=='-' ? 'W' : 'E'), buf+1);

	printf("SEA: %.0fm\n", sea);

	acoord.getTime().get(y, m, d, utc);
	sec2ims(utc, c, hh, mm, sec);
	printf("UTC: %04d-%02d-%02dT%02d:%02d:%02d\n", y, m, d, hh, mm, (int)sec);

	struct tm t;
	utc2localtime(y, m, d, hh, mm, (int)sec, t);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X %Z", &t);
	printf("LOC: %s\n", buf);

	sunH.getLtLg(alt, az);
	printf("SUN-ALT: %+05.2fd\n", alt.degree());

	moonH.getLtLg(alt, az);
	printf("MOON-ALT: %+05.2fd\n", alt.degree());

	printf("MOON-PHASE: %+05.2fd\n", moonPhase);
}

//------------------------------------------------------------------------
void print_table(const char* prompt, const AstroTime& atime)
{
	char buf[256];
	char c;
	int y, m, d, hh, mm;
	double utc, sec;
	Degree az, alt;

	atime.get(y, m, d, utc);
	sec2ims(utc, c, hh, mm, sec);
	struct tm t;
	utc2localtime(y, m, d, hh, mm, (int)sec, t);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X %Z", &t);
	printf("%s: %s\n", prompt, buf);
}

//------------------------------------------------------------------------
const char gUsage[] =
	"usage: sunmoon [-r] lt=<LT> lg=<LG> [sea=<SEA>] [utc=<UTC>] [table=<DAYS>]\n"
	" version 2015.1\n"
	"   -r  : add refraction to alt\n"
	"   LT  : latidute.  default is NAGOYA '35d10m00s'\n"
	"   LG  : longitude. default is NAGOYA '136d55m00s'\n"
	"   SEA : sea level altitude[m]. default is 0\n"
	"   UTC : ISO 8601 time format '2014-12-31T23:59:59'. default is current time\n"
	"   DAYS: time table days of sunrise, sunset, moonrise and moonset. default is 0\n"
	;

/** -r: 大気差補正ON */
bool gAddRefraction = false;

/** table: 出没表日数. */
unsigned gTableDays = 0;

//------------------------------------------------------------------------
int main(int argc, char** argv)
{
#ifdef WIN32
	_tzset();
#else
	tzset();
#endif
	// nagoya
	Degree lt(35, 10, 00);	//  35.16666..
	Degree lg(136, 55, 00);	// 136.91666..
	double sea = 0;

	// UTC
	int y, m, d, hh=0, mm=0; double sec=0;

	// astro class
	AstroCoordinate acoord;	// 現在時刻で初期化.
	Planets pl;

	//--- コマンドラインを解析する.
	if (argc < 2) {
show_help:
		fputs(gUsage, stderr);
		exit(1);
	}
	while (argc > 1) {
		char* arg = argv[1];
		if (strcmp(arg, "help") == 0)
			goto show_help;
		if (strcmp(arg, "-r") == 0)
			gAddRefraction = true;
		else if (strncmp(arg, "lt=", 3) == 0)
			lt = Degree::parseDms(arg + 3);
		else if (strncmp(arg, "lg=", 3) == 0)
			lg = Degree::parseDms(arg + 3);
		else if (sscanf(arg, "sea=%lf", &sea) == 1)
			;
		else if (sscanf(arg, "table=%u", &gTableDays) == 1)
			;
		else if (sscanf(arg, "utc=%d-%d-%dT%d:%d:%lf", &y, &m, &d, &hh, &mm, &sec) >= 3) {
			//--- 時間設定.
			AstroTime atime(Jday(y, m, d), hh*3600+mm*60+sec);
			acoord.setTime(atime);
		}
		else {
			fputs("unknown argument.\n", stderr);
			goto show_help;
		}
		++argv;
		--argc;
	}

	//--- 計算実行.
	acoord.setPosition(lg, lt);			// 天文緯度経度を設定する.　厳密には測地緯度経度と同じ値を与えてはいけない.
	acoord.setLocation(lg, lt, sea);	// 測地緯度経度と海抜高度を設定する.
	acoord.beginConvert();
	pl.calc(acoord);
	Vec3 sun  = pl.vecQ(Planets::SUN);
	Vec3 moon = pl.vecQ(Planets::MOON);
	double cosSun = sun.inner(moon);
	if (cosSun > 1) cosSun = 1; // acos()でのDOMAINエラー回避.

	acoord.conv_q2h(sun);
	acoord.conv_q2h(moon);
	if (gAddRefraction) { // 大気差補正.
		acoord.addRefraction(sun);
		acoord.addRefraction(moon);
	}

	//--- 結果表示.
	print(acoord, sea, sun, moon, rad2dd(acos(cosSun)));

	//--- 出没計算.
	if (gTableDays != 0) {
		AstroTime t = acoord.getTime();
		const double jd_end = t.jd() + gTableDays;
		const double sun_z  = sin(dms2rad(0,0,960));	// 太陽視半径による出没補正. 視半径は 960" で決め打ち.
		const double min3_z = sin(hms2rad(0,3,0));		// 時角3分の高度のz座標値.
		int step = -1;	// 初回は指定時刻の1秒前の高度を計算する.
		for (t.addSec(step); t.jd() < jd_end; t.addSec(step)) {
			// 前回時刻の高度を保存する. ただし、初回はこの値を使ってはいけない.
			const Vec3 sun0 = sun;
			const Vec3 moon0 = moon;
			// 今回時刻の高度を計算する.
			acoord.setTime(t);
			acoord.beginConvert();
			pl.calc(acoord);
			sun  = pl.vecQ(Planets::SUN);
			moon = pl.vecQ(Planets::MOON);
			acoord.conv_q2h(sun);
			acoord.conv_q2h(moon);
			// 大気差補正は常時実施する.
			acoord.addRefraction(sun);
			acoord.addRefraction(moon);
			if (step > 0) {
				// 前回時刻の高度と比較し、境界値を跨いだ時刻を出没時刻として表示する.
				if (sun0.z < -sun_z && sun.z >= -sun_z) print_table("SUN-RISE", t);
				if (sun0.z >= -sun_z && sun.z < -sun_z) print_table("SUN-SET",  t);
				if (moon0.z < 0 && moon.z >= 0) print_table("MOON-RISE", t);
				if (moon0.z >= 0 && moon.z < 0) print_table("MOON-SET",  t);
			}
			if (fabs(sun.z - sun_z) > min3_z && fabs(moon.z) > min3_z)
				step = 60; // 高度が±時角3分以上なら1分単位でざっくり時刻を進める.
			else
				step = 1; // 高度が±時角3分以内なら1秒単位で時刻を進める.
		}
	}
	return EXIT_SUCCESS;
}
// sunmoon.cpp - end.
