//. planets.h - �f���ʒu�v�Z
//	Copyright (C) 1997,1998,1999,2000 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef PLANETS_H_
#define PLANETS_H_
#include "degree.h"
#include "vec3.h"

namespace astro {
	using namespace util;
    class AstroCoordinate;

//------------------------------------------------------------------------
//.----- class Planets : �f���ʒu�v�Z ------------------------------------
//------------------------------------------------------------------------
// �f���ʒu�v�Z
class Planets {
    Degree L;                   // ���z�̊􉽊w���ω��o
    double R;                   // ���z�̒n�S����[AU]
    Vec3 sunVc;                 // ���z�̊􉽊w�n�S���ω������W
    Degree Lm, Bm;              // ���̒n�S�����o�A������
    double Pm;                  // ���̒n������
    Vec3 vc[10];                // SUN����PLUTO�܂ł̏u���̕��ω������W�ʒu[�f�����s���܂�]
    Vec3 vq[10];                //  '' �u���̕��ϐԓ����W
    Vec3 vj[10];                //  '' J2000.0�ԓ����W
public:
    Planets() {}

    // �f�����ʖ�
    enum id_e {
        SUN,
        MOON,
        MERCURY,
        VENUS,
        MARS,
        JUPITER,
        SATURN,
        URANUS,
        NEPTUNE,
        PLUTO
    };

    // ���S�􉽊w�������W���A�f�����s���t���̒n�S�������W�ɕϊ�����
    // @param r ���a
    // @param b ���S����
    // @param l ���S���o
    // @param c ���s���萔
    Vec3 conv(double r, Degree b, Degree l, Degree c) const;

    // ���E�f���̈ʒu���v�Z����
    void calc(const AstroCoordinate& acoord);

	// ���z�̈ʒu(���ω��o�A�n�S����)���v�Z����
	// @param T J2000.0����̃����E�X���I[TDT]
	// @param l �u���̕��Ϗt���_�ɂ�鉩�o(���s���␳�Ȃ�)
	//			 �����s���␳�ɂ� -0.00569�x ���Z����Ηǂ�
	// @param r �n�S����[AU]
	static void calc_sun(double T, Degree& l, double& r);

    // ���E�f���̏u���̒n�S���ω������W�𓾂�[�f�����s���܂�]
    const Vec3& vecC(int id) const { return vc[id]; }

    // ���E�f���̏u���̒n�S���ϐԓ����W�𓾂�[�f�����s���܂�]
    // ���������݂̂́A���S���ϐԓ����W�ł���
    const Vec3& vecQ(int id) const { return vq[id]; }

    // ���E�f����J2000.0�̒n�S�ԓ����W�𓾂�[�f�����s���܂�]
    // ���������݂̂́A���S���ϐԓ����W�ł���
    const Vec3& vecJ(int id) const { return vj[id]; }
};//endclass Planets

}//endnamespace astro
#endif // PLANETS_H_
