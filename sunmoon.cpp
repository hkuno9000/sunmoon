/**@file sunmoon - 太陽、月の高度計算
 * @copyright 2006,2014,2015 hkuno@willsoft.co.jp
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
	return sprintf(buf, "%06.2fd(%s)  %+06.2fd", azDeg, azName, alt.degree());
}

int sprintRaDec(char* buf, const Vec3& v)
{
	char* p = buf;
	Degree ra, dec;
	v.getLtLg(dec, ra);
	normRaDec(ra, dec);
	p += ra.sprintHms(p, NULL);
	p += sprintf(p, "(%06.2fd)  ", ra.degree());
	p += dec.sprintDms(p, NULL);
	p += sprintf(p, "(%+06.2fd)", dec.degree());
	return p - buf;
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

inline double min_value(double a, double b)
{
	return a < b ? a : b;
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

	Degree lst; lst.setHs(acoord.lst()); lst.sprintHms(buf, NULL);
	printf("LST: %s\n", buf);

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
	"usage: sunmoon [-r] [-p] lt=<LT> lg=<LG> [sea=<SEA>] [utc=<UTC>] [leap=<LEAP>] [table=<DAYS>]\n"
	" version 2015.7\n"
	"   -r  : add refraction to ALT\n"
	"   -p  : print RADEC,J2000,AZALT\n"
	"   LT  : latidute.  default is NAGOYA '35d10m00s'\n"
	"   LG  : longitude. default is NAGOYA '136d55m00s'\n"
	"   SEA : sea level altitude[m]. default is 0\n"
	"   UTC : ISO 8601 time format '2014-12-31T23:59:59'. default is current time\n"
	"   DAYS: time table days of sunrise, sunset, moonrise, moonset and culmination. default is 0\n"
	"   LEAP: TAI-UTC leap seconds. default is %s\n";
	;

/** -r: 大気差補正ON */
bool gAddRefraction = false;

/** -p: 全惑星の赤経赤緯表示ON */
bool gPlanetRaDc = false;

/** table: 出没表日数. */
unsigned gTableDays = 0;

//------------------------------------------------------------------------
void print_planet(const AstroCoordinate& acoord, const Planets& pl, const char* name, int id)
{
	char azalt[256];
	char radec[256];
	char j2000[256];

	Vec3 v = pl.vecQ(id);	// 地心平均赤道座標(MOONのみ測心)
	acoord.conv_q2tq(v);	// 平均位置→真位置(章動補正).
	Vec3 q = v;
	acoord.conv_q2h(v);		// 赤道座標→地平座標.
	if (gAddRefraction) acoord.addRefraction(v);	// 大気差補正.

	sprintAzAlt(azalt, v);
	sprintRaDec(radec, q);
	sprintRaDec(j2000, pl.vecJ(id));
	printf("%-8s: RADEC[%s], J2000[%s], AZALT[%s]\n", name, radec, j2000, azalt);
}

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
	int leap;

	// astro class
	AstroCoordinate acoord;	// 現在時刻で初期化.
	Planets pl;

	//--- コマンドラインを解析する.
	if (argc < 2) {
show_help:
		fprintf(stderr, gUsage, AstroTime::initLeapText);
		exit(1);
	}
	while (argc > 1) {
		char* arg = argv[1];
		if (strcmp(arg, "help") == 0)
			goto show_help;
		if (strcmp(arg, "-r") == 0)
			gAddRefraction = true;
		else if (strcmp(arg, "-p") == 0)
			gPlanetRaDc = true;
		else if (strncmp(arg, "lt=", 3) == 0)
			lt = Degree::parseDms(arg + 3);
		else if (strncmp(arg, "lg=", 3) == 0)
			lg = Degree::parseDms(arg + 3);
		else if (sscanf(arg, "sea=%lf", &sea) == 1)
			;
		else if (sscanf(arg, "leap=%d", &leap) == 1)
			acoord.setLeapSec(AstroTime::initLeapSec = leap);
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
	double cosSun = sun.inner(moon);	// sun/moonは方向余弦なので、その内積は位相角のcosである.
	if (cosSun > 1) cosSun = 1;			// acos()でのDOMAINエラー回避.
	if (cosSun < -1) cosSun = -1;		// acos()でのDOMAINエラー回避.
	Degree phase; phase.setArcCos(cosSun);	// acos は 0..180度の範囲で値を返す.
	if (sun.x * moon.y - sun.y * moon.x < 0) { // XY平面の外積値が負の値なら、位相角度を 180～360度の範囲に補正する.
		phase.setNeg(); phase.mod360();
	}
	acoord.conv_q2tq(sun);
	acoord.conv_q2tq(moon);
	acoord.conv_q2h(sun);
	acoord.conv_q2h(moon);
	if (gAddRefraction) { // 大気差補正.
		acoord.addRefraction(sun);
		acoord.addRefraction(moon);
	}

	//--- 結果表示.
	print(acoord, sea, sun, moon, phase.degree());

	//--- 全惑星の赤経赤緯表示.
	if (gPlanetRaDc) {
		print_planet(acoord, pl, "SUN",     Planets::SUN);
		print_planet(acoord, pl, "MOON",    Planets::MOON);
		print_planet(acoord, pl, "MERCURY", Planets::MERCURY);
		print_planet(acoord, pl, "VENUS",   Planets::VENUS);
		print_planet(acoord, pl, "MARS",    Planets::MARS);
		print_planet(acoord, pl, "JUPITER", Planets::JUPITER);
		print_planet(acoord, pl, "SATURN",  Planets::SATURN);
		print_planet(acoord, pl, "URANUS",  Planets::URANUS);
		print_planet(acoord, pl, "NEPTUNE", Planets::NEPTUNE);
		print_planet(acoord, pl, "PLUTO",   Planets::PLUTO);
	}

	//--- 出没計算.
	if (gTableDays != 0) {
		AstroTime t = acoord.getTime();
		const double jd_end = t.jd() + gTableDays;
		const double sun_rz  = sin(dms2rad(0,0,960));	// 太陽視半径による出没補正. 視半径は 960" で決め打ち.
		const double min30_z = sin(hms2rad(0,30,0));	// 時角30分の高度のz座標値.
		const double min3_z  = sin(hms2rad(0,3,0));	// 時角1分の高度のz座標値.
		const double sec15_z = sin(hms2rad(0,0,15));	// 時角15秒の高度のz座標値.
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
			acoord.conv_q2tq(sun);
			acoord.conv_q2tq(moon);
			acoord.conv_q2h(sun);
			acoord.conv_q2h(moon);
			// 大気差補正は常時実施する.
			acoord.addRefraction(sun);
			acoord.addRefraction(moon);
			// 太陽視半径分を高度補正する.
			sun.z += sun_rz;
			if (step > 0) {
				// 前回時刻の高度と比較し、境界値を跨いだ時刻を出没時刻として表示する.
				if (sun0.z < 0 && sun.z >= 0) print_table("SUN-RISE", t);
				if (sun0.z >= 0 && sun.z < 0) print_table("SUN-SET",  t);
				if (moon0.z < 0 && moon.z >= 0) print_table("MOON-RISE", t);
				if (moon0.z >= 0 && moon.z < 0) print_table("MOON-SET",  t);
				// 前回時刻の東西と比較し、子午線を跨いだ時刻を南中時刻として表示する.
				if (sun0.y >= 0 && sun.y < 0) print_table("SUN-CULM",  t);
				if (moon0.y >= 0 && moon.y < 0) print_table("MOON-CULM",  t);
			}
			double z = min_value(fabs(sun.z), fabs(moon.z));
			double y = min_value(fabs(sun.y), fabs(moon.y));
			z = min_value(z, y);	// 地平線通過、子午線通過付近の最小座標値を求める.
			if (z >= min30_z)
				step = 20*60; // 高度が±時角30分以上なら20分単位で時刻を進める.
			else if (z >= min3_z)
				step = 2*60; // 高度が±時角3分以上なら2分単位で時刻を進める.
			else if (z >= sec15_z)
				step = 10; // 高度が±時角15秒以上なら10秒単位で時刻を進める.
			else
				step = 1; // 1秒単位で時刻を進める.
		}
	}
	return EXIT_SUCCESS;
}
// sunmoon.cpp - end.
