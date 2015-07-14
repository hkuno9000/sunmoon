/**@file sunmoon - ���z�A���̍��x�v�Z
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
	az.setNeg(); az.mod360();   // ���0���Ƃ����v����(��90���A�k180���A��270��)
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

/** -r: ��C���␳ON */
bool gAddRefraction = false;

/** -p: �S�f���̐Ԍo�Ԉܕ\��ON */
bool gPlanetRaDc = false;

/** table: �o�v�\����. */
unsigned gTableDays = 0;

//------------------------------------------------------------------------
void print_planet(const AstroCoordinate& acoord, const Planets& pl, const char* name, int id)
{
	char azalt[256];
	char radec[256];
	char j2000[256];

	Vec3 v = pl.vecQ(id);	// �n�S���ϐԓ����W(MOON�̂ݑ��S)
	acoord.conv_q2tq(v);	// ���ψʒu���^�ʒu(�͓��␳).
	Vec3 q = v;
	acoord.conv_q2h(v);		// �ԓ����W���n�����W.
	if (gAddRefraction) acoord.addRefraction(v);	// ��C���␳.

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
	AstroCoordinate acoord;	// ���ݎ����ŏ�����.
	Planets pl;

	//--- �R�}���h���C������͂���.
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
			//--- ���Ԑݒ�.
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

	//--- �v�Z���s.
	acoord.setPosition(lg, lt);			// �V���ܓx�o�x��ݒ肷��.�@�����ɂ͑��n�ܓx�o�x�Ɠ����l��^���Ă͂����Ȃ�.
	acoord.setLocation(lg, lt, sea);	// ���n�ܓx�o�x�ƊC�����x��ݒ肷��.
	acoord.beginConvert();
	pl.calc(acoord);
	Vec3 sun  = pl.vecQ(Planets::SUN);
	Vec3 moon = pl.vecQ(Planets::MOON);
	double cosSun = sun.inner(moon);	// sun/moon�͕����]���Ȃ̂ŁA���̓��ς͈ʑ��p��cos�ł���.
	if (cosSun > 1) cosSun = 1;			// acos()�ł�DOMAIN�G���[���.
	if (cosSun < -1) cosSun = -1;		// acos()�ł�DOMAIN�G���[���.
	Degree phase; phase.setArcCos(cosSun);	// acos �� 0..180�x�͈̔͂Œl��Ԃ�.
	if (sun.x * moon.y - sun.y * moon.x < 0) { // XY���ʂ̊O�ϒl�����̒l�Ȃ�A�ʑ��p�x�� 180�`360�x�͈̔͂ɕ␳����.
		phase.setNeg(); phase.mod360();
	}
	acoord.conv_q2tq(sun);
	acoord.conv_q2tq(moon);
	acoord.conv_q2h(sun);
	acoord.conv_q2h(moon);
	if (gAddRefraction) { // ��C���␳.
		acoord.addRefraction(sun);
		acoord.addRefraction(moon);
	}

	//--- ���ʕ\��.
	print(acoord, sea, sun, moon, phase.degree());

	//--- �S�f���̐Ԍo�Ԉܕ\��.
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

	//--- �o�v�v�Z.
	if (gTableDays != 0) {
		AstroTime t = acoord.getTime();
		const double jd_end = t.jd() + gTableDays;
		const double sun_rz  = sin(dms2rad(0,0,960));	// ���z�����a�ɂ��o�v�␳. �����a�� 960" �Ō��ߑł�.
		const double min30_z = sin(hms2rad(0,30,0));	// ���p30���̍��x��z���W�l.
		const double min3_z  = sin(hms2rad(0,3,0));	// ���p1���̍��x��z���W�l.
		const double sec15_z = sin(hms2rad(0,0,15));	// ���p15�b�̍��x��z���W�l.
		int step = -1;	// ����͎w�莞����1�b�O�̍��x���v�Z����.
		for (t.addSec(step); t.jd() < jd_end; t.addSec(step)) {
			// �O�񎞍��̍��x��ۑ�����. �������A����͂��̒l���g���Ă͂����Ȃ�.
			const Vec3 sun0 = sun;
			const Vec3 moon0 = moon;
			// ���񎞍��̍��x���v�Z����.
			acoord.setTime(t);
			acoord.beginConvert();
			pl.calc(acoord);
			sun  = pl.vecQ(Planets::SUN);
			moon = pl.vecQ(Planets::MOON);
			acoord.conv_q2tq(sun);
			acoord.conv_q2tq(moon);
			acoord.conv_q2h(sun);
			acoord.conv_q2h(moon);
			// ��C���␳�͏펞���{����.
			acoord.addRefraction(sun);
			acoord.addRefraction(moon);
			// ���z�����a�������x�␳����.
			sun.z += sun_rz;
			if (step > 0) {
				// �O�񎞍��̍��x�Ɣ�r���A���E�l���ׂ����������o�v�����Ƃ��ĕ\������.
				if (sun0.z < 0 && sun.z >= 0) print_table("SUN-RISE", t);
				if (sun0.z >= 0 && sun.z < 0) print_table("SUN-SET",  t);
				if (moon0.z < 0 && moon.z >= 0) print_table("MOON-RISE", t);
				if (moon0.z >= 0 && moon.z < 0) print_table("MOON-SET",  t);
				// �O�񎞍��̓����Ɣ�r���A�q�ߐ����ׂ���������쒆�����Ƃ��ĕ\������.
				if (sun0.y >= 0 && sun.y < 0) print_table("SUN-CULM",  t);
				if (moon0.y >= 0 && moon.y < 0) print_table("MOON-CULM",  t);
			}
			double z = min_value(fabs(sun.z), fabs(moon.z));
			double y = min_value(fabs(sun.y), fabs(moon.y));
			z = min_value(z, y);	// �n�����ʉ߁A�q�ߐ��ʉߕt�߂̍ŏ����W�l�����߂�.
			if (z >= min30_z)
				step = 20*60; // ���x���}���p30���ȏ�Ȃ�20���P�ʂŎ�����i�߂�.
			else if (z >= min3_z)
				step = 2*60; // ���x���}���p3���ȏ�Ȃ�2���P�ʂŎ�����i�߂�.
			else if (z >= sec15_z)
				step = 10; // ���x���}���p15�b�ȏ�Ȃ�10�b�P�ʂŎ�����i�߂�.
			else
				step = 1; // 1�b�P�ʂŎ�����i�߂�.
		}
	}
	return EXIT_SUCCESS;
}
// sunmoon.cpp - end.
