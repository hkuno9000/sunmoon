//. jday.cpp - �����E�X��(���)
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include <cstdlib>
#include "jday.h"
using namespace astro;
using namespace std;

//------------------------------------------------------------------------
//.----- class Jday : �����E�X��(���) -----------------------------------
//------------------------------------------------------------------------
const long BC0001_1_1 = 1721058L;	//  BC1.Jan.1

//------------------------------------------------------------------------
//. ���t�ݒ�

//. march_days - 3��1������̒ʓ�(3��1�� = 0)
static int
march_days(int& y, int m)
{
	while (m > 12)
		m -= 12, ++y;
	while (m < 3)
		m += 12, --y;		// 1,2���͑O�N��13,14���ƌ����Ă�
	return (m - 2) * 979 / 32 - 31;	// 979/32=30.593 ���ό�����
}

//. Jday::setGdate - �O���S���I��̓��t��ݒ肷�� (AD1582�`����p)
void
Jday::setGdate(int y, int m, int day)
{
	// 3��1������̒ʓ��� day �ɓ��� (3��1�� = 0)
	day += march_days(y, m);

	// ����ɔN�̓�����������(400�N��97��̉[�N)
	int q = y / 400;
	int r = y % 400;
	if (r < 0)
		r += 400, --q;
	d = y * 365L + q * 97L + r / 4 - r / 100 + day + 62 + BC0001_1_1;
}

//. Jday::setJdate - �����E�X��̓��t��ݒ肷�� (BC4713�`AD1582 �̊��ԗp)
void
Jday::setJdate(int y, int m, int day)
{
	// 3��1������̒ʓ��� day �ɓ��� (3��1�� = 0)
	day += march_days(y, m);

	// ����ɔN�̓�����������(4�N��1��̉[�N)
	int q = y / 4;
	if (y % 4 < 0)
		--q;
	d = y * 365L + q + day + 60 + BC0001_1_1;
}

//------------------------------------------------------------------------
//. ���t�擾(�O���S���I��A�����E�X��)

//. Jday::getGdate - �O���S���I��̓��t�𓾂� (AD1582�`����p)
void
Jday::getGdate(int& year, int& month, int& day, int& day_of_week) const
{
	long r, m, c4;

	// �O���S���I��ł� 400�N�ŉ[�N�̗�ꏄ����̂�
	// 400�N���̓����Ń����E�X��������
	r  = d - 62 - BC0001_1_1;   // 0�N3��1��(�O���S���I��)����̌o�ߓ���
	c4 = r / (365L * 400 + 97); // 400�N�P��
	r  = r % (365L * 400 + 97); // 400�N�P�ʓ��̓���
	if (r < 0)
		r += 365L * 400 + 97, --c4;

	// �[�N���l�����ĔN(�N��3��1��)�ƁA�N������̓������v�Z����
	int y = (int)(r / 365);	// 400�N�P�ʓ��̔N
	if (r < y * 365L + y / 4 - y / 100 + y / 400)
		--y;
	r -=    y * 365L + y / 4 - y / 100 + y / 400; // �N������̓���

	// �������肵�A�c�����Ƃ���
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

//. Jday::getJdate - �����E�X��̓��t�𓾂� (BC4713�`AD1582 �̊��ԗp)
void
Jday::getJdate(int& year, int& month, int& day, int& day_of_week) const
{
	long r, m, y4;

	// �����E�X��ł� 4�N�ŉ[�N�̗�ꏄ����̂�
	// 4�N�P�ʂŔN�����߂�
	r = d - 60 - BC0001_1_1; // 0�N3��1��(�����E�X��)����̌o�ߓ���
	y4 = r / (365L * 4 + 1); // 4�N�P��
	r  = r % (365L * 4 + 1); // 4�N�P�ʓ��̓���
	if (r < 0)
		r += (365L * 4 + 1), --y4;

	// �[�N���l�����ĔN(�N��3��1��)�ƁA�N������̓������v�Z����
	int y = (int)(r / 365);
	if (r < y * 365 + y / 4)
		--y;
	r -=    y * 365 + y / 4;

	// �������肵�A�c�����Ƃ���
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
//. �N�����Ɨj�� (�O���S���I��)
//	�v�Z���ʂ��L���b�V�����Ă����A�������t�Ȃ�Čv�Z���Ȃ����Ƃ�
//	���������͂����Ă���
THREAD static class {
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
{	// 0=���A1=���A2=�� ... 6=�y
	// �j���v�Z�́A�N�������v�Z���ăL���b�V��������
	// ���O�ł�������������Ȃ̂ŁA���̏�Ōv�Z����
	int w = (int)((d + 1) % 7); if (w < 0) w += 7;
	return w;
}

//------------------------------------------------------------------------
#ifdef TEST
#include <cstdio>
#include <cstdlib>
#include <cstring>
int main(int argc, char** argv)
{
	if (argc == 2 && strcmp(argv[1], "self") == 0) {
		// �\�[�X���̃e�X�g�p�^�[���Ŏ��ȃe�X�g����
		system("sed /^j/,/^g2000/!d jday.cpp >$in");
		system("jday <$in >$out");
		system("fc $in $out");
		return EXIT_SUCCESS;
	}
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
