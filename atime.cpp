//. atime.cpp - �V������
//	Copyright (C) 1997,1998,2014 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include <ctime>
#include "jday.h"
#include "degree.h"	//!@bug VC6 bug? ���O��"namespace astro {}"��u���� degree.h�� ostream�Q�ƂŃR���p�C���G���[�ƂȂ�.
#include "atime.h"
#if defined(WIN32)
#include <windows.h>
#elif defined(BSD) || defined(__APPLE__) || defined(__CYGWIN__)
#include <sys/time.h>
#endif
using namespace std;
using namespace util;
using namespace astro;
namespace astro {

//------------------------------------------------------------------------
//.----- class AstroTime : �V������ --------------------------------------
//------------------------------------------------------------------------

//----- �R���X�g���N�^ ---------------------------------------------------
// dut1��leapSec�͓K���ȏ����l�ɂ���B
// �����Ȍv�Z�̂��߂ɂ́AsetDUT1(),setLeapSec()�Őݒ肷�邱�ƁB

// �[�b�̏����l [TAI-UTC] http://hpiers.obspm.fr/iers/bul/bulc/bulletinc.dat
int AstroTime::initLeapSec = 35; // June 2012.

// ���ݎ����Ő�������
AstroTime::AstroTime()
	: d(0), s(0), dut1(0), leapSec(initLeapSec)
{
	update();
}

// �w��̓��t�Ǝ����Ő�������
// @param jd  ���E�����߂̓��t
// @param utc ���E���O������̌o�ߕb��
AstroTime::AstroTime(const Jday& jday, double utc)
	: d(jday), s(utc - 12*3600L), dut1(0), leapSec(initLeapSec)
{
	adjust();
}


//----- ���t�Ǝ����̒��� -------------------------------------------------
//. AstroTime::adjust - �b�������z���Ȃ��悤�ɂ���
void
AstroTime::adjust()
{
	while (s >= 86400.0)
		d += 1, s -= 86400.0;
	while (s < 0)
		d -= 1, s += 86400.0;
}

//----- ���ݎ����ɍX�V���� -----------------------------------------------
//. AstroTime::update - ���ݎ���(�b�P��)�ɍX�V����
void
AstroTime::update()
{
	time_t tt = time(0);
	struct tm& t = *gmtime(&tt);
	d.setGdate(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	s = hms2hs(t.tm_hour, t.tm_min, t.tm_sec)
		- 12 * 3600.0; // ���E�����߂�0�ɂȂ�悤�ɕ␳����
	adjust();
}

//. AstroTime::updateSystemTime - ���ݎ���(�~���b�P��)�ɍX�V����
void
AstroTime::updateSystemTime()
{
#if defined(WIN32)
	::SYSTEMTIME t;
	::GetSystemTime(&t); // ���E���œ��t�����𓾂�
	d.setGdate(t.wYear, t.wMonth, t.wDay);
	s = hms2hs(t.wHour, t.wMinute, t.wSecond + t.wMilliseconds / 1000.0)
		- 12 * 3600.0; // ���E�����߂�0�ɂȂ�悤�ɕ␳����
	adjust();
#elif defined(BSD) || defined(__APPLE__) || defined(__CYGWIN__)
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

//----- �P���� -----------------------------------------------------------
//. AstroTime::gmst - �O���j�W���ύP����[�b] [0�`86400)
double
AstroTime::gmst() const
{
	// ���ȔN�\ ���E���̍����
	// GMST = UT1 - 12h + ��m
	// ��m  = 18h 41m 50.54841s
	//      + 8640184.812866s Tu + 0.093104s Tu^2 - 0.0000062s Tu^3
	// Tu = 2000�N1��1��12��(UT1)���烆���E�X���I�P�ʂő��������E��
	// UT1 = UTC + DUT1 (���E��)
	// ����m�̓O���j�W��V��(���ϑ��z)
	// ���̎��łق� 0.001�b�̐��x������
	// ������ DUT1�̐��x�� 0.1�b�ł��邽�߁A������ 0.1�b�̐��x�ƂȂ�
	// �����DUT1�����ݒ�Ȃ� UT1��UTC�̍ő卷�ł���}0.9�b�����x�ƂȂ�
	const double Tu = (jd1() - Jday::AD2000_1_1_12UT) / 36525U;	// �����E�X���I
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
using namespace std;
using namespace util;
using namespace astro;
int main(int argc, char** argv)
{
	if (argc == 2 && strcmp(argv[1], "update") == 0) {
		// ���ݎ��ԍX�V.
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
	while (gets(buf)) {
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
