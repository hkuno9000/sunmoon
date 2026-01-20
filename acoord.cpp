/// @file
/// 天文座標系
/// @author hkuno9000@gmail.com
/// @copyright 1997,1998,2026 Hiroshi Kuno. MIT License
/// @see <https://github.com/hkuno9000/sunmoon.git>
#include "acoord.h"
#include "planets.h"
using namespace std;
using namespace util;
using namespace astro;
namespace astro {

//------------------------------------------------------------------------
//.----- class AstroCoordinate : 天文座標系 ------------------------------
//------------------------------------------------------------------------

//------------------------------------------------------------------------
//. 平均恒星時・日月平均黄経・歳差・章動を計算する

/// AstroCoordinate::calc - 計算メイン
void
AstroCoordinate::calc()
{
	// 平均恒星時
	m_gmst = atime.gmst();

	// T = J2000.0からのユリウス世紀[TD]
	const double T = atime.j2000() / 36525;

	// 時刻差が10秒以内なら、歳差や章動は変化量が小さいので計算不要である
	if (lastT == 0 || fabs(T - lastT) * (36525 * 86400.0) >= 10) {
		calc2(lastT = T); // 歳差と章動、分点差の計算
		recalcMat2 = true;
	}
	recalcMat = true;
}

/// AstroCoordinate::calc2 - 歳差・章動の計算
void
AstroCoordinate::calc2(double T)
{
	// T = J2000.0からのユリウス世紀[TD]
	const double T2 = T * T;
	const double T3 = T * T * T;
	double x;

	// 一般歳差(general precession J2000.0)の角度[Degree]
	// 変化量は 7e-7″/ sec ぐらいである
	// ζA = 2306.2181″T + 0.30188″T^2 + 0.017998″T^3
	// ΖA = 2306.2181″T + 1.09468″T^2 + 0.018203″T^3
	// θA = 2004.3109″T - 0.42665″T^2 - 0.041833″T^3
	// ρA = 5029.0966″T - 1.11113″T^2 - 0.000006″T^3
	za.setSec(2306.2181 * T + 0.30188 * T2 + 0.017998 * T3);
	Za.setSec(2306.2181 * T + 1.09468 * T2 + 0.018203 * T3);
	ta.setSec(2004.3109 * T - 0.42665 * T2 - 0.041833 * T3);
	Pa.setSec(5029.0966 * T - 1.11113 * T2 - 0.000006 * T3);

	// 平均黄道傾角(mean obliquity of the ecliptic)[rad]
	// 変化量は 1e-8″/ sec ぐらいである
	// εA = 23°26′21.448″
	//		- 46.8150″T - 0.00059″T^2 + 0.001813″T^3
	x = dms2ds(23, 26, 21.448)
			- 46.8150 * T - 0.00059 * T2 + 0.001813 * T3;
	moe = ds2rad(x);

	// 太陽の平均黄経
	// 変化量は 0.04″/ sec である
	// L = 280°27′59.24″ + 129602771.103″T + 1.092″T^2
	//   =       ''         + 100r2771.103″T  + 1.092″T^2
	x = dms2ds(280, 27, 59.24)
			+ 1296000.0 * fmod(100 * T, 1) + 2771.103 * T + 1.092 * T2;
	const double L = ds2rad(ds2mod1(x));

	// 月の平均黄経
	// 変化量は 0.54″/ sec である
	// Lm= 218°18′59.92″+ 1336r307°52′52.581″T - 5.279″T^2
	//   =        ''       + 1336r1108372.581″T     - 5.279″T^2
	x = dms2ds(218, 18, 59.92)
			+ 1296000.0 * fmod(1336 * T, 1) + 1108372.581 * T - 5.279 * T2;
	const double Lm = ds2rad(ds2mod1(x));

	// 太陽の近地点の平均黄経
	// Γ = 282°56′14.45″+ 6190.055″T + 1.645″T^2
	x = dms2ds(282, 56, 14.45) + 6190.055 * T + 1.645 * T2;
	const double gamma = ds2rad(x);

	// 月の近地点の平均黄経
	// Γ' = 83°21′11.68″+ 11r109°00′49.364″T - 37.158″T^2
	//     =        ''      + 11r392449.364″T      - 37.158″T^2
	x = dms2ds(83, 21, 11.68)
			+ 1296000.0 * fmod(11 * T, 1) + 392449.364 * T + 37.158 * T2;
	const double gamma_m = ds2rad(x);

	// 月の平均昇交点黄経
	// Ω= 125°02′40.40″- 5r134°08′10.267″T + 7.472″T^2
	//   =        ''       - 5r482890.267″T + 7.472″T^2
	x = dms2ds(125, 2, 40.40)
			- 1296000.0 * fmod(5 * T, 1) - 482890.267 * T + 7.472 * T2;
	const double omega = ds2rad(x);

	// 黄経の章動 (IAU1980章動理論 から 0.004″以上のもののみ)
	// 恒星時の視位置を0.001sの単位にしたいのでやや高精度の計算をしている
	// 変化量は 1e-6″/ sec ぐらいである
	// Δφ=(-17.1996″- 0.01742″T) sin(Ω)       周期6798日
	//      +(-1.3187″- 0.00016″T) sin(2L)       周期 183日
	//      +(+0.2062″+ 0.00002″T) sin(2Ω)      周期3399日
	//      +(+0.1426″- 0.00034″T) sin(L-Γ)     周期 365日
	//      +(-0.0517″+ 0.00012″T) sin(3L-Γ)    周期 122日
	//      +(+0.0217″- 0.00005″T) sin(L+Γ)     周期 365日
	//      +(+0.0129″+ 0.00001″T) sin(2L-Ω)    周期 178日
	//      +(+0.0048″            ) sin(2L-2Γ')  周期 206日
	//      +(+0.0046″            ) sin(2Γ'-Ω)  周期1305日
	//      +(-0.2274″- 0.00002″T) sin(2Lm)        周期13.7日
	//      +(+0.0712″+ 0.00001″T) sin(Lm-Γ')     周期27.6日
	//      +(-0.0386″- 0.00004″T) sin(2Lm-Ω)     周期13.6日
	//      +(-0.0301″            ) sin(3Lm-Γ')    周期 9.1日
	//      +(-0.0158″            ) sin(-Lm+2L-Γ') 周期31.8日
	//      +(+0.0123″            ) sin(Lm+Γ')     周期27.1日
	//      +(+0.0063″+ 0.00001″T) sin(Lm-Γ'+Ω)  周期27.7日
	//      +(+0.0063″            ) sin(2Lm-2L)     周期14.8日
	//      +(-0.0059″            ) sin(3Lm-2L+Γ') 周期 9.6日
	//      +(-0.0058″- 0.00001″T) sin(-Lm+Γ'+Ω) 周期27.4日
	//      +(-0.0051″            ) sin(3Lm-Γ'-Ω) 周期 9.1日

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

	// 黄道傾角の章動 (IAU1980章動理論 から 0.05″以上のもののみ)
	// 変化量は 1e-6″/ sec ぐらいである
	// Δε =(+9.2025″+ 0.00089″T) cos(Ω)    周期6798日
	//     + (+0.5736″- 0.00031″T) cos(2L)    周期 183日
	//     + (-0.0895″+ 0.00005″T) cos(2Ω)   周期3399日
	//     + (+0.0977″- 0.00005″T) cos(2Lm)   周期13.7日
	x = 	(  9.2025  + 0.00089 * T) * cos(omega)
		+( 0.5736  - 0.00031 * T) * cos(2 * L)
		+(-0.0895  + 0.00005 * T) * cos(2 * omega)
		+( 0.0977  + 0.00005 * T) * cos(2 * Lm)
		;
	nob = ds2rad(x);

	// 分点差(赤経方向の章動)
	// 変化量は 1e-7″/ sec ぐらいである
	// 平均恒星時から視恒星時を求めるために使用する
	Eq = rad2hs(nlg) * cos(moe + nob);

    // 太陽黄経、地心距離計算
    Planets::calc_sun(T, Ls, Rs);
}


//------------------------------------------------------------------------
//. 座標変換行列を計算する

/// AstroCoordinate::calcMat - 視赤道／地平座標変換行列の計算
void
AstroCoordinate::calcMat()
{
	Degree z(lst(), asHs());
	Degree y(90, 0, -lt.sec()); // 90°- lt  赤道の高度

	// 地平→赤道座標変換行列
	h2q.setRotate(y, 'Y');
	h2q *= Mat3x3(z, 'Z');

	// 赤道→地平座標変換行列
	z.setNeg();
	y.setNeg();
	q2h.setRotate(z, 'Z');
	q2h *= Mat3x3(y, 'Y');

	// 再計算完了
	recalcMat = false;
}

/// AstroCoordinate::calcMat2 - 歳差・章動・赤道／黄道座標変換行列の計算
void
AstroCoordinate::calcMat2()
{
	// 歳差（50.3″／年）補正行列
	Degree d1(za); d1 -= Degree(90*3600); // ζA - 90°
	Degree d2(ta); d2.setNeg();           // -θA
	Degree d3(Za); d3 += Degree(90*3600); // ΖA + 90°
	j2q.setRotate(d1, 'Z');
	j2q *= Mat3x3(d2, 'X');
	j2q *= Mat3x3(d3, 'Z'); 	// J2000.0赤道 → 平均赤道
	d1.setNeg();
	d2.setNeg();
	d3.setNeg();
	q2j.setRotate(d3, 'Z');
	q2j *= Mat3x3(d2, 'X');
	q2j *= Mat3x3(d1, 'Z'); 	// J2000.0赤道 ← 平均赤道

	// 章動（±9″）補正行列
	d1.setRadian(-moe);         // -εA
	d2.setRadian(nlg);          // Δφ
	d3.setRadian(moe + nob);    // εA + Δε
	q2tq.setRotate(d1, 'X');
	q2tq *= Mat3x3(d2, 'Z');
	q2tq *= Mat3x3(d3, 'X');	// 平均赤道  → 真赤道
	d1.setNeg();
	d2.setNeg();
	d3.setNeg();
	tq2q.setRotate(d3, 'X');
	tq2q *= Mat3x3(d2, 'Z');
	tq2q *= Mat3x3(d1, 'X');	// 平均赤道  ← 真赤道

	// 年周光行差(最大20″)補正ベクトル
	const double cosL = cos(Ls);
	const double k = 9.936508e-5;	// 光行差定数
	k_abr.x = k * sin(Ls);		// k * 地球公転運動の方向余弦
	k_abr.y = k * -cosL * cos(moe + nob);
	k_abr.z = k * -cosL * sin(moe + nob);

	// 平均赤道／黄道座標変換行列
	d1.setRadian(moe);
	c2q.setRotate(d1, 'X');		// 平均黄道  → 平均赤道
	d1.setNeg();
	q2c.setRotate(d1, 'X');		// 平均黄道  ← 平均赤道

	// 真赤道／黄道座標変換行列
	d1.setRadian(moe + nob);    // εA + Δε
	tc2tq.setRotate(d1, 'X');	// 真黄道  → 平均赤道
	d1.setNeg();
	tq2tc.setRotate(d1, 'X');   // 真黄道  ← 平均赤道

    // 太陽の地心幾何学赤道座標[AU] ※光行差含まず
    sunJ.setLtLg(Rs, Degree(0), Ls - Pa);					// J2000.0の黄道座標
    sunJ *= Mat3x3(Degree(dms2ds(23, 26, 21.448)), 'X');	// J2000.0の平均黄道傾角
    sunQ.setLtLg(Rs, Degree(0), Ls);		// 瞬時の平均黄道座標
    sunQ *= c2q;

	// 再計算完了
	recalcMat2 = false;
}

#define NEWABR
//------------------------------------------------------------------------
//. 光行差

/// AstroCoordinate::addAnnualAberration - 恒星の年周光行差追加
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

/// AstroCoordinate::subAnnualAberration - 恒星の年周光行差除去
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
//. 大気差 (大気差 = 視高度 - 真高度)

/// refractionApp - 視高度に対する大気差を得る.
/// 文献１のラドーの大気差表(RADAU'S REFRACTION)に近似した値が得られる
/// ように実験式を組み立てて計算している。式の精度は、
/// 視高度 90°～11°に対して ±0.1″、
/// 視高度 11°～-1°に対して ±2″の精度である。
/// [文献1]平成９年 天体位置表、海上保安庁
/// ```
///	視高度	大気差[″]
///	-1	3387.5
///	0	2196.0
///	2	1146.6
///	5	613.5
///	8	406.8
///	11	300.8
///	15	221.0
///	20	163.8
///	25	128.3
///	30	103.8
///	35	85.6
///	40	71.5
///	45	60.0
///	50	50.4
///	60	34.7
///	70	21.9
///	80	10.6
///	90	0
/// ```
/// @param sinAlt sin(視高度)
/// @param cosAlt cos(視高度)
/// @return 大気差
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
		// 高度11°以下は連分数補間で大気差を算出する
		ds = y0 + (sinAlt - x0) /
			(y1 + (sinAlt - x1) /
			(y2 + (sinAlt - x2) /
			(y3 + (sinAlt - x3) /
			(y4 + (sinAlt - x4) /
			 y5))));
	}
	else {
		// 高度11°以上は実験的な近似式で算出する
		double tanz = cosAlt / sinAlt;	// tan(視天頂距離) == 1 / tan(視高度)
		ds = 60.06058 * tanz - 0.06058 * tanz * tanz * tanz;
	}
	return Degree(ds);
}

/// refractionTrue - 真高度に対する大気差を得る
/// @param alt 真高度
/// @return 大気差
Degree
AstroCoordinate::refractionTrue(Degree alt) const
{
	Degree app;		// 視高度
	Degree ref(0);		// 大気差
	do {
		app = alt + ref;
		ref = refractionApp(app);
	} while (fabs((alt + ref - app).sec()) > 0.1);
	return ref;
}

/// 視地平座標に対する大気差を得る
/// @param vh 視地平座標
/// @return 大気差 (視高度 - 真高度)
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

/// 真地平座標に対する大気差を得る
/// @param vh 真地平座標
/// @return 大気差 (視高度 - 真高度)
Degree
AstroCoordinate::refractionTrue(const Vec3& vh) const
{
	double xy = sqrt(vh.x * vh.x + vh.y * vh.y);
	Degree alt;
	alt.setArcTan2(vh.z, xy);
	return refractionTrue(alt);
}

/// AstroCoordinate::addRefraction -大気差追加
void
AstroCoordinate::addRefraction(Vec3& vh) const
{
	// 真天頂離角 Z の sin,cosと、ベクタの動径 r を得る
	double r = vh.radius();
	double cos_z = vh.z / r;
	double sin_z = sqrt(vh.x * vh.x + vh.y * vh.y) / r;

	// sin_z≒0 なら、続く計算で0除算が発生するので打ち切る
	// このとき天頂離隔 z≒0°であり、大気差0°なので打ち切っても問題ない
	// 1e-10 [rad] == 6.28e-9″ 以下のとき打ち切ることにする
	if (sin_z <= 1e-10) return;

	// 真天頂離角 Z を大気差 ref だけ減じて(高度を高くして)視高度にする
	// 計算式は以下の通りである
	//   K = sin(Z - ref) / sin Z;
	//   x = x * K;
	//   y = y * K;
	//   z = r * cos(Z - ref);
	// 実際の計算では K と z の式を三角関数の加法定理で展開する
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

/// AstroCoordinate::subRefraction - 大気差除去
void
AstroCoordinate::subRefraction(Vec3& vh) const
{
	// 天頂離角 Z の sin,cosと、ベクタの動径 r を得る
	double r = vh.radius();
	double cos_z = vh.z / r;
	double sin_z = sqrt(vh.x * vh.x + vh.y * vh.y) / r;

	// sin_z≒0 なら、続く計算で0除算が発生するので打ち切る
	// このとき天頂離隔 z≒0°であり、大気差0°なので打ち切っても問題ない
	// 1e-10 [rad] == 6.28e-9″ 以下のとき打ち切ることにする
	if (sin_z <= 1e-10) return;

	// 天頂離角 Z を大気差 ref だけ増して(高度を低くして)真高度にする
	// 計算式は以下の通りである
	//   K = sin(Z + ref) / sin Z;
	//   x = x * K;
	//   y = y * K;
	//   z = r * cos(Z + ref);
	// 実際の計算では K と z の式を三角関数の加法定理で展開する
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
//. 地心座標変換

/// AstroCoordinate::setLocation - 観測位置を設定する.
/// @details 月や人工衛星など地球近傍の天体に対する補正を行うために使用する
/// @param longitude 測地経度(東経を正、西経を負とする)
/// @param latitude  測地緯度
/// @param h         海抜高度[m]
void
AstroCoordinate::setLocation(const Degree& longitude, const Degree& latitude, double h)
{
#ifdef OLD_LOCATION_SYSTEM
	// 日本測地系(ベッセル楕円体)
	const double a = 6377397.15500;      // 地球楕円体の赤道半径[m]
	const double e2 = 0.006674372230614; // ''          離心率^2
#else
	// 世界測地系(GRS80). http://ja.wikipedia.org/wiki/GRS80
	const double a = 6378137.00000;         // 地球楕円体の赤道半径[m]
	const double e2 = 0.006694380022900788; // ''          離心率^2
#endif
	const double N = a / sqrt(1 - e2 * sin(latitude)); // 東西線曲率半径

	// 地球楕円体上の幾何学的位置を求める
	location.setLtLg(1, latitude, longitude);
	location.x *= N + h;
	location.y *= N + h;
	location.z *= N * (1 - e2) + h;

#ifdef OLD_LOCATION_SYSTEM	// 日本測地系(ベッセル楕円体)
	// 地球楕円体の中心と地球重心のずれを補正し、重心を原点とする
	// ※計算の単位はメートルである
	location.x += -136;
	location.y +=  521;
	location.z +=  681;
#else	// 世界測地系(GRS80).
		// 重心補正は不要である.
#endif
}

}//.endnamespace astro
//------------------------------------------------------------------------
#ifdef TEST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef WIN32
#include <windows.h>
#endif
using namespace std;
using namespace util;
using namespace astro;
int main(int argc, char** argv)
{
	if (argc == 2 && strcmp(argv[1], "ref") == 0) {
		// 大気差テスト
		AstroCoordinate ac;
		freopen("kisa.txt", "r", stdin);
		char buf[256];
		while (fgets(buf, sizeof(buf), stdin)) {
			double h;	// 視高度
			double d;	// 大気差 [″]
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
		// 光行差テスト
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
#ifdef WIN32
	long t0 = GetTickCount();
	int i;
	for (i = 0; i < 10000; ++i)
//		ac.updateTime();	// calc()計算時間 5367 / 10000 ms
//		ac.updateSystemTime();	// calc()計算時間 5643 / 10000 ms
		ac.setTime(ac.getTime());//calc()計算時間 1525 / 10000 ms
	long t1 = GetTickCount();
	fprintf(stderr, "calc()計算時間 %lu / %d ms\n", (t1 - t0), i);
#endif
	fputs("y.m.d.sec\n", stderr);
	while (fgets(buf, sizeof(buf), stdin)) {
		if (sscanf(buf, "%d.%d.%d.%lf", &y, &m, &d, &utc) >= 3)
			ac.setTime(AstroTime(Jday(y, m, d), utc));
		else
			ac.updateTime();

		ac.getTime().get(y, m, d, utc);
		printf("%4d.%02d.%02d.%05d, jd:%.8g",
			y, m, d, (int)utc, ac.getTime().jd());

		int hh, mm; double ss; char c;
		sec2ims(ac.gmst(), c, hh, mm, ss);
		printf(", gmst:%02dh%02dm%.3fs", hh, mm, ss);
		sec2ims(ac.gast(), c, hh, mm, ss);
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
