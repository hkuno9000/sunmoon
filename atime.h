//. atime.h - �V������
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef ATIME_H_
#define ATIME_H_
#include "jday.h"

namespace astro {

//------------------------------------------------------------------------
//.----- class AstroTime : �V������ --------------------------------------
//------------------------------------------------------------------------
// �����E�X���A���̕b����ێ����A�K�v�ɉ����čP���������v�Z����
// �������E�X���͐��E��12���ɓ��t���ς�B
// ���O���j�W��V��(���ϑ��z)
//	���z�͒n���̌��]�^���ɂ��V���̉��������N�łقڈ������B
//	�V���͒n���̎��]�^���ɂ�����łقڈ��]����B
//	���̏o�E���̓���𐶊����Ԃ̊�Ƃ��Ă����X�́A�V�����
//	���z�����]����̂ɗv���鎞�Ԃ�����Ƃ��Ă��邪�A���z��
//	�������i��ł��邽�߁A�Ԍo�����ł݂�Ɛԓ��ƌ����t���H��
//	�t�߂ł͐i�݂��x���A�Ď��~���t�߂ł͐i�݂������Ȃ�B�܂�
//	�G�߂ɂ���Ĉ���̒���������Ă��܂��B����ł͕s�ւȂ̂ŁA
//	�ԓ������葬�x�ň�N������鉼�z�̑��z���A�����̊�Ƃ���B
//	���̉��z���z�̐Ԍo���u���ϑ��z�v�ł���B
// �����E��(UT0,UT1,UT2)
//	���E���͓V�̊ϑ������ɂ��Č��肳���B
//	�V�̈ʒu�ϑ����狁�߂��n�����ύP����(LMST)�ƁA�ϑ��n�̗̍p
//	�o�x�ɂ���UT0 �𓾂�B
//	���ɋɉ^���ɂ��ϑ��n�̌o�x�ω�(�ɉ^���ɂ�����)�␳����
//	�������āA�ϑ��n�ɂ��Ȃ����E���ʂ� UT1 �𓾂�B
//	�ɉ^���͎��]�������a10m���炢�͈̔͂ŕϓ����錻�ۂł���A
//	�p�x�ɂ���Ɓ}0.3��(���p��0.02s)���炢�̕ω��ƂȂ�B
//	����ɒn���̎��]���x�ω��̋G�ߕϓ��i�����͂ɂ��n���̕ό`��A
//	�ΐ����̋G�ߕω��������Ƃ����j��Ts�������āA�b�̐i�݂𕽊�
//	������ UT2 �𓾂�B1960�N��UTC���o�ꂷ��܂ł́A����UT2������
//	�ɗp���鎞�ԂƂ��ė��p����Ă����B
//	�� = �ϑ��n�̗̍p�V���ܓx
//	�� = �ϑ��n�̗̍p�V���ܓx
//	 t = �N������̌o�ߎ���[�N�P��]
//	 x,y = �ɉ^���ɂ��ɂ̈ʒu
//	���� = -(x sin�� + y cos��) tan��  (���悻 �}0.3��tan��)
//	��Ts = 0.022��ain2��t - 0.012��cos2��t - 0.006��sin4��t + 0.007��cos4��t
//	��m = �O���j�W��V��(���ϑ��z)
//	UT0 = LMST - �� - ��m + 12h
//	UT1 = UT0 + ����
//	UT2 = UT1 + ��Ts
// �����萢�E��(UTC)
//	���ی��q��(TAI)�ɉ[�b�����A�n���̎��]�Ɋ�Â����E��(UT1)�Ƃ�
//	�������� �}0.9�b�ȓ��ɊǗ��������Ԃł���B��X������g���Ă���
//	���Ԃł���B
//	�����ŏI�b�ɉ[�b(�P�b)��ǉ����邩�������ƂŁA���������}0.9�b
//	�ȓ���������B���̎����́A���ےn����]�ϑ�����(IERS)�̒����ǂ�
//	���肷��B
//	1976�`1997��20�N�ԂŁA���v 15 �b�̉[�b���}������Ă���B
//	1997�N7��1������1999�N1��1���܂ł� UTC - TAI = -31�b�ł���B
// �����ےn����]�ϑ�����(IERS:International Earch Rotation Service)
//	���E�e�n�̓V����ɂ����āAUTC����Ƃ��čP���̈ʒu�ϑ����s��
//	UT0 - UTC �����߂�B�����IERS������(�t�����X)���������āA
//	�Ɉʒu(x,y)�AUT1 - UTC�AUTC - TAI�̊m��l�A����l�A�\���l��
//	���\���Ă���BWWW��ł� http://hpiers.obspm.fr/ ������\�l��
//	����ł���B
// ��DUT1
//	�e���̕W�����d�g��(���{�ł�JJY)���񂶂� UT1 - UTC �̗\���l
//	(0.1�b�P��)�̂��ƁBUTC����UT1�̗\���l�����߂邽�߂Ɏg�p����B
// ���O���j�b�W���ύP����(GMST)
//	GMST��UT1�̊֌W�͈ȉ��̎��Œ�߂��Ă���B
//	�o�T�F���ȔN�\ ���E���̍����
//	GMST = UT1 - 12h + ��m
//	��m  = 18h 41m 50.54841s
//		+ 8640184.812866s Tu + 0.093104s Tu^2 - 0.0000062s Tu^3
//	Tu = 2000�N1��1��12��(UT1)���烆���E�X���I�P�ʂő��������E��
//	���{�W����JST(UTC + 9h)����A���x�̍���UT1�𓾂�ɂ�JJY����
//	�����񎞂�IERS����DUT1(0.1�b�P��)�𓾂�K�v������B
// ���͊w��(TD:Dynamical Time)
//	���E��(UT)�͗\���s�\�ȉ[�b�̒��������邽�߈�l�Ȏ����n�ł͂Ȃ��A
//	�f���̋O���v�Z���̓V�̗͊w�ŗp����ɂ͓s���������B
//	�����Ŏ��Ԍo�߂���l�Ȏ����n�Ƃ��ėp�ӂ��ꂽ�̂��͊w���ł���B
//	��ʑ��ΐ����_����A��Ԉʒu�ɂ���Ď����̐i�݂͋ώ��łȂ����߁A
//	���z�n�d�S����Ƃ����f���̈ʒu�v�Z�ɂ́A
//	�u���z�n�͊w��(TDB:Barycentric Dynamical Time)�v��p���A
//	�n�S���ʒu�̌v�Z�ɂ́A
//	�u�n���͊w��(TDT:Terrestrial Dynamical Time)�v��p����B
//	TDT�͍��ی��q��(TAI)�ɑ΂��āATDT = TAI + 32.184�b �ƒ�߂���
//	����BTDT��TDB�̍��� �}0.002�b�ȓ��ł���B

class AstroTime {
	Jday d;		// �����E�X�� (���E�����߂̃����E�X��)
	double s;	// �����E�X���̕b��(0�`86400.0) �����E�����߂�0
	double dut1;	// UT1 - UTC �̒l�B0.1�b�̐��x�B
	int leapSec;	// �[�b�ATAI - UTC �̒l�B1997.7�`1999.1�܂ł� +31�b

protected:
	//----- �b�������z���Ȃ��悤�ɂ��� -------------------------------
	void ajust();

	//----- �e�����n�̓��̕b�� ---------------------------------------
	// UT1[�b]         �����߂�0�Ƃ���
	double ut1() const	{ return s + dut1; }

	// ���ی��q��[�b]  �����߂�0�Ƃ���
	double tai() const	{ return s + leapSec; }

	// �n���͊w��[�b]  �����߂�0�Ƃ���
	double tdt() const	{ return tai() + 32.184; }

public:
	//----- �R���X�g���N�^ -------------------------------------------
	// ��dut1��0.0�AleapSec��initLeapSec�ɂ���B
	//   �����Ȍv�Z�̂��߂ɂ́AsetDUT1(),setLeapSec()�Őݒ肷�邱�ƁB

	// �[�b�̏����l
	static int initLeapSec;

	// ���ݎ����Ő�������
	AstroTime();

	// �w��̓��t�Ǝ����Ő�������
	// @param jd  ���E�����߂̓��t
	// @param utc ���E���O������̌o�ߕb��
	explicit AstroTime(const Jday& jday, double utc = 0);


	//----- ���ݎ����ɍX�V���� ---------------------------------------
	// ��dut1��leapSec�͕ύX���Ȃ�
	//   �����Ȍv�Z�̂��߂ɂ́AsetDUT1(),setLeapSec()�Őݒ肷�邱�ƁB
	void update();           // �b�P�ʂ̍X�V
	void updateSystemTime(); // �~���b�P�ʂ̍X�V


	//----- �����̐ݒ�E�擾 -----------------------------------------
	// ��dut1��leapSec�͕ύX���Ȃ�
	//   �����Ȍv�Z�̂��߂ɂ́AsetDUT1(),setLeapSec()�Őݒ肷�邱�ƁB
	void set(const Jday& jday, double utc = 0) {
		d = jday; s = utc - 12*3600L; ajust();
	}

	void get(Jday& jday, double& utc) const {
		jday = d; utc = s + 12*3600L;
		if (utc >= 86400)
			++jday, utc -= 86400;
	}

	void get(int& year, int& month, int& day, double& utc) const {
		Jday jday(0);
		int junk;
		get(jday, utc);
		jday.getGdate(year, month, day, junk);
	}


	//----- ���萢�E������UT1�ւ̗\���␳�l�ݒ� ----------------------
	void setDUT1(double _dut1) {
		dut1 = _dut1; // UT1 - UTC �̒l [�b]
	}


	//----- ���萢�E�����獑�ی��q���ւ̕ϊ��l(�[�b)�ݒ� -------------
	void setLeapSec(int _leapSec) {
		leapSec = _leapSec; // �[�b�ATAI - UTC �̒l [�b]
	}


	//----- ���ƕb�̉����Z -------------------------------------------
	void addSec(double n) { s += n; ajust(); }
	void addDay(long n)   { d += n; ajust(); }
	void subSec(double n) { s -= n; ajust(); }
	void subDay(long n)   { d -= n; ajust(); }

	//----- �֌W���Z -------------------------------------------------
	bool operator==(const AstroTime& a) const { return d == a.d && s == a.s; }
	bool operator!=(const AstroTime& a) const { return d != a.d || s != a.s; }
	bool operator< (const AstroTime& a) const { return d < a.d || (d == a.d && s <  a.s); }
	bool operator<=(const AstroTime& a) const { return d < a.d || (d == a.d && s <= a.s); }
	bool operator> (const AstroTime& a) const { return d > a.d || (d == a.d && s >  a.s); }
	bool operator>=(const AstroTime& a) const { return d > a.d || (d == a.d && s >= a.s); }


	//----- ���̏����t���̃����E�X�� ---------------------------------

	// �����E�X��[UTC]
	double jd() const {
		return d.jd() + s / 86400L;
	}

	// �����E�X��[UT1]
	double jd1() const {
		return d.jd() + ut1() / 86400L;
	}

	// J2000.0(�͊w��2000�N1��1��12��)����̌o�ߓ���[TDT]
	// ��TDT��TDB�̈Ⴂ(�}0.002�b)�����ɂȂ�Ȃ��悤�Ȑ��x�̌v�Z
	//   �ł���΁A���z�n�͊w��(TDB)�ɂ��o�ߓ����Ƃ��ėp���ėǂ��B
	double j2000() const {
		return d.jd() - Jday::AD2000_1_1_12UT + tdt() / 86400L;
	}


	//----- �P���� ---------------------------------------------------
	// �O���j�W���ύP����[�b] [0�`86400)
	double gmst() const;

};//endclass AstroTime

}//endnamespace astro
#endif // ATIME_H_
