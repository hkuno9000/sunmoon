//. acoord.cpp - �V�����W�n
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#include "acoord.h"
#include "planets.h"
using namespace util;
using namespace astro;

//------------------------------------------------------------------------
//.----- class AstroCoordinate : �V�����W�n ------------------------------
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//. ���ύP�����E�������ω��o�E�΍��E�͓����v�Z����

//. AstroCoordinate::calc - �v�Z���C��
void
AstroCoordinate::calc()
{
	// ���ύP����
	m_gmst = atime.gmst();

	// T = J2000.0����̃����E�X���I[TD]
	const double T = atime.j2000() / 36525;

	// ��������10�b�ȓ��Ȃ�A�΍���͓��͕ω��ʂ��������̂Ōv�Z�s�v�ł���
	if (lastT == 0 || fabs(T - lastT) * (36525 * 86400.0) >= 10) {
		calc2(lastT = T); // �΍��Ə͓��A���_���̌v�Z
		recalcMat2 = true;
	}
	recalcMat = true;
}

//. AstroCoordinate::calc2 - �΍��E�͓��̌v�Z
void
AstroCoordinate::calc2(double T)
{
	// T = J2000.0����̃����E�X���I[TD]
	const double T2 = T * T;
	const double T3 = T * T * T;
	double x;

	// ��ʍ΍�(general precession J2000.0)�̊p�x[Degree]
	// �ω��ʂ� 7e-7��/ sec ���炢�ł���
	// ��A = 2306.2181��T + 0.30188��T^2 + 0.017998��T^3
	// ��A = 2306.2181��T + 1.09468��T^2 + 0.018203��T^3
	// ��A = 2004.3109��T - 0.42665��T^2 - 0.041833��T^3
	// ��A = 5029.0966��T - 1.11113��T^2 - 0.000006��T^3
	za.setSec(2306.2181 * T + 0.30188 * T2 + 0.017998 * T3);
	Za.setSec(2306.2181 * T + 1.09468 * T2 + 0.018203 * T3);
	ta.setSec(2004.3109 * T - 0.42665 * T2 - 0.041833 * T3);
	Pa.setSec(5029.0966 * T - 1.11113 * T2 - 0.000006 * T3);

	// ���ω����X�p(mean obliquity of the ecliptic)[rad]
	// �ω��ʂ� 1e-8��/ sec ���炢�ł���
	// ��A = 23��26��21.448��
	//		- 46.8150��T - 0.00059��T^2 + 0.001813��T^3
	x = dms2ds(23, 26, 21.448)
			- 46.8150 * T - 0.00059 * T2 + 0.001813 * T3;
	moe = ds2rad(x);

	// ���z�̕��ω��o
	// �ω��ʂ� 0.04��/ sec �ł���
	// L = 280��27��59.24�� + 129602771.103��T + 1.092��T^2
	//   =       ''         + 100r2771.103��T  + 1.092��T^2
	x = dms2ds(280, 27, 59.24)
			+ 1296000.0 * fmod(100 * T, 1) + 2771.103 * T + 1.092 * T2;
	const double L = ds2rad(ds2mod1(x));

	// ���̕��ω��o
	// �ω��ʂ� 0.54��/ sec �ł���
	// Lm= 218��18��59.92��+ 1336r307��52��52.581��T - 5.279��T^2
	//   =        ''       + 1336r1108372.581��T     - 5.279��T^2
	x = dms2ds(218, 18, 59.92)
			+ 1296000.0 * fmod(1336 * T, 1) + 1108372.581 * T - 5.279 * T2;
	const double Lm = ds2rad(ds2mod1(x));

	// ���z�̋ߒn�_�̕��ω��o
	// �� = 282��56��14.45��+ 6190.055��T + 1.645��T^2
	x = dms2ds(282, 56, 14.45) + 6190.055 * T + 1.645 * T2;
	const double gamma = ds2rad(x);

	// ���̋ߒn�_�̕��ω��o
	// ��' = 83��21��11.68��+ 11r109��00��49.364��T - 37.158��T^2
	//     =        ''      + 11r392449.364��T      - 37.158��T^2
	x = dms2ds(83, 21, 11.68)
			+ 1296000.0 * fmod(11 * T, 1) + 392449.364 * T + 37.158 * T2;
	const double gamma_m = ds2rad(x);

	// ���̕��Ϗ���_���o
	// ��= 125��02��40.40��- 5r134��08��10.267��T + 7.472��T^2
	//   =        ''       - 5r482890.267��T + 7.472��T^2
	x = dms2ds(125, 2, 40.40)
			- 1296000.0 * fmod(5 * T, 1) - 482890.267 * T + 7.472 * T2;
	const double omega = ds2rad(x);

	// ���o�͓̏� (IAU1980�͓����_ ���� 0.004���ȏ�̂��̂̂�)
	// �P�����̎��ʒu��0.001s�̒P�ʂɂ������̂ł�⍂���x�̌v�Z�����Ă���
	// �ω��ʂ� 1e-6��/ sec ���炢�ł���
	// ����=(-17.1996��- 0.01742��T) sin(��)       ����6798��
	//      +(-1.3187��- 0.00016��T) sin(2L)       ���� 183��
	//      +(+0.2062��+ 0.00002��T) sin(2��)      ����3399��
	//      +(+0.1426��- 0.00034��T) sin(L-��)     ���� 365��
	//      +(-0.0517��+ 0.00012��T) sin(3L-��)    ���� 122��
	//      +(+0.0217��- 0.00005��T) sin(L+��)     ���� 365��
	//      +(+0.0129��+ 0.00001��T) sin(2L-��)    ���� 178��
	//      +(+0.0048��            ) sin(2L-2��')  ���� 206��
	//      +(+0.0046��            ) sin(2��'-��)  ����1305��
	//      +(-0.2274��- 0.00002��T) sin(2Lm)        ����13.7��
	//      +(+0.0712��+ 0.00001��T) sin(Lm-��')     ����27.6��
	//      +(-0.0386��- 0.00004��T) sin(2Lm-��)     ����13.6��
	//      +(-0.0301��            ) sin(3Lm-��')    ���� 9.1��
	//      +(-0.0158��            ) sin(-Lm+2L-��') ����31.8��
	//      +(+0.0123��            ) sin(Lm+��')     ����27.1��
	//      +(+0.0063��+ 0.00001��T) sin(Lm-��'+��)  ����27.7��
	//      +(+0.0063��            ) sin(2Lm-2L)     ����14.8��
	//      +(-0.0059��            ) sin(3Lm-2L+��') ���� 9.6��
	//      +(-0.0058��- 0.00001��T) sin(-Lm+��'+��) ����27.4��
	//      +(-0.0051��            ) sin(3Lm-��'-��) ���� 9.1��

	x = 	(-17.1996 - 0.01742 * T) * sin(omega)
		+(-1.3187 - 0.00016 * T) * sin(2 * L)
		+( 0.2062 + 0.00002 * T) * sin(2 * omega)
		+( 0.1426 - 0.00034 * T) * sin(L - gamma)
		+(-0.0517 + 0.00012 * T) * sin(3 * L - gamma)
		+( 0.0217 - 0.00005 * T) * sin(L + gamma)
		+( 0.0129 + 0.00001 * T) * sin(2 * L - omega)
		+( 0.0048              ) * sin(2 * L - 2 * gamma_m)
		+( 0.0046              ) * sin(2 * gamma_m - omega)
		+(-0.2274 - 0.00002 * T) * sin(2 * Lm)
		+( 0.0712 + 0.00001 * T) * sin(Lm - gamma_m)
		+(-0.0386 - 0.00004 * T) * sin(2 * Lm - omega)
		+(-0.0301              ) * sin(3 * Lm - gamma_m)
		+(-0.0158              ) * sin(-Lm + 2 * L - gamma_m)
		+( 0.0123              ) * sin(Lm + gamma_m)
		+( 0.0063 + 0.00001 * T) * sin(Lm - gamma_m + omega)
		+( 0.0063              ) * sin(2 * Lm - 2 * L)
		+(-0.0059              ) * sin(3 * Lm - 2 * L + gamma_m)
		+(-0.0058 - 0.00001 * T) * sin(-Lm + gamma_m + omega)
		+(-0.0051              ) * sin(3 * Lm - gamma_m - omega)
		;
	nlg = ds2rad(x);

	// �����X�p�͓̏� (IAU1980�͓����_ ���� 0.05���ȏ�̂��̂̂�)
	// �ω��ʂ� 1e-6��/ sec ���炢�ł���
	// ���� =(+9.2025��+ 0.00089��T) cos(��)    ����6798��
	//     + (+0.5736��- 0.00031��T) cos(2L)    ���� 183��
	//     + (-0.0895��+ 0.00005��T) cos(2��)   ����3399��
	//     + (+0.0977��- 0.00005��T) cos(2Lm)   ����13.7��
	x = 	(  9.2025  + 0.00089 * T) * cos(omega)
		+( 0.5736  - 0.00031 * T) * cos(2 * L)
		+(-0.0895  + 0.00005 * T) * cos(2 * omega)
		+( 0.0977  + 0.00005 * T) * cos(2 * Lm)
		;
	nob = ds2rad(x);

	// ���_��(�Ԍo�����͓̏�)
	// �ω��ʂ� 1e-7��/ sec ���炢�ł���
	// ���ύP�������王�P���������߂邽�߂Ɏg�p����
	Eq = rad2hs(nlg) * cos(moe + nob);

    // ���z���o�A�n�S�����v�Z
    Planets::calc_sun(T, Ls, Rs);
}


//------------------------------------------------------------------------
//. ���W�ϊ��s����v�Z����

//. AstroCoordinate::calcMat - ���ԓ��^�n�����W�ϊ��s��̌v�Z
void
AstroCoordinate::calcMat()
{
	Degree z(lst(), asHs());
	Degree y(90, 0, -lt.sec()); // 90��- lt  �ԓ��̍��x

	// �n�����ԓ����W�ϊ��s��
	h2q.setRotate(y, 'Y');
	h2q *= Mat3x3(z, 'Z');

	// �ԓ����n�����W�ϊ��s��
	z.setNeg();
	y.setNeg();
	q2h.setRotate(z, 'Z');
	q2h *= Mat3x3(y, 'Y');

	// �Čv�Z����
	recalcMat = false;
}

//. AstroCoordinate::calcMat2 - �΍��E�͓��E�ԓ��^�������W�ϊ��s��̌v�Z
void
AstroCoordinate::calcMat2()
{
	// �΍��i50.3���^�N�j�␳�s��
	Degree d1(za); d1 -= Degree(90*3600); // ��A - 90��
	Degree d2(ta); d2.setNeg();           // -��A
	Degree d3(Za); d3 += Degree(90*3600); // ��A + 90��
	j2q.setRotate(d1, 'Z');
	j2q *= Mat3x3(d2, 'X');
	j2q *= Mat3x3(d3, 'Z'); 	// J2000.0�ԓ� �� ���ϐԓ�
	d1.setNeg();
	d2.setNeg();
	d3.setNeg();
	q2j.setRotate(d3, 'Z');
	q2j *= Mat3x3(d2, 'X');
	q2j *= Mat3x3(d1, 'Z'); 	// J2000.0�ԓ� �� ���ϐԓ�

	// �͓��i�}9���j�␳�s��
	d1.setRadian(-moe);         // -��A
	d2.setRadian(nlg);          // ����
	d3.setRadian(moe + nob);    // ��A + ����
	q2tq.setRotate(d1, 'X');
	q2tq *= Mat3x3(d2, 'Z');
	q2tq *= Mat3x3(d3, 'X');	// ���ϐԓ�  �� �^�ԓ�
	d1.setNeg();
	d2.setNeg();
	d3.setNeg();
	tq2q.setRotate(d3, 'X');
	tq2q *= Mat3x3(d2, 'Z');
	tq2q *= Mat3x3(d1, 'X');	// ���ϐԓ�  �� �^�ԓ�

	// �N�����s��(�ő�20��)�␳�x�N�g��
	const double cosL = cos(Ls);
	const double k = 9.936508e-5;	// ���s���萔
	k_abr.x = k * sin(Ls);		// k * �n�����]�^���̕����]��
	k_abr.y = k * -cosL * cos(moe + nob);
	k_abr.z = k * -cosL * sin(moe + nob);

	// ���ϐԓ��^�������W�ϊ��s��
	d1.setRadian(moe);
	c2q.setRotate(d1, 'X');		// ���ω���  �� ���ϐԓ�
	d1.setNeg();
	q2c.setRotate(d1, 'X');		// ���ω���  �� ���ϐԓ�

	// �^�ԓ��^�������W�ϊ��s��
	d1.setRadian(moe + nob);    // ��A + ����
	tc2tq.setRotate(d1, 'X');	// �^����  �� ���ϐԓ�
	d1.setNeg();
	tq2tc.setRotate(d1, 'X');   // �^����  �� ���ϐԓ�

    // ���z�̒n�S�􉽊w�ԓ����W[AU] �����s���܂܂�
    sunJ.setLtLg(Rs, Degree(0), Ls - Pa);					// J2000.0�̉������W
    sunJ *= Mat3x3(Degree(dms2ds(23, 26, 21.448)), 'X');	// J2000.0�̕��ω����X�p
    sunQ.setLtLg(Rs, Degree(0), Ls);		// �u���̕��ω������W
    sunQ *= c2q;

	// �Čv�Z����
	recalcMat2 = false;
}

#define NEWABR
//------------------------------------------------------------------------
//. ���s��

//. AstroCoordinate::addAnnualAberration - �P���̔N�����s���ǉ�
void
AstroCoordinate::addAnnualAberration(Vec3& v) const
{
#ifdef NEWABR
	double r = v.radius();
	v += k_abr * r;
	v *= r / v.radius();
#else
	v.setDirectionCosines();
	const double x = v.x;
	const double y = v.y;
	const double z = v.z;
	v += k_abr * Mat3x3(
		1-x*x,	-y*x,	-z*x,
		-x*y,	1-y*y,	-z*y,
		-x*z,	-y*z,	1-z*z
	);
#endif
}

//. AstroCoordinate::subAnnualAberration - �P���̔N�����s������
void
AstroCoordinate::subAnnualAberration(Vec3& v) const
{
#ifdef NEWABR
	double r = v.radius();
	v -= k_abr * r;
	v *= r / v.radius();
#else
	v.setDirectionCosines();
	const double x = v.x;
	const double y = v.y;
	const double z = v.z;
	v -= k_abr * Mat3x3(
		1-x*x,	-y*x,	-z*x,
		-x*y,	1-y*y,	-z*y,
		-x*z,	-y*z,	1-z*z
	);
#endif
}


//------------------------------------------------------------------------
//. ��C�� (��C�� = �����x - �^���x)

//. refractionApp - �����x�ɑ΂����C���𓾂�.
// �����P�̃��h�[�̑�C���\(RADAU'S REFRACTION)�ɋߎ������l��������
// �悤�Ɏ�������g�ݗ��ĂČv�Z���Ă���B���̐��x�́A
// �����x 90���`11���ɑ΂��� �}0.1���A
// �����x 11���`-1���ɑ΂��� �}2���̐��x�ł���B
// [����1]�����X�N �V�̈ʒu�\�A�C��ۈ���
//
//	�����x	��C��[��]
//	-1	3387.5
//	0	2196.0
//	2	1146.6
//	5	613.5
//	8	406.8
//	11	300.8
//	15	221.0
//	20	163.8
//	25	128.3
//	30	103.8
//	35	85.6
//	40	71.5
//	45	60.0
//	50	50.4
//	60	34.7
//	70	21.9
//	80	10.6
//	90	0
// @param sinAlt sin(�����x)
// @param cosAlt cos(�����x)
// @return ��C��
Degree
AstroCoordinate::refractionApp(double sinAlt, double cosAlt) const
{
	static const double
		x0 = -1.7452406437283512e-02,	// sin -1
		x1 =  0.0000000000000000e+00,	// sin 0
		x2 =  3.4899496702500969e-02,	// sin 2
		x3 =  8.7155742747658166e-02,	// sin 5
		x4 =  1.3917310096006544e-01,	// sin 8
		x5 =  1.9080899537654480e-01,	// sin 11
		y0 =  3.3875000000000000e+03,
		y1 = -1.4647424622143107e-05,
		y2 = -4.0047277054103424e+03,
		y3 =  2.3156532586059462e-04,
		y4 =  6.3426893064547585e+02,
		y5 = -6.0463879316560807e-03;

	double ds;
	if (sinAlt <= x5) {
		// ���x11���ȉ��͘A������Ԃő�C�����Z�o����
		ds = y0 + (sinAlt - x0) /
			(y1 + (sinAlt - x1) /
			(y2 + (sinAlt - x2) /
			(y3 + (sinAlt - x3) /
			(y4 + (sinAlt - x4) /
			 y5))));
	}
	else {
		// ���x11���ȏ�͎����I�ȋߎ����ŎZ�o����
		double tanz = cosAlt / sinAlt;	// tan(���V������) == 1 / tan(�����x)
		ds = 60.06058 * tanz - 0.06058 * tanz * tanz * tanz;
	}
	return Degree(ds);
}

//. refractionTrue - �^���x�ɑ΂����C���𓾂�
// @param alt �^���x
// @return ��C��
Degree
AstroCoordinate::refractionTrue(Degree alt) const
{
	Degree app;		// �����x
	Degree ref(0);		// ��C��
	do {
		app = alt + ref;
		ref = refractionApp(app);
	} while (fabs((alt + ref - app).sec()) > 0.1);
	return ref;
}

//. ���n�����W�ɑ΂����C���𓾂�
// @param vh ���n�����W
// @return ��C�� (�����x - �^���x)
Degree
AstroCoordinate::refractionApp(const Vec3& vh) const
{
	double r = vh.radius();
	double xy = sqrt(vh.x * vh.x + vh.y * vh.y);
	if (r != 0)
		return refractionApp(vh.z / r, xy / r);
	else
		return Degree(0);
}

//. �^�n�����W�ɑ΂����C���𓾂�
// @param vh �^�n�����W
// @return ��C�� (�����x - �^���x)
Degree
AstroCoordinate::refractionTrue(const Vec3& vh) const
{
	double xy = sqrt(vh.x * vh.x + vh.y * vh.y);
	Degree alt;
	alt.setArcTan2(vh.z, xy);
	return refractionTrue(alt);
}

//. AstroCoordinate::addRefraction -��C���ǉ�
void
AstroCoordinate::addRefraction(Vec3& vh) const
{
	// �^�V�����p Z �� sin,cos�ƁA�x�N�^�̓��a r �𓾂�
	double r = vh.radius();
	double cos_z = vh.z / r;
	double sin_z = sqrt(vh.x * vh.x + vh.y * vh.y) / r;

	// sin_z��0 �Ȃ�A�����v�Z��0���Z����������̂őł��؂�
	// ���̂Ƃ��V�����u z��0���ł���A��C��0���Ȃ̂őł��؂��Ă����Ȃ�
	// 1e-10 [rad] == 6.28e-9�� �ȉ��̂Ƃ��ł��؂邱�Ƃɂ���
	if (sin_z <= 1e-10) return;

	// �^�V�����p Z ���C�� ref ����������(���x����������)�����x�ɂ���
	// �v�Z���͈ȉ��̒ʂ�ł���
	//   K = sin(Z - ref) / sin Z;
	//   x = x * K;
	//   y = y * K;
	//   z = r * cos(Z - ref);
	// ���ۂ̌v�Z�ł� K �� z �̎����O�p�֐��̉��@�藝�œW�J����
	//   K = cos(ref) - cos(Z)*sin(ref)/sin(Z);
	//   z = r * (cos(Z)*cos(ref) + sin(Z)*sin(ref));
	Degree ref(refractionTrue(vh));
	double cos_ref = cos(ref);
	double sin_ref = sin(ref);
	double k = cos_ref - sin_ref * cos_z / sin_z;
	vh.x *= k;
	vh.y *= k;
	vh.z = r * (cos_z * cos_ref + sin_z * sin_ref);
}

//. AstroCoordinate::subRefraction - ��C������
void
AstroCoordinate::subRefraction(Vec3& vh) const
{
	// �V�����p Z �� sin,cos�ƁA�x�N�^�̓��a r �𓾂�
	double r = vh.radius();
	double cos_z = vh.z / r;
	double sin_z = sqrt(vh.x * vh.x + vh.y * vh.y) / r;

	// sin_z��0 �Ȃ�A�����v�Z��0���Z����������̂őł��؂�
	// ���̂Ƃ��V�����u z��0���ł���A��C��0���Ȃ̂őł��؂��Ă����Ȃ�
	// 1e-10 [rad] == 6.28e-9�� �ȉ��̂Ƃ��ł��؂邱�Ƃɂ���
	if (sin_z <= 1e-10) return;

	// �V�����p Z ���C�� ref ����������(���x��Ⴍ����)�^���x�ɂ���
	// �v�Z���͈ȉ��̒ʂ�ł���
	//   K = sin(Z + ref) / sin Z;
	//   x = x * K;
	//   y = y * K;
	//   z = r * cos(Z + ref);
	// ���ۂ̌v�Z�ł� K �� z �̎����O�p�֐��̉��@�藝�œW�J����
	//   K = cos(ref) + cos(Z)*sin(ref)/sin(Z);
	//   z = r * (cos(Z)*cos(ref) - sin(Z)*sin(ref));
	Degree ref(refractionApp(cos_z, sin_z));
	double cos_ref = cos(ref);
	double sin_ref = sin(ref);
	double k = cos_ref + sin_ref * cos_z / sin_z;
	vh.x *= k;
	vh.y *= k;
	vh.z = r * (cos_z * cos_ref - sin_z * sin_ref);
}


//------------------------------------------------------------------------
//. �n�S���W�ϊ�

//. AstroCoordinate::setLocation - �ϑ��ʒu��ݒ肷��
// ����l�H�q���Ȃǒn���ߖT�̓V�̂ɑ΂���␳���s�����߂Ɏg�p����
// @param longitude ���n�o�x(���o�𐳁A���o�𕉂Ƃ���)
// @param latitude  ���n�ܓx
// @param h         �C�����x[m]
void
AstroCoordinate::setLocation(const Degree& longitude, const Degree& latitude, double h)
{
	const double a = 6377397.15500;      // �n���ȉ~�̂̐ԓ����a[m]
	const double e2 = 0.006674372230614; // ''          ���S��^2
	const double N = a / sqrt(1 - e2 * sin(latitude)); // �������ȗ����a

	// �n���ȉ~�̏�̊􉽊w�I�ʒu�����߂�
	location.setLtLg(1, latitude, longitude);
	location.x *= N + h;
	location.y *= N + h;
	location.z *= N * (1 - e2) + h;

	// �n���ȉ~�̂̒��S�ƒn���d�S�̂����␳���A�d�S�����_�Ƃ���
	// ���v�Z�̒P�ʂ̓��[�g���ł���
	location.x += -136;
	location.y +=  521;
	location.z +=  681;
}

//------------------------------------------------------------------------
#ifdef TEST
#include <cstdio>
#include <cstdlib>
#include <windows.h>
int main(int argc, char** argv)
{
	if (argc == 2 && strcmp(argv[1], "self") == 0) {
		// �\�[�X���̃e�X�g�p�^�[���Ŏ��ȃe�X�g����
		system("sed /^1996/,/^1997.11/!d acoord.cpp >$in");
		system("acoord <$in >$out");
		system("fc $in $out");
		return EXIT_SUCCESS;
	}
	if (argc == 2 && strcmp(argv[1], "ref") == 0) {
		// ��C���e�X�g
		AstroCoordinate ac;
		freopen("kisa.txt", "r", stdin);
		char buf[256];
		while (gets(buf)) {
			double h;	// �����x
			double d;	// ��C�� [��]
			sscanf(buf, "%lf %lf", &h, &d);
			Vec3 v(1, Degree(90 - h, 0, 0), Degree(0)); 
			Degree ref(ac.refractionApp(v));
			printf("%s\t%.3f\t%.3f\n", buf, ref.sec(), d - ref.sec());
			// Degree trueAlt(h * 3600 - ref.sec());
			// Degree appAlt(trueAlt + ac.refractionTrue(trueAlt));
			// printf("delta=%20.12f[sec]\n", h * 3600 - appAlt.sec());
		}
		return EXIT_SUCCESS;
	}
	if (argc == 2 && strcmp(argv[1], "abr") == 0) {
		// ���s���e�X�g
		AstroCoordinate ac;
		ac.setTime(AstroTime(Jday(1978, 6, 10), 0));
		ac.beginConvert();
		for (int ra = 0; ra < 24; ra += 3) {
			for (int z = 0; z <= 180; z += 30) {
				Degree dz(z, 0, 0);
				Degree dt(ra * 15, 0, 0);
				Degree d1, d2;
				Vec3 v(1, dz, dt), v1, v2;
				v1 = v;
				ac.addAnnualAberration(v1);
				d1.setArcTan2((v1 - v).radius(), 1);
				v2 = v1;
				ac.subAnnualAberration(v2);
				d2.setArcTan2((v2 - v).radius(), 1);
				printf("ra:%02dh dec:%+03dd\n", ra, 90 - z);
				printf("add:x%+.10f y%+.10f z%+.10f %10.5fsec\n",
					v1.x - v.x,
					v1.y - v.y,
					v1.z - v.z,
					acos(v1.inner(v) / v1.radius()) * RAD2DS
				);
				printf("sub:x%+.10f y%+.10f z%+.10f %10.5fsec\n",
					v2.x - v.x,
					v2.y - v.y,
					v2.z - v.z,
					acos(v2.inner(v) / v2.radius()) * RAD2DS
				);
			}
		}
		return EXIT_SUCCESS;
	}

	Jday dd(0);
	char buf[256];
	int y, m, d=1; double utc;
	AstroCoordinate ac;
#if 1
	long t0 = GetTickCount();
	int i;
	for (i = 0; i < 10000; ++i)
//		ac.updateTime();	// calc()�v�Z���� 5367 / 10000 ms
//		ac.updateSystemTime();	// calc()�v�Z���� 5643 / 10000 ms
		ac.setTime(ac.getTime());//calc()�v�Z���� 1525 / 10000 ms
	long t1 = GetTickCount();
	cerr << "calc()�v�Z���� " << (t1 - t0) << " / " << i << " ms" << endl;
#endif
	cerr << "y.m.d.sec" << endl;
	while (gets(buf)) {
		if (sscanf(buf, "%d.%d.%d.%lf", &y, &m, &d, &utc) >= 3)
			ac.setTime(AstroTime(Jday(y, m, d), utc));
		else
			ac.updateTime();

		ac.getTime().get(y, m, d, utc);
		printf("%4d.%02d.%02d.%05d, jd:%.8g",
			y, m, d, (int)utc, ac.getTime().jd());

		int hh, mm; double ss; char c;
		f2ims(ac.gmst()/3600, c, hh, mm, ss);
		printf(", gmst:%02dh%02dm%.3fs", hh, mm, ss);
		f2ims(ac.gast()/3600, c, hh, mm, ss);
		printf(", gast:%02dh%02dm%.3fs\n", hh, mm, ss);
	}
	return EXIT_SUCCESS;
}
/*
1996.12.31.00000, jd:2450448.5, gmst:06h38m47.588s, gast:06h38m47.675s
1997.02.15.00000, jd:2450494.5, gmst:09h40m9.134s, gast:09h40m9.225s
1997.05.17.00000, jd:2450585.5, gmst:15h38m55.673s, gast:15h38m55.529s
1997.11.16.00000, jd:2450768.5, gmst:03h40m25.305s, gast:03h40m24.958s
*/
#endif
//. acoord.cpp - end
