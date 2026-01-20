/// @file
/// 角度と時間の変換
/// @author hkuno9000@gmail.com
/// @copyright 1997,1998,1999,2000,2026 Hiroshi Kuno. MIT License
/// @see <https://github.com/hkuno9000/sunmoon.git>
#ifndef DEGREE_H_
#define DEGREE_H_
#include <iostream>
#include <cmath>
#include "defs.h"

namespace util {
using namespace std;

//------------------------------------------------------------------------
//.----- 定数 PI (π) ----------------------------------------------------
//------------------------------------------------------------------------
extern const double PI;    	///< π
extern const double DD2RAD;	///< π/180
extern const double HH2RAD;	///< π/12
extern const double DS2RAD;	///< π/180/3600
extern const double HS2RAD;	///< π/12/3600
extern const double RAD2DD;	///< 180/π
extern const double RAD2HH;	///< 12/π
extern const double RAD2DS;	///< 180*3600/π
extern const double RAD2HS;	///< 12*3600/π

//------------------------------------------------------------------------
/// 丸め変換 - x を r で剰余を得て [0...r)      の範囲に丸める.
/// example: fmod1(710, 360) => 350
/// @param x 丸め対象値
/// @param r 丸め範囲
/// @return 丸め値 [0...r)
/// @attention rは正の数であること
double fmod1(double x, double r);

/// 丸め変換 - x を r で剰余を得て [-r/2...r/2) の範囲に丸める.
/// example: fmod2(710, 360) => -10
/// @param x 丸め対象値
/// @param r 丸め範囲
/// @return 丸め値 [-r/2...r/2)
/// @attention rは正の数であること
double fmod2(double x, double r);

//------------------------------------------------------------------------
/// 四捨五入 - x の小数部を四捨五入して整数を得る.
double round(double x);

/// 四捨五入 - x の小数部point-1桁を四捨五入し、小数部をpoint桁に丸める.
double round(double x, int point);

//------------------------------------------------------------------------
/// 秒から度分秒へ変換する - 秒(sec)の値を符号、度分秒(i,m,s)に分ける.
/// example: sec2ims(3678.9) => sign:'+', i:1, m:1, s:18.9
/// @param sec [in] 秒値
/// @param sign [out] 符号. '+' or '-'. sec==0 は '+' とする
/// @param i [out] 度
/// @param m [out] 分
/// @param s [out] 秒
void sec2ims(double sec, char& sign, double& i, double& m, double& s);
void sec2ims(double sec, char& sign, int&    i, int&    m, double& s);

/// 分から度分へ変換する - 分(min)の値を符号、度分(i,m)に分ける.
/// example: min2im(3678.9) =>  sign:'+', i:61, m:18.9
/// @param min [in] 分値
/// @param sign [out] 符号. '+' or '-'. min==0 は '+' とする
/// @param i [out] 度
/// @param m [out] 分
void min2im(double min, char& sign, double& i, double& m);
void min2im(double min, char& sign, int&    i, double& m);

//------------------------------------------------------------------------
/// 度分秒を度の小数にする.
/// example: dms2dd(1, 30, 36) => 1.51
/// @param d [in] 度
/// @param m [in] 分
/// @param s [in] 秒
/// @return 度 + 分/60 + 秒/3600
/// @attention d,m,sは正の値であること
inline double dms2dd(double d, double m=0, double s=0) {
	return d + m / 60 + s / 3600;
}
/// 度分秒を度秒にする.
/// example: dms2ds(1, 30, 36) => 5436
/// @param d [in] 度
/// @param m [in] 分
/// @param s [in] 秒
/// @return 度*3600 + 分*60 + 秒
/// @attention d,m,sは正の値であること
inline double dms2ds(double d, double m=0, double s=0) {
	return d * 3600 + m * 60 + s;
}
/// 度分秒をラジアンにする.
/// example: dms2rad(1, 30, 36) => 5436 * π / (180*3600)
/// @param d [in] 度
/// @param m [in] 分
/// @param s [in] 秒
/// @return (度*3600 + 分*60 + 秒) * π / (180*3600)
/// @attention d,m,sは正の値であること
inline double dms2rad(double d, double m=0, double s=0) {
	return dms2ds(d, m, s) * DS2RAD;
}

//------------------------------------------------------------------------
//.----- hms2xx : 時分秒の変換 -------------------------------------------
//------------------------------------------------------------------------
/// 時分秒を時の小数にする.
/// example: hms2hh(1, 30, 36) => 1.51
/// @param h [in] 時
/// @param m [in] 分
/// @param s [in] 秒
/// @return 時 + 分/60 + 秒/3600
/// @attention h,m,sは正の値であること
inline double hms2hh(double h, double m=0, double s=0) {
	return h + m / 60 + s / 3600;
}
/// 時分秒を時秒にする.
/// example: hms2hs(1, 30, 36) => 5436
/// @param h [in] 時
/// @param m [in] 分
/// @param s [in] 秒
/// @return 時*3600 + 分*60 + 秒
/// @attention h,m,sは正の値であること
inline double hms2hs(double h, double m=0, double s=0) {
	return h * 3600 + m * 60 + s;
}
/// 時分秒をラジアンにする.
/// example: hms2rad(1, 30, 36) => 5436 * π / (12*3600)
/// @param h [in] 時
/// @param m [in] 分
/// @param s [in] 秒
/// @return (時*3600 + 分*60 + 秒) * π / (12*3600)
/// @attention h,m,sは正の値であること
inline double hms2rad(double h, double m=0, double s=0) {
	return hms2hs(h, m, s) * HS2RAD;
}

//------------------------------------------------------------------------
//.----- ds2xxx, hs2xxx : 度と時の単位変換、丸め -------------------------
//------------------------------------------------------------------------
//. dd2rad(dd)     - 度   を ラジアン にする
//. hh2rad(hh)     - 時   を ラジアン にする
//. ds2rad(ds)     - 度秒 を ラジアン にする
//. hs2rad(hs)     - 時秒 を ラジアン にする
//. rad2dd(dd)     - ラジアン を 度   にする
//. rad2hh(hh)     - ラジアン を 時   にする
//. rad2ds(ds)     - ラジアン を 度秒 にする
//. rad2hs(hs)     - ラジアン を 時秒 にする
//. dd2mod1(dd)    - 度       を [0...360)      の範囲に丸める
//. hh2mod1(dd)    - 時       を [0...24)       の範囲に丸める
//. ds2mod1(ds)    - 度秒     を [0...360*3600) の範囲に丸める
//. hs2mod1(ds)    - 時秒     を [0...24*3600)  の範囲に丸める
//. rad2mod1(rad)  - ラジアン を [0...2π)      の範囲に丸める
//. dd2mod2(dd)    - 度       を [-180...180)   の範囲に丸める
//. ds2mod2(ds)    - 度秒     を [-180*3600...180*3600) の範囲に丸める
//. rad2mod2(rad)  - ラジアン を [-π...π)     の範囲に丸める
inline double dd2rad(double dd) { return dd * DD2RAD; }
inline double hh2rad(double hh) { return hh * HH2RAD; }
inline double ds2rad(double ds) { return ds * DS2RAD; }
inline double hs2rad(double hs) { return hs * HS2RAD; }

inline double rad2dd(double rad) { return rad * RAD2DD; }
inline double rad2hh(double rad) { return rad * RAD2HH; }
inline double rad2ds(double rad) { return rad * RAD2DS; }
inline double rad2hs(double rad) { return rad * RAD2HS; }

inline double dd2mod1(double dd)   { return fmod1(dd, 360); }
inline double ds2mod1(double ds)   { return fmod1(ds, 360*3600L); }
inline double hh2mod1(double hh)   { return fmod1(hh, 24); }
inline double hs2mod1(double hs)   { return fmod1(hs, 24*3600L); }
inline double rad2mod1(double rad) { return fmod1(rad, 2*PI); }

inline double dd2mod2(double dd)   { return fmod2(dd, 360); }
inline double ds2mod2(double ds)   { return fmod2(ds, 360*3600L); }
inline double rad2mod2(double rad) { return fmod2(rad, 2*PI); }

//------------------------------------------------------------------------
//.----- class asXxxx : ダミー型 -----------------------------------------
//------------------------------------------------------------------------
// コンストラクタに渡すダミー型。別の引数で渡す数値型の意味付けをする。
class asRadian {};
class asHh {};
class asHm {};
class asHs {};
class asDs {};
class asDm {};
class asDd {};

//------------------------------------------------------------------------
/// 角度クラス.
class Degree {
	/// 角秒[″]  １°の3600倍.
	double ds;

public:
	//----- コンストラクタ -------------------------------------------
	Degree()
		: ds(0) {}

	explicit Degree(double ds)
		: ds(ds) {}

	Degree(double x, asRadian) { setRadian(x); }
	Degree(double x, asHh)  { setHh(x); }
	Degree(double x, asHm)  { setHm(x); }
	Degree(double x, asHs)  { setHs(x); }
	Degree(double x, asDd)  { setDegree(x); }
	Degree(double x, asDm)  { setMinute(x); }
	Degree(double x, asDs)  { setSec(x);    }

	Degree(double d, double m, double s)
		: ds(d * 3600 + m * 60 + s) { }

	Degree(const Degree& a)
		: ds(a.ds) {}

	Degree& operator=(const Degree& a) {
		ds = a.ds; return *this;
	}

	//----- 負数 -----------------------------------------------------
	friend Degree operator-(const Degree& a) {
		return Degree(-a.ds);
	}

	//----- 加減算 ---------------------------------------------------
	Degree& operator+=(const Degree& a) {
		ds += a.ds; return *this;
	}
	Degree& operator-=(const Degree& a) {
		ds -= a.ds; return *this;
	}
	friend Degree operator+(const Degree& a, const Degree& b) {
		return Degree(a.ds + b.ds);
	}
	friend Degree operator-(const Degree& a, const Degree& b) {
		return Degree(a.ds - b.ds);
	}

	//----- 係数乗除算 -----------------------------------------------
	Degree& operator*=(double k) {
		ds *= k; return *this;
	}
	Degree& operator/=(double k) {
		ds /= k; return *this;
	}
	friend Degree operator*(const Degree& a, double k) {
		return Degree(a.ds * k);
	}
	friend Degree operator*(double k, const Degree& a) {
		return Degree(a.ds * k);
	}
	friend Degree operator/(const Degree& a, double k) {
		return Degree(a.ds / k);
	}

	//----- 関係演算 -------------------------------------------------
	bool operator==(const Degree& a) const { return ds == a.ds; }
	bool operator!=(const Degree& a) const { return ds != a.ds; }
	bool operator< (const Degree& a) const { return ds <  a.ds; }
	bool operator<=(const Degree& a) const { return ds <= a.ds; }
	bool operator> (const Degree& a) const { return ds >  a.ds; }
	bool operator>=(const Degree& a) const { return ds >= a.ds; }

	//----- 正負判定 -------------------------------------------------
	// 0以上の判定は !isMinus()で可能.
	// 0以下の判定は !isPlus()で可能.
	bool isZero() const  { return ds == 0; }
	bool isPlus() const  { return ds > 0; } // 0より大
	bool isMinus() const { return ds < 0; } // 0より小

	//----- 符号反転、逆数 -------------------------------------------
	void setNeg() { ds = -ds; }
	void setInv() { ds = 1 / ds; }

	//----- 角度の丸め -----------------------------------------------
	/// 360度の範囲で角度を丸め、 [0°...360°) とする
	void mod360() { ds = fmod1(ds, 360*3600L); }

	/// ±180度の範囲で角度を丸め、[-180°...180°) とする
	void mod180() { ds = fmod2(ds, 360*3600L); }

	/// ±180度の範囲で角度を丸め、さらに±90度以上は角度を折り返してtrueを返す.
	/// @attention 緯度に対してtrueが返るときは、経度に180度加える必要がある
	bool mod90() {
		mod180();
		if (ds > 90*3600L) {
			ds =  180*3600L - ds; return true;
		}
		else if (ds < -90*3600L) {
			ds = -180*3600L - ds; return true;
		}
		else
			return false;
	}

	//----- 角度の定数加算 -------------------------------------------
	void add180() { ds += 180 * 3600L; }
	void sub180() { ds -= 180 * 3600L; }
	void add90()  { ds +=  90 * 3600L; }
	void sub90()  { ds -=  90 * 3600L; }

	//----- 角度取得・設定 -------------------------------------------
	/// 角度 [°]
	double degree() const    { return ds / 3600; }
	void setDegree(double d) { ds = d * 3600; }

	/// 角度分 [′]
	double minute() const    { return ds / 60; }
	void setMinute(double m) { ds = m * 60; }

	/// 角度秒 [″]
	double sec() const      { return ds; }
	void setSec(double s)   { ds = s; }

	/// 時角時 [h]
	double hh() const       { return ds / (15*3600L); }
	void setHh(double h)    { ds =   h  * (15*3600L); }

	/// 時角分 [m]
	double hm() const       { return ds / (15*60); }
	void setHm(double m)    { ds =   m  * (15*60); }

	/// 時角秒 [s]
	double hs() const           { return ds / 15; }
	void setHs(double s)        { ds =    s * 15; }

	/// ラジアン
	double radian() const       { return ds * DS2RAD; }
	void setRadian(double rad)  { ds =  rad * RAD2DS; }

	//----- 分秒を分離して設定・取得する -----------------------------
	/// 度分秒
	void setDms(double d, double m, double s) {
		ds = d * 3600 + m * 60 + s;
	}
	void getDms(char& sign, double& d, double& m, double& s) const {
		sec2ims(ds, sign, d, m, s);
	}
	void getDms(char& sign, int& d, int& m, double& s) const {
		sec2ims(ds, sign, d, m, s);
	}
	void getDms(char& sign, int& d, int& m, double& s, int point) const {
		sec2ims(round(ds, point), sign, d, m, s);
	}
	void getDm(char& sign, double& d, double& m) const {
		min2im(minute(), sign, d, m);
	}
	void getDm(char& sign, int& d, double& m) const {
		min2im(minute(), sign, d, m);
	}
	void getDm(char& sign, int& d, double& m, int point) const {
		min2im(round(minute(), point), sign, d, m);
	}

	/// 時分秒（時角)
	void setHms(double h, double m, double s) {
		ds = (h * 3600 + m * 60 + s) * 15;
	}
	void getHms(char& sign, double& h, double& m, double& s) const {
		sec2ims(hs(), sign, h, m, s);
	}
	void getHms(char& sign, int& h, int& m, double& s) const {
		sec2ims(hs(), sign, h, m, s);
	}
	void getHms(char& sign, int& h, int& m, double& s, int point) const {
		sec2ims(round(hs(), point), sign, h, m, s);
	}
	void getHm(char& sign, double& h, double& m) const {
		min2im(hm(), sign, h, m);
	}
	void getHm(char& sign, int& h, double& m) const {
		min2im(hm(), sign, h, m);
	}
	void getHm(char& sign, int& h, double& m, int point) const {
		min2im(round(hm(), point), sign, h, m);
	}

	//----- 文字列化 -------------------------------------------------
	ostream& printDegree(ostream& o) const;
	ostream& printRadian(ostream& o) const;
	ostream& printDms(ostream& o) const;
	ostream& printHms(ostream& o) const;
	friend ostream& operator<<(ostream& o, const Degree& a) {
		return a.printDegree(o);
	}
	int sprintDms(char* buf, const char* fmt, int point=0) const; // "%c%02dd %02dm %02.0fs"
	int sprintDm (char* buf, const char* fmt, int point=1) const; // "%c%02dd %04.1fm"
	int sprintHms(char* buf, const char* fmt, int point=0) const; // "%02dh %02dm %02.0fs"
	int sprintHm (char* buf, const char* fmt, int point=1) const; // "%02dh %04.1fm"

	//----- 文字列解析 -----------------------------------------------
	static Degree parseDms(const char* p);
	static Degree parseHms(const char* p);

	//----- 三角関数 -------------------------------------------------
	friend double sin(const Degree& a)   { return ::sin(a.radian()); }
	friend double cos(const Degree& a)   { return ::cos(a.radian()); }
	friend double tan(const Degree& a)   { return ::tan(a.radian()); }
	void setArcSin(double x)             { setRadian(asin(x)); }
	void setArcCos(double x)             { setRadian(acos(x)); }
	void setArcTan(double x)             { setRadian(atan(x)); }
	void setArcTan2(double y, double x)  { setRadian(atan2(y, x)); }
};//endclass Degree

/// 緯度経度の値を、緯度±90度、経度±180度に正規化する
inline void normLtLg(Degree& lt, Degree& lg) {
	if (lt.mod90())
		lg.add180();
	lg.mod180();
}

/// 赤経赤緯の値を、赤緯±90度、赤経0～360度に正規化する
inline void normRaDec(Degree& ra, Degree& dec) {
	if (dec.mod90())
		ra.add180();
	ra.mod360();
}

}//endnamespace util
#endif // DEGREE_H_
