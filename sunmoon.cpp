/**@file sunmoon - 太陽、月の高度計算
 * @copyright 2006,2014,2015,2018 hkuno@willsoft.co.jp
 * $Id: sunmoon.cpp,v 1.3 2006-08-08 07:01:16 hkuno Exp $
 */
#include <cstdio>
#include <cstdarg>
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
void print_location(const AstroCoordinate& acoord, double sea)
{
	char buf[256];

	acoord.latitude().sprintDms(buf, NULL);
	printf("LT: %s\n", buf);

	acoord.longitude().sprintDms(buf, NULL);
	printf("LG: %c%s\n", (buf[0]=='-' ? 'W' : 'E'), buf+1);

	printf("SEA: %.0fm\n", sea);
}

//------------------------------------------------------------------------
void print_time(const AstroCoordinate& acoord)
{
	char buf[256];
	char c;
	int y, m, d, hh, mm;
	double utc, sec;

	acoord.getTime().get(y, m, d, utc);
	sec2ims(utc, c, hh, mm, sec);
	printf("UTC: %04d-%02d-%02dT%02d:%02d:%02d\n", y, m, d, hh, mm, (int)sec);

	struct tm t;
	utc2localtime(y, m, d, hh, mm, (int)sec, t);
	strftime(buf, sizeof(buf), "%Y-%m-%d %X %Z", &t);
	printf("LOC: %s\n", buf);

	Degree lst; lst.setHs(acoord.lst()); lst.sprintHms(buf, NULL);
	printf("LST: %s\n", buf);
}

//------------------------------------------------------------------------
void print_alt(const Vec3& sunH, const Vec3& moonH, double moonPhase)
{
	Degree az, alt;

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
/** short help-message */
const char gUsage[] = "usage: sunmoon [-h?rpj] [lt=<LT>] [lg=<LG>] [sea=<SEA>] [utc=<UTC>] [repeat=<N>,<STEP>] [leap=<LEAP>] [table=<DAYS>]\n";

/** detail help-message for options and version */
const char gDetailHelp[] =
	"  version 2018.5\n"
	"  -h -?: this help\n"
	"  -r   : add refraction to ALT\n"
	"  -p   : print RADEC,J2000,AZALT of Sun, Moon and planets\n"
	"  -j   : print J2000 only for -p\n"
	"  LT   : latidute.  default is NAGOYA '35d10m00s'\n"
	"  LG   : longitude. default is NAGOYA '136d55m00s'\n"
	"  SEA  : sea level altitude[m]. default is 0\n"
	"  UTC  : ISO 8601 time format '2014-12-31T23:59:59'. default is current time\n"
	"  N    : repeat count. default is 1\n"
	"  STEP : repeat step days,hours,minutes or seconds. default is '1day'\n"
	"  DAYS : time table days of sunrise, sunset, moonrise, moonset and culmination. default is 0\n"
	"  LEAP : TAI-UTC leap seconds. default is %s\n"
	"\n  supports and source codes at: https://github.com/hkuno9000/sunmoon/\n"
	;

/** -r: 大気差補正ON */
bool gAddRefraction = false;

/** -p: 全惑星の赤経赤緯表示ON */
bool gPlanetRaDc = false;

/** -j: J2000限定表示ON */
bool gJ2000only = false;

/** table: 出没表日数. */
unsigned gTableDays = 0;

/** repeat count: 繰り返し回数. */
unsigned gRepeatCount = 1;

/** repeat step: 繰り返し刻み値 [秒単位]. */
long gRepeatStep = 1;

//------------------------------------------------------------------------
/** usageとエラーメッセージを表示後に、exitする */
void error_abort(const char* msg, ...)
{
	fputs(gUsage, stderr);
	if (msg) {
		va_list arg;
		va_start(arg, msg);
		vfprintf(stderr, msg, arg);
		va_end(arg);
	}
	exit(EXIT_FAILURE);
}

//------------------------------------------------------------------------
void print_planet(const AstroCoordinate& acoord, const Planets& pl, const char* name, int id)
{
	char azalt[256];
	char radec[256];
	char j2000[256];

	sprintRaDec(j2000, pl.vecJ(id));
	if (gJ2000only) {
		printf("%-8s: J2000[%s]\n", name, j2000);
		return;
	}
	Vec3 v = pl.vecQ(id);	// 地心平均赤道座標(MOONのみ測心)
	acoord.conv_q2tq(v);	// 平均位置→真位置(章動補正).
	Vec3 q = v;
	acoord.conv_q2h(v);		// 赤道座標→地平座標.
	if (gAddRefraction) acoord.addRefraction(v);	// 大気差補正.

	sprintAzAlt(azalt, v);
	sprintRaDec(radec, q);
	printf("%-8s: RADEC[%s], J2000[%s], AZALT[%s]\n", name, radec, j2000, azalt);
}

//------------------------------------------------------------------------
void calc_main(AstroCoordinate& acoord, Planets& pl)
{
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
	print_time(acoord);
	print_alt(sun, moon, phase.degree());

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
	while (argc > 1) {
		char* arg = argv[1];
		char unit = 'd';
		if (arg[0] == '-') {
			char* sw = arg+1;
			do {
				switch (*sw) {
				case 'h': case '?':
					goto show_help;
				case 'r':
					gAddRefraction = true; break;
				case 'p':
					gPlanetRaDc = true; break;
				case 'j':
					gJ2000only = true; break;
				default:
					error_abort("unknown option: -%c\n", *sw);
				}
			} while (*++sw);
		}
		else if (strcmp(arg, "help") == 0) {
show_help:
			error_abort(gDetailHelp, AstroTime::initLeapText);
		}
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
		else if (sscanf(arg, "repeat=%u,%ld%c", &gRepeatCount, &gRepeatStep, &unit) >= 1) {
			//--- 繰り返し設定.
			switch (unit) {
			default:
			error_abort("unknown repeat step unit: '%c', unit should be 'd/h/m/s'.\n", unit, arg);
			case 'd': gRepeatStep *= 24*3600L; break;
			case 'h': gRepeatStep *= 3600L; break;
			case 'm': gRepeatStep *= 60L; break;
			case 's': break;
			}
		}
		else {
			error_abort("unknown argument: %s\n", arg);
		}
		++argv;
		--argc;
	}

	//--- 計算実行.
	acoord.setPosition(lg, lt);			// 天文緯度経度を設定する.　厳密には測地緯度経度と同じ値を与えてはいけない.
	acoord.setLocation(lg, lt, sea);	// 測地緯度経度と海抜高度を設定する.
	print_location(acoord, sea);
	unsigned repeat = 0;
	for (AstroTime atime = acoord.getTime(); gRepeatCount--; atime.addSec(gRepeatStep)) {
		if (++repeat > 1) puts("---"); // 2回以後の繰り返しに対して分離マーカを出力する.
		acoord.setTime(atime);
		calc_main(acoord, pl);
	} // endfor gRepeatCount.
	return EXIT_SUCCESS;
}
// sunmoon.cpp - end.
