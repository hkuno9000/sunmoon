/**@file sunmoon - 太陽、月の高度計算
 * @copyright 2006 hkuno@willsoft.co.jp
 * $Id: sunmoon.cpp,v 1.2 2006-07-13 08:30:18 hkuno Exp $
 */
#include <stdio.h>
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

//------------------------------------------------------------------------
void print(const AstroCoordinate& acoord, double sea, const Vec3& sunH, const Vec3& moonH)
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

	sunH.getLtLg(alt, az);
	printf("SUN-ALT: %+05.2fd\n", alt.degree());

	moonH.getLtLg(alt, az);
	printf("MOON-ALT: %+05.2fd\n", alt.degree());
}

//------------------------------------------------------------------------
const char gUsage[] =
	"usage: sunmoon [-r] lt=<LT> lg=<LG> [sea=<SEA>] [utc=<UTC>]\n"
	" $Revision: 1.2 $\n"
	"   -r  : add refraction to alt\n"
	"   LT  : latidute  '35d39m16s'\n"
	"   LG  : longitude '139d44m40s'\n"
	"   SEA : sea level altitude[m]. default is 0\n"
	"   UTC : ISO 8601 time format '2006-12-31T23:59:59'. default is current time\n"
	;

/** -r: 大気差補正ON */
bool gAddRefraction = false;

//------------------------------------------------------------------------
int main(int argc, char** argv)
{
	// tokyo
	Degree lt(35, 39, 16);
	Degree lg(139, 44, 40);
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
	acoord.setPosition(lg, lt);
	acoord.setLocation(lg, lt, sea);
	acoord.beginConvert();
	pl.calc(acoord);
	Vec3 sun  = pl.vecQ(Planets::SUN);
	Vec3 moon = pl.vecQ(Planets::MOON);
	acoord.conv_q2h(sun);
	acoord.conv_q2h(moon);
	if (gAddRefraction) { // 大気差補正.
		acoord.addRefraction(sun);
		acoord.addRefraction(moon);
	}

	//--- 結果表示.
	print(acoord, sea, sun, moon);
	return EXIT_SUCCESS;
}
// sunmoon.cpp - end.
