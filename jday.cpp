//. jday.cpp - ユリウス日(暦日)
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include <cstdlib>
#include "jday.h"
using namespace std;
using namespace astro;
namespace astro {

//------------------------------------------------------------------------
//.----- class Jday : ユリウス日(暦日) -----------------------------------
//------------------------------------------------------------------------
const long BC0001_1_1 = 1721058L;	//  BC1.Jan.1

//------------------------------------------------------------------------
//. 日付設定

//. march_days - 3月1日からの通日(3月1日 = 0)
static int
march_days(int& y, int m)
{
	while (m > 12)
		m -= 12, ++y;
	while (m < 3)
		m += 12, --y;		// 1,2月は前年の13,14月と見立てる
	return (m - 2) * 979 / 32 - 31;	// 979/32=30.593 平均月日数
}

//. Jday::setGdate - グレゴリオ暦の日付を設定する (AD1582～現代用)
void
Jday::setGdate(int y, int m, int day)
{
	// 3月1日からの通日を day に得る (3月1日 = 0)
	day += march_days(y, m);

	// それに年の日数を加える(400年に97回の閏年)
	int q = y / 400;
	int r = y % 400;
	if (r < 0)
		r += 400, --q;
	d = y * 365L + q * 97L + r / 4 - r / 100 + day + 62 + BC0001_1_1;
}

//. Jday::setJdate - ユリウス暦の日付を設定する (BC4713～AD1582 の期間用)
void
Jday::setJdate(int y, int m, int day)
{
	// 3月1日からの通日を day に得る (3月1日 = 0)
	day += march_days(y, m);

	// それに年の日数を加える(4年に1回の閏年)
	int q = y / 4;
	if (y % 4 < 0)
		--q;
	d = y * 365L + q + day + 60 + BC0001_1_1;
}

//------------------------------------------------------------------------
//. 日付取得(グレゴリオ暦、ユリウス暦)

//. Jday::getGdate - グレゴリオ暦の日付を得る (AD1582～現代用)
void
Jday::getGdate(int& year, int& month, int& day, int& day_of_week) const
{
	long r, m, c4;

	// グレゴリオ暦では 400年で閏年の暦が一巡するので
	// 400年分の日数でユリウス日を割る
	r  = d - 62 - BC0001_1_1;   // 0年3月1日(グレゴリオ暦)からの経過日数
	c4 = r / (365L * 400 + 97); // 400年単位
	r  = r % (365L * 400 + 97); // 400年単位内の日数
	if (r < 0)
		r += 365L * 400 + 97, --c4;

	// 閏年を考慮して年(年初3月1日)と、年初からの日数を計算する
	int y = (int)(r / 365);	// 400年単位内の年
	if (r < y * 365L + y / 4 - y / 100 + y / 400)
		--y;
	r -=    y * 365L + y / 4 - y / 100 + y / 400; // 年初からの日数

	// 月を決定し、残りを日とする
	m = (r + 31) * 32 / 979 + 2;
	r -= (m - 2) * 979 / 32 - 31;
	if (m > 12)
		m -= 12, ++y;
	year  = (int)(c4 * 400 + y);
	month = (int) m;
	day   = (int) r;
	int w = (int)((d + 1) % 7); if (w < 0) w += 7;
	day_of_week = (int) w;
}

//. Jday::getJdate - ユリウス暦の日付を得る (BC4713～AD1582 の期間用)
void
Jday::getJdate(int& year, int& month, int& day, int& day_of_week) const
{
	long r, m, y4;

	// ユリウス暦では 4年で閏年の暦が一巡するので
	// 4年単位で年を求める
	r = d - 60 - BC0001_1_1; // 0年3月1日(ユリウス暦)からの経過日数
	y4 = r / (365L * 4 + 1); // 4年単位
	r  = r % (365L * 4 + 1); // 4年単位内の日数
	if (r < 0)
		r += (365L * 4 + 1), --y4;

	// 閏年を考慮して年(年初3月1日)と、年初からの日数を計算する
	int y = (int)(r / 365);
	if (r < y * 365 + y / 4)
		--y;
	r -=    y * 365 + y / 4;

	// 月を決定し、残りを日とする
	m = (r + 31) * 32 / 979 + 2;
	r -= (m - 2) * 979 / 32 - 31;
	if (m > 12)
		m -= 12, ++y;
	year  = (int) (y4 * 4 + y);
	month = (int) m;
	day   = (int) r;
	int w = (int)((d + 1) % 7); if (w < 0) w += 7;
	day_of_week = (int) w;
}

//------------------------------------------------------------------------
//. 年月日と曜日 (グレゴリオ暦)
//	計算結果をキャッシュしておき、同じ日付なら再計算を省くことで
//	高速化をはかっている
static class jday_cache {
	long jd;
	int y, m, d, w;
	void update(const Jday& j) {
		if (jd != j.jd()) {
			jd = j.jd(); j.getGdate(y, m, d, w);
		}
	}
public:
	int year(const Jday& j)  { update(j); return y; }
	int month(const Jday& j) { update(j); return m; }
	int day(const Jday& j)   { update(j); return d; }
} cache;

int
Jday::year() const
{
	return cache.year(*this);
}

int
Jday::month() const
{
	return cache.month(*this);
}

int
Jday::day() const
{
	return cache.day(*this);
}

int
Jday::dayOfWeek() const
{	// 0=日、1=月、2=火 ... 6=土
	// 曜日計算は、年月日を計算してキャッシュするより
	// 自前でやった方が高速なので、この場で計算する
	int w = (int)((d + 1) % 7); if (w < 0) w += 7;
	return w;
}

}//.endnamespace astro
//------------------------------------------------------------------------
#ifdef TEST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
using namespace astro;
int main(int argc, char** argv)
{
	static char* name[] = {"Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat", "?"};
	char buf[100];
	int y, m, d, gy, gm, gd, gw, jy, jm, jd, jw;
	char jg;
	Jday dd(0);
	fputs("jday test. input [gj]yyyy.mm.dd\n", stderr);
	while (gets(buf)) {
		if (sscanf(buf, " %c%d.%d.%d", &jg, &y, &m, &d) != 4)
			continue;
		if (jg == 'j') {
			dd.setJdate(y, m, d);
			dd.getGdate(gy, gm, gd, gw);
			dd.getJdate(jy, jm, jd, jw);
			printf("j%04d.%02d.%02d = jd:%8ld,"
				" g%04d.%02d.%02d.%s, j%04d.%02d.%02d.%s\n",
				y, m, d, dd.jd(),
				gy, gm, gd, name[gw], jy, jm, jd, name[jw]
			);
		} else {
			dd.setGdate(y, m, d);
			dd.getGdate(gy, gm, gd, gw);
			dd.getJdate(jy, jm, jd, jw);
			printf("g%04d.%02d.%02d = jd:%8ld,"
				" g%04d.%02d.%02d.%s, j%04d.%02d.%02d.%s\n",
				y, m, d, dd.jd(),
				gy, gm, gd, name[gw], jy, jm, jd, name[jw]
			);
		}
	}
	return EXIT_SUCCESS;
}
/* input sample && output
j-5000.01.01 = jd: -105192, g-5001.11.22.Fri, j-5000.01.01.Fri
j-4712.01.01 = jd:       0, g-4713.11.24.Mon, j-4712.01.01.Mon
j0000.01.01 = jd: 1721058, g-001.12.30.Thr, j0000.01.01.Thr
j0001.01.01 = jd: 1721424, g0000.12.30.Sat, j0001.01.01.Sat
j1582.10.05 = jd: 2299161, g1582.10.15.Fri, j1582.10.05.Fri
g1582.10.15 = jd: 2299161, g1582.10.15.Fri, j1582.10.05.Fri
g1858.11.17 = jd: 2400001, g1858.11.17.Wed, j1858.11.05.Wed
g1900.01.01 = jd: 2415021, g1900.01.01.Mon, j1899.12.20.Mon
g1970.01.01 = jd: 2440588, g1970.01.01.Thr, j1969.12.19.Thr
g2000.01.01 = jd: 2451545, g2000.01.01.Sat, j1999.12.19.Sat
*/
#endif
//. jday.cpp - end
