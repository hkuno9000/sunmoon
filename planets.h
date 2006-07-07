//. planets.h - 惑星位置計算
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
//.----- class Planets : 惑星位置計算 ------------------------------------
//------------------------------------------------------------------------
// 惑星位置計算
class Planets {
    Degree L;                   // 太陽の幾何学平均黄経
    double R;                   // 太陽の地心距離[AU]
    Vec3 sunVc;                 // 太陽の幾何学地心平均黄道座標
    Degree Lm, Bm;              // 月の地心視黄経、視黄緯
    double Pm;                  // 月の地平視差
    Vec3 vc[10];                // SUNからPLUTOまでの瞬時の平均黄道座標位置[惑星光行差含む]
    Vec3 vq[10];                //  '' 瞬時の平均赤道座標
    Vec3 vj[10];                //  '' J2000.0赤道座標
public:
    Planets() {}

    // 惑星識別名
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

    // 日心幾何学黄道座標を、惑星光行差付きの地心黄道座標に変換する
    // @param r 動径
    // @param b 日心黄緯
    // @param l 日心黄経
    // @param c 光行差定数
    Vec3 conv(double r, Degree b, Degree l, Degree c) const;

    // 月・惑星の位置を計算する
    void calc(const AstroCoordinate& acoord);

	// 太陽の位置(平均黄経、地心距離)を計算する
	// @param T J2000.0からのユリウス世紀[TDT]
	// @param l 瞬時の平均春分点による黄経(光行差補正なし)
	//			 ※光行差補正には -0.00569度 加算すれば良い
	// @param r 地心距離[AU]
	static void calc_sun(double T, Degree& l, double& r);

    // 月・惑星の瞬時の地心平均黄道座標を得る[惑星光行差含む]
    const Vec3& vecC(int id) const { return vc[id]; }

    // 月・惑星の瞬時の地心平均赤道座標を得る[惑星光行差含む]
    // ただし月のみは、測心平均赤道座標である
    const Vec3& vecQ(int id) const { return vq[id]; }

    // 月・惑星のJ2000.0の地心赤道座標を得る[惑星光行差含む]
    // ただし月のみは、測心平均赤道座標である
    const Vec3& vecJ(int id) const { return vj[id]; }
};//endclass Planets

}//endnamespace astro
#endif // PLANETS_H_
