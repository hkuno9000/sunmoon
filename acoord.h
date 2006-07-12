//. acoord.h - 天文座標系
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
//.----- class AstroCoordinate : 天文座標系 ------------------------------
//------------------------------------------------------------------------
// 天文座標系クラス
class AstroCoordinate {
	double lastT;		// calc()で使用するキャッシュ
	bool recalcMat;		// calcMat()の再計算フラグ
	bool recalcMat2;	// calcMat2()の再計算フラグ
protected:
	// 天文経緯度(極運動も含めた瞬時の天文経緯度)
	Degree lt;		// 天文緯度 [°]
	Degree lg;		// 天文経度 [°E]

	// 観測位置
	Vec3 location;		// 地球重心を原点とした直交座標 [m]

	// 天文時刻
	AstroTime atime;	// ユリウス日、秒
	double m_gmst;		// グリニジ平均恒星時[秒]
	double Eq;		// 分点差(Equation of Equionxes) [秒]

	// 一般歳差(general precession J2000.0)の角度[degree]
	Degree za;		// ζA
	Degree Za;		// ΖA
	Degree ta;		// θA
    Degree Pa;		// ρA

	// 章動(nutation J2000.0)の角度[rad]
	double moe;		// 平均黄道傾角  (εA:mean obliquity of the ecliptic) [rad]
	double nlg;		// 黄経の章動    (Δφ:nutation in longitude) [rad]
	double nob;		// 黄道傾角の章動(Δε:nutation in obliquity) [rad]

	// 太陽の平均黄経、地心距離
	Degree Ls;		// 瞬時の平均春分点による黄経
	double Rs;		// 地心距離[AU]

    // 太陽の地心幾何学赤道座標[AU] ※光行差含まず
    Vec3 sunJ;		// J2000.0の赤道座標[AU]
    Vec3 sunQ;		// 瞬時の平均赤道座標[AU]

	// 視赤道／地平座標変換行列
	Mat3x3 q2h;		// 赤道→地平
	Mat3x3 h2q;		// 地平→赤道

	// 平均赤道／黄道座標変換行列
	Mat3x3 c2q;		// 平均黄道  → 平均赤道
	Mat3x3 q2c;		// 平均黄道  ← 平均赤道

	// 真赤道／黄道座標変換行列
	Mat3x3 tc2tq;	// 真黄道  → 真赤道
	Mat3x3 tq2tc;   // 真黄道  ← 真赤道


	// 歳差（50.3″／年）補正行列
	Mat3x3 j2q;		// J2000.0赤道 → 平均赤道
	Mat3x3 q2j;		// J2000.0赤道 ← 平均赤道

	// 章動（±9″）補正行列
	Mat3x3 q2tq;		// 平均赤道  → 真赤道
	Mat3x3 tq2q;		// 平均赤道  ← 真赤道

	// 年周視差(annual parallax) --- 1″以下なので無視

	// 年周光行差(annual aberration) 補正ベクトル --- 最大20″
	Vec3 k_abr;		// 光行差定数 * 地球公転運動の方向余弦


	// 平均恒星時・日月平均黄経・歳差・章動を計算する
	// @param T J2000.0からのユリウス世紀[TDT]
	void calc();		// 計算メイン
	void calc2(double T);	// 歳差・章動の計算

	// 座標変換行列を計算する
	void calcMat();		// 視赤道／地平座標変換行列の計算
	void calcMat2();	// 歳差・章動・赤道／黄道座標変換行列の計算

public:
	//----- コンストラクタ -------------------------------------------
	AstroCoordinate() {
		lastT = 0; calc();
	}


	//----- 天文経緯度の設定と取得 -----------------------------------

	// 天文経緯度を設定する
	// ※地方恒星時や高度方位変換に使用するので、極運動も含めた瞬時の
	// 天文経緯度を設定する必要がある。
	// しかし、極運動は観測結果をもとに数ヶ月後に決定される値であるため、
	// 観測当日は予測値を入れておくしかない。また厳密な計算をするので
	// なければ、極運動(±0.3″程度)は小さな量なので無視して良い。
	// ※国土地理院が発行する五万分の１・二万五千分の１の地形図は、
	// 測地経緯度で作成されており、天文経緯度とは最大30″のずれが
	// ある。
	// @param longitude 天文経度(東経を正、西経を負とする)
	// @param latitude  天文緯度
	void setPosition(const Degree& longitude, const Degree& latitude) {
		lt = latitude;
		lg = longitude;
		recalcMat = true;
	}

	// 天文緯度を得る
	const Degree& latitude() const  { return lt; }

	// 天文経度を得る(東経を正、西経を負とする)
	const Degree& longitude() const { return lg; }


	//----- 観測位置の設定・取得 -------------------------------------
	// 観測位置は月や人工衛星など地球近傍の天体の地心位置を
	// 測心位置に変換するために使用する

	// 観測位置を設定する
	// @param longitude 測地経度(東経を正、西経を負とする)
	// @param latitude  測地緯度
	// @param h         海抜高度[m]
	void setLocation(const Degree& longitude, const Degree& latitude, double h = 0);

	// 観測位置を得る
	// @return 地球重心を原点とした観測位置の直交座標[m]
	Vec3 getLocation() const { return location; }

	// 地心平均赤道座標の観測位置を得る
	// getLocation()をグリニジ平均恒星時だけ回転した値である
	// ※厳密には極運動[±0.3″]も計算に入れるべきだが、
	//   小さな値なので、ここでは無視する。
	// @return 地球重心を原点とした観測位置の平均赤道直交座標[m]
	Vec3 getQLocation() const {
		return location * Mat3x3(Degree(gmst(), asHs()), 'Z');
	}


	//----- 天文時刻の設定と取得 -------------------------------------

	// 天文時刻を設定する
	void setTime(const AstroTime& a) { atime = a; calc(); }

	// 設定した天文時刻を得る
	const AstroTime& getTime() const { return atime; }


	//----- 現在時刻に更新する ---------------------------------------

	// 秒単位の現在時刻更新
	void updateTime()	{ atime.update(); calc(); }

	// ミリ秒単位の現在時刻更新
	void updateSystemTime()	{ atime.updateSystemTime(); calc(); }


	//----- 恒星時[秒]の取得 -----------------------------------------

	// グリニジ平均恒星時[秒] [0～86400)
	double gmst() const	{ return m_gmst; }

	// 地方平均恒星時[秒] [0～86400)
	double lmst() const	{ return hs2mod1(gmst() + lg.hs()); }

	// グリニジ視恒星時[秒] [0～86400)
	double gast() const	{ return hs2mod1(gmst() + Eq); }

	// 地方視恒星時[秒] [0～86400)
	double lst() const	{ return hs2mod1(gmst() + Eq + lg.hs()); }


	//----- 大気差の計算 ---------------------------------------------
	// 大気差 = 視高度 - 真高度
private:
	// 視高度のsin,cosから大気差を得る
	Degree refractionApp(double sinAlt, double cosAlt) const;
public:
	// 視高度に対する大気差を得る
	Degree refractionApp(Degree alt) const {
		return refractionApp(sin(alt), cos(alt));
	}

	// 真高度に対する大気差を得る
	Degree refractionTrue(Degree alt) const;

	// 視地平座標に対する大気差を得る
	Degree refractionApp(const Vec3& vh) const;

	// 真地平座標に対する大気差を得る
	Degree refractionTrue(const Vec3& vh) const;

	//----- 座標変換 -------------------------------------------------

	// 座標変換の前準備を行う
	// 座標変換行列を必要に応じて再計算する
	void beginConvert() {
		if (recalcMat)  calcMat();
		if (recalcMat2) calcMat2();
	}

	// 赤道座標から地平座標へ変換する(視位置)
	void conv_q2h(Vec3& v) const { v *= q2h; }

	// 地平座標から赤道座標へ変換する(視位置)
	void conv_h2q(Vec3& v) const { v *= h2q; }

	// 赤道座標から黄道座標へ変換する(平均位置)
	void conv_q2c(Vec3& v) const { v *= q2c; }

	// 黄道座標から赤道座標へ変換する(平均位置)
	void conv_c2q(Vec3& v) const { v *= c2q; }

	// 赤道座標から黄道座標へ変換する(真位置)
	void conv_tq2tc(Vec3& v) const { v *= tq2tc; }

	// 黄道座標から赤道座標へ変換する(真位置)
	void conv_tc2tq(Vec3& v) const { v *= tc2tq; }

	// 地平座標において真高度から見かけの高度への大気差補正を行う
	void addRefraction(Vec3& vh) const;

	// 地平座標において見かけの高度から真高度への大気差補正を行う
	void subRefraction(Vec3& vh) const;

	// J2000赤道座標から瞬時の平均赤道座標へ変換する(歳差補正)
	void conv_j2q(Vec3& v) const { v *= j2q; }

	// 瞬時の平均赤道座標からJ2000赤道座標へ変換する(歳差補正)
	void conv_q2j(Vec3& v) const { v *= q2j; }

	// 瞬時の平均赤道座標から瞬時の真赤道座標へ変換する(章動補正)
	void conv_q2tq(Vec3& v) const { v *= q2tq; }

	// 瞬時の真赤道座標から瞬時の平均赤道座標へ変換する(章動補正)
	void conv_tq2q(Vec3& v) const { v *= tq2q; }

	// 瞬時の真赤道座標から瞬時の視赤道座標へ変換する(恒星の年周光行差補正)
	void addAnnualAberration(Vec3& v) const;

	// 瞬時の視赤道座標から瞬時の真赤道座標へ変換する(恒星の年周光行差補正)
	void subAnnualAberration(Vec3& v) const;

	// J2000.0日心赤道座標から地心赤道座標へ変換する[AU]
	void conv_js2je(Vec3& v) const { v += sunJ; }

	// 瞬時の日心赤道座標から地心赤道座標へ変換する[AU]
	void conv_qs2qe(Vec3& v) const { v += sunQ; }
};

}//endnamespace astro
#endif // ACOORD_H_
