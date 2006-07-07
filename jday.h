//. jday.h - �����E�X��(���)
//	Copyright (C) 1997,1998, 2000 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef JDAY_H_
#define JDAY_H_
#include "defs.h"
namespace astro {
using namespace std;

//------------------------------------------------------------------------
//.----- class Jday : �����E�X��(���) -----------------------------------
//------------------------------------------------------------------------
// �����E�X���N���X
//	�����E�X��A�O���S���I��ɑΉ����Ă���
//@	�A���ߋC�ɂ��Ă͔h���N���X�őΉ���\��
//
// �V���w�I�ɂ́A�����E�X���⏀�����E�X���͐��E��(�o�x0���ɂ����鎞��)
// �ň������̂ł���B�܂�������̎����������Ƃ��Ċ܂߂Ĉ������̂ł���B
// �������Ȃ���A���̂悤�ȓV���w��̒�`�������ɂ��Ă͂߂Čv�Z���s��
// �ƁA��������t�ύX�����l�����Ȃ���΂Ȃ炸�g�����肪�����̂ŁA
// �{�N���X�ň��������E�X���́A���̒P�ʂň����A�����n�����E���E�n����
// ���킸�A�Ƃ��鎞���n�ɂ�����o�ߓ����𑪂邽�߂̂��̂Ƃ����B
// �V���w�I�Ȍv�Z���s���ꍇ�ɂ́A���E�����Z�⏬�����̕t��������K�v������B
//
//	�v�Z�\�͈͂� �}INT_MAX�N�ł��邪�A���疜�N�𒴂���͈͂��v�Z���Ă�
//	�����I�ȈӖ��͂��܂�Ȃ��B�Ȃ��Ȃ�n���̎��]���x�͂킸���ł��邪
//	�N�X�x���Ȃ��Ă��邽�߂P�N�̓����͗y���Ȗ����ł͏��Ȃ��Ȃ�A�y��
//	�ȉߋ��ł͑����Ȃ�B����Č���̂P�N�̓��������Ƃɂ������K�p
//	�ł��Ȃ�����ł���B
/*
----- �m�[�g -------------------------------------------------------------
�O���S���I��(Gregorius calendar)�F
	���s�̑��z��B400�N�Ԃ�97��̉[�N������B
	4�Ŋ���؂�A100�Ŋ���؂�Ȃ��N���[�N�Ƃ��A2��29����݂���B
	400�Ŋ���؂��N���[�N�Ƃ���B
	���[�}���c�O���S���E�X13�����A�����E�X��1582�N10��4���̗������A
	�O���S���I��1582�N10��15���Ƃ��Ď��{�����B
	���{�ł́A���A���z��̖���5�N12��2���̗����𖾎�6(1873)�N1��1��
	�Ƃ��Ď��{�����B

�����E�X��(Julian calendar)�F
	4�Ŋ���؂��N���[�N�Ƃ����@�B
	���[�}�̃W�����A�X�E�V�[�U�[�����肵�ABC 46 ������{�����B

�����E�X��(Julian day)�F
	�����E�X�� BC 4713(-4712)�N1��1������(���E��)����N�Z�����ʓ��B
	1582�N�ɃO���S���I��։����ۂɓ������ꂽ�B�N��w�ɂ����āA
	�e��̗�Ɉˑ����Ȃ��œ��t�𓝈�ł��ĕ֗��ł���B
	���̎n�܂肪���E���̐���(���{���Ԃ�21��)�ł��邱�Ƃɒ��ӂ��邱�ƁB
	���߈ȑO�̓}�C�i�X���ԂŌv�Z����̂��֗��ł���B
	��F
		j-5000.1.1  12UT = -105192 Fri (BC5001)
		j-4712.1.1  12UT =       0 Mon (BC4713)
		j0000.1.1   12UT = 1721058 Thr (BC0001)
		j0001.1.1   12UT = 1721424 Sat
		j1582.10.5  12UT = 2299161 Fri
		g1582.10.15 12UT = 2299161 Fri
		g1858.11.17 12UT = 2400001 Wed
		g1900.1.1   12UT = 2415021 Mon
		g1970.1.1   12UT = 2440588 Thr
		g2000.1.1   12UT = 2451545 Sat

�������E�X��(Modified Julian day)�F
	1858�N11��17��0��(���E��)����N�Z�����ʓ��B
	1973�N�ɍ��ۓV���w�A���ō̑����ꂽ�B
	����ł̓����E�X�����V���ɒB���Ă��茅���傫���Čv�Z�ɕs�ւȂ̂ŁA
	2,400,000.5 �������āA�T���ɔ[�܂�A�Ȃ������E���ߑO�O���œ���
	�ς��悤�ɂ������̂ł���B
--------------------------------------------------------------------------
*/
class Jday {
	long d;			// �����E�X��

public:
	//----- ��ƂȂ���t�̃����E�X�� -------------------------------
	enum {
		AD1900_1_1_12UT = 2415021L,
		AD2000_1_1_12UT = 2451545L,	// �V�̗�̋N�Z���AJ2000.0
	};

	//----- �R���X�g���N�^ -------------------------------------------
	Jday(int year, int month, int day=1) {	// ���t�͐��E�����߂̂���
		setGdate(year, month, day);
	}

	explicit Jday(long n)
		: d(n) {}

	Jday(const Jday& a)
		: d(a.d) {}

	Jday& operator=(const Jday& a) {
		if (this != &a)
			d = a.d;
		return *this;
	}

	//----- ���̉����Z -----------------------------------------------
	Jday& operator+=(long n) {
		d += n; return *this;
	}
	Jday& operator-=(long n) {
		d -= n; return *this;
	}
	Jday& operator++() {
		++d; return *this;
	}
	Jday& operator--() {
		--d; return *this;
	}
	friend long operator-(const Jday& a, const Jday& b) {
		return a.d - b.d; // ���̓�����Ԃ�
	}

	//----- �֌W���Z -------------------------------------------------
	bool operator==(const Jday& a) const { return d == a.d; }
	bool operator!=(const Jday& a) const { return d != a.d; }
	bool operator< (const Jday& a) const { return d <  a.d; }
	bool operator<=(const Jday& a) const { return d <= a.d; }
	bool operator> (const Jday& a) const { return d >  a.d; }
	bool operator>=(const Jday& a) const { return d >= a.d; }

	//----- �N�����Ɨj�� (�O���S���I��) ------------------------------
	int year() const;
	int month() const;	// 1�`12
	int day() const;	// 1�`31
	int dayOfWeek() const;	// 0=���A1=���A2=�� ... 6=�y

	//----- ���� -----------------------------------------------------
	long jd() const {	// �����E�X��
		return d;
	}
	long mjd() const {	// �������E�X��
		return d - 2400000L;
	}

	//----- J2000.0 --------------------------------------------------
	// �O���S���I��̓���[���E��]���� J2000.0 (2000�N1��1��12:00UT)�������Ƃ���
    // �o�ߓ����A�����E�X�N���A�����E�X���I�����߂�
    // @param y, m, dd �O���S���I��̔N����
    // @return J2000.0 �������Ƃ���o�ߎ���
	static double jd2000(int y, int m, double dd) {	// ����
		Jday w(y, m, 0);
		return w.jd() - AD2000_1_1_12UT + dd - 0.5;	// ���߂����̕ς��ڂȂ̂� 0.5���␳����
	}
	static double jy2000(int y, int m, double dd) {	// �����E�X�N��
		return jd2000(y, m, dd) / 365.25;
	}
	static double jc2000(int y, int m, double dd) {	// �����E�X���I��
		return jd2000(y, m, dd) / 36525;
	}

	//----- ���t�ݒ�E�擾(�O���S���I��) -----------------------------
	void getGdate(int& year, int& month, int& day, int& dayOfWeek) const;
	void setGdate(int year, int month, int day);

	//----- ���t�ݒ�E�擾(�����E�X��) -------------------------------
	void getJdate(int& year, int& month, int& day, int& dayOfWeek) const;
	void setJdate(int year, int month, int day);
};

}//endnamesapae astro
#endif // JDAY_H_
