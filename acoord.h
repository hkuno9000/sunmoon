//. acoord.h - �V�����W�n
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef ACOORD_H_
#define ACOORD_H_
#include "degree.h"
#include "atime.h"
#include "vec3.h"

namespace astro {
using namespace std;
using namespace util;

//------------------------------------------------------------------------
//.----- class AstroCoordinate : �V�����W�n ------------------------------
//------------------------------------------------------------------------
// �V�����W�n�N���X
class AstroCoordinate {
	double lastT;		// calc()�Ŏg�p����L���b�V��
	bool recalcMat;		// calcMat()�̍Čv�Z�t���O
	bool recalcMat2;	// calcMat2()�̍Čv�Z�t���O
protected:
	// �V���o�ܓx(�ɉ^�����܂߂��u���̓V���o�ܓx)
	Degree lt;		// �V���ܓx [��]
	Degree lg;		// �V���o�x [��E]

	// �ϑ��ʒu
	Vec3 location;		// �n���d�S�����_�Ƃ����������W [m]

	// �V������
	AstroTime atime;	// �����E�X���A�b
	double m_gmst;		// �O���j�W���ύP����[�b]
	double Eq;		// ���_��(Equation of Equionxes) [�b]

	// ��ʍ΍�(general precession J2000.0)�̊p�x[degree]
	Degree za;		// ��A
	Degree Za;		// ��A
	Degree ta;		// ��A
    Degree Pa;		// ��A

	// �͓�(nutation J2000.0)�̊p�x[rad]
	double moe;		// ���ω����X�p  (��A:mean obliquity of the ecliptic) [rad]
	double nlg;		// ���o�͓̏�    (����:nutation in longitude) [rad]
	double nob;		// �����X�p�͓̏�(����:nutation in obliquity) [rad]

	// ���z�̕��ω��o�A�n�S����
	Degree Ls;		// �u���̕��Ϗt���_�ɂ�鉩�o
	double Rs;		// �n�S����[AU]

    // ���z�̒n�S�􉽊w�ԓ����W[AU] �����s���܂܂�
    Vec3 sunJ;		// J2000.0�̐ԓ����W[AU]
    Vec3 sunQ;		// �u���̕��ϐԓ����W[AU]

	// ���ԓ��^�n�����W�ϊ��s��
	Mat3x3 q2h;		// �ԓ����n��
	Mat3x3 h2q;		// �n�����ԓ�

	// ���ϐԓ��^�������W�ϊ��s��
	Mat3x3 c2q;		// ���ω���  �� ���ϐԓ�
	Mat3x3 q2c;		// ���ω���  �� ���ϐԓ�

	// �^�ԓ��^�������W�ϊ��s��
	Mat3x3 tc2tq;	// �^����  �� �^�ԓ�
	Mat3x3 tq2tc;   // �^����  �� �^�ԓ�


	// �΍��i50.3���^�N�j�␳�s��
	Mat3x3 j2q;		// J2000.0�ԓ� �� ���ϐԓ�
	Mat3x3 q2j;		// J2000.0�ԓ� �� ���ϐԓ�

	// �͓��i�}9���j�␳�s��
	Mat3x3 q2tq;		// ���ϐԓ�  �� �^�ԓ�
	Mat3x3 tq2q;		// ���ϐԓ�  �� �^�ԓ�

	// �N������(annual parallax) --- 1���ȉ��Ȃ̂Ŗ���

	// �N�����s��(annual aberration) �␳�x�N�g�� --- �ő�20��
	Vec3 k_abr;		// ���s���萔 * �n�����]�^���̕����]��


	// ���ύP�����E�������ω��o�E�΍��E�͓����v�Z����
	// @param T J2000.0����̃����E�X���I[TDT]
	void calc();		// �v�Z���C��
	void calc2(double T);	// �΍��E�͓��̌v�Z

	// ���W�ϊ��s����v�Z����
	void calcMat();		// ���ԓ��^�n�����W�ϊ��s��̌v�Z
	void calcMat2();	// �΍��E�͓��E�ԓ��^�������W�ϊ��s��̌v�Z

public:
	//----- �R���X�g���N�^ -------------------------------------------
	AstroCoordinate() {
		lastT = 0; calc();
	}


	//----- �V���o�ܓx�̐ݒ�Ǝ擾 -----------------------------------

	// �V���o�ܓx��ݒ肷��
	// ���n���P�����⍂�x���ʕϊ��Ɏg�p����̂ŁA�ɉ^�����܂߂��u����
	// �V���o�ܓx��ݒ肷��K�v������B
	// �������A�ɉ^���͊ϑ����ʂ����Ƃɐ�������Ɍ��肳���l�ł��邽�߁A
	// �ϑ������͗\���l�����Ă��������Ȃ��B�܂������Ȍv�Z������̂�
	// �Ȃ���΁A�ɉ^��(�}0.3�����x)�͏����ȗʂȂ̂Ŗ������ėǂ��B
	// �����y�n���@�����s����ܖ����̂P�E�񖜌ܐ番�̂P�̒n�`�}�́A
	// ���n�o�ܓx�ō쐬����Ă���A�V���o�ܓx�Ƃ͍ő�30���̂��ꂪ
	// ����B
	// @param longitude �V���o�x(���o�𐳁A���o�𕉂Ƃ���)
	// @param latitude  �V���ܓx
	void setPosition(const Degree& longitude, const Degree& latitude) {
		lt = latitude;
		lg = longitude;
		recalcMat = true;
	}

	// �V���ܓx�𓾂�
	const Degree& latitude() const  { return lt; }

	// �V���o�x�𓾂�(���o�𐳁A���o�𕉂Ƃ���)
	const Degree& longitude() const { return lg; }


	//----- �ϑ��ʒu�̐ݒ�E�擾 -------------------------------------
	// �ϑ��ʒu�͌���l�H�q���Ȃǒn���ߖT�̓V�̂̒n�S�ʒu��
	// ���S�ʒu�ɕϊ����邽�߂Ɏg�p����

	// �ϑ��ʒu��ݒ肷��
	// @param longitude ���n�o�x(���o�𐳁A���o�𕉂Ƃ���)
	// @param latitude  ���n�ܓx
	// @param h         �C�����x[m]
	void setLocation(const Degree& longitude, const Degree& latitude, double h = 0);

	// �ϑ��ʒu�𓾂�
	// @return �n���d�S�����_�Ƃ����ϑ��ʒu�̒������W[m]
	Vec3 getLocation() const { return location; }

	// �n�S���ϐԓ����W�̊ϑ��ʒu�𓾂�
	// getLocation()���O���j�W���ύP����������]�����l�ł���
	// �������ɂ͋ɉ^��[�}0.3��]���v�Z�ɓ����ׂ������A
	//   �����Ȓl�Ȃ̂ŁA�����ł͖�������B
	// @return �n���d�S�����_�Ƃ����ϑ��ʒu�̕��ϐԓ��������W[m]
	Vec3 getQLocation() const {
		return location * Mat3x3(Degree(gmst(), asHs()), 'Z');
	}


	//----- �V�������̐ݒ�Ǝ擾 -------------------------------------

	// �V��������ݒ肷��
	void setTime(const AstroTime& a) { atime = a; calc(); }

	// �ݒ肵���V�������𓾂�
	const AstroTime& getTime() const { return atime; }


	//----- ���ݎ����ɍX�V���� ---------------------------------------

	// �b�P�ʂ̌��ݎ����X�V
	void updateTime()	{ atime.update(); calc(); }

	// �~���b�P�ʂ̌��ݎ����X�V
	void updateSystemTime()	{ atime.updateSystemTime(); calc(); }


	//----- �P����[�b]�̎擾 -----------------------------------------

	// �O���j�W���ύP����[�b] [0�`86400)
	double gmst() const	{ return m_gmst; }

	// �n�����ύP����[�b] [0�`86400)
	double lmst() const	{ return hs2mod1(gmst() + lg.hs()); }

	// �O���j�W���P����[�b] [0�`86400)
	double gast() const	{ return hs2mod1(gmst() + Eq); }

	// �n�����P����[�b] [0�`86400)
	double lst() const	{ return hs2mod1(gmst() + Eq + lg.hs()); }


	//----- ��C���̌v�Z ---------------------------------------------
	// ��C�� = �����x - �^���x
private:
	// �����x��sin,cos�����C���𓾂�
	Degree refractionApp(double sinAlt, double cosAlt) const;
public:
	// �����x�ɑ΂����C���𓾂�
	Degree refractionApp(Degree alt) const {
		return refractionApp(sin(alt), cos(alt));
	}

	// �^���x�ɑ΂����C���𓾂�
	Degree refractionTrue(Degree alt) const;

	// ���n�����W�ɑ΂����C���𓾂�
	Degree refractionApp(const Vec3& vh) const;

	// �^�n�����W�ɑ΂����C���𓾂�
	Degree refractionTrue(const Vec3& vh) const;

	//----- ���W�ϊ� -------------------------------------------------

	// ���W�ϊ��̑O�������s��
	// ���W�ϊ��s���K�v�ɉ����čČv�Z����
	void beginConvert() {
		if (recalcMat)  calcMat();
		if (recalcMat2) calcMat2();
	}

	// �ԓ����W����n�����W�֕ϊ�����(���ʒu)
	void conv_q2h(Vec3& v) const { v *= q2h; }

	// �n�����W����ԓ����W�֕ϊ�����(���ʒu)
	void conv_h2q(Vec3& v) const { v *= h2q; }

	// �ԓ����W���物�����W�֕ϊ�����(���ψʒu)
	void conv_q2c(Vec3& v) const { v *= q2c; }

	// �������W����ԓ����W�֕ϊ�����(���ψʒu)
	void conv_c2q(Vec3& v) const { v *= c2q; }

	// �ԓ����W���物�����W�֕ϊ�����(�^�ʒu)
	void conv_tq2tc(Vec3& v) const { v *= tq2tc; }

	// �������W����ԓ����W�֕ϊ�����(�^�ʒu)
	void conv_tc2tq(Vec3& v) const { v *= tc2tq; }

	// �n�����W�ɂ����Đ^���x���猩�����̍��x�ւ̑�C���␳���s��
	void addRefraction(Vec3& vh) const;

	// �n�����W�ɂ����Č������̍��x����^���x�ւ̑�C���␳���s��
	void subRefraction(Vec3& vh) const;

	// J2000�ԓ����W����u���̕��ϐԓ����W�֕ϊ�����(�΍��␳)
	void conv_j2q(Vec3& v) const { v *= j2q; }

	// �u���̕��ϐԓ����W����J2000�ԓ����W�֕ϊ�����(�΍��␳)
	void conv_q2j(Vec3& v) const { v *= q2j; }

	// �u���̕��ϐԓ����W����u���̐^�ԓ����W�֕ϊ�����(�͓��␳)
	void conv_q2tq(Vec3& v) const { v *= q2tq; }

	// �u���̐^�ԓ����W����u���̕��ϐԓ����W�֕ϊ�����(�͓��␳)
	void conv_tq2q(Vec3& v) const { v *= tq2q; }

	// �u���̐^�ԓ����W����u���̎��ԓ����W�֕ϊ�����(�P���̔N�����s���␳)
	void addAnnualAberration(Vec3& v) const;

	// �u���̎��ԓ����W����u���̐^�ԓ����W�֕ϊ�����(�P���̔N�����s���␳)
	void subAnnualAberration(Vec3& v) const;

	// J2000.0���S�ԓ����W����n�S�ԓ����W�֕ϊ�����[AU]
	void conv_js2je(Vec3& v) const { v += sunJ; }

	// �u���̓��S�ԓ����W����n�S�ԓ����W�֕ϊ�����[AU]
	void conv_qs2qe(Vec3& v) const { v += sunQ; }
};

}//endnamespace astro
#endif // ACOORD_H_
