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
#ifdef M_PI
constexpr double PI = M_PI;
#else
constexpr double PI = 3.14159265358979323846;
#endif
constexpr double DD2RAD = PI / 180;
constexpr double HH2RAD = PI /  12;
constexpr double DS2RAD = PI / (180*3600L);
constexpr double HS2RAD = PI / ( 12*3600L);
constexpr double RAD2DD = 180 / PI;
constexpr double RAD2HH = 12  / PI;
constexpr double RAD2DS = (180*3600L) / PI;
constexpr double RAD2HS = ( 12*3600L) / PI;

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
inline constexpr double dms2dd(double d, double m=0, double s=0) {
	return d + m / 60 + s / 3600;
}
/// 度分秒を秒単位の角度にする.
/// example: dms2ds(1, 30, 36) => 5436
/// @param d [in] 度
/// @param m [in] 分
/// @param s [in] 秒
/// @return 度*3600 + 分*60 + 秒
/// @attention d,m,sは正の値であること
inline constexpr double dms2ds(double d, double m=0, double s=0) {
	return d * 3600 + m * 60 + s;
}
/// 度分秒をラジアン角にする.
/// example: dms2rad(1, 30, 36) => 5436 * π / (180*3600)
/// @param d [in] 度
/// @param m [in] 分
/// @param s [in] 秒
/// @return (度*3600 + 分*60 + 秒) * π / (180*3600)
/// @attention d,m,sは正の値であること
inline constexpr double dms2rad(double d, double m=0, double s=0) {
	return dms2ds(d, m, s) * DS2RAD;
}

//------------------------------------------------------------------------
/// 時分秒を少数付き時間数にする.
/// example: hms2hh(1, 30, 36) => 1.51
/// @param h [in] 時
/// @param m [in] 分
/// @param s [in] 秒
/// @return 時 + 分/60 + 秒/3600
/// @attention h,m,sは正の値であること
inline constexpr double hms2hh(double h, double m=0, double s=0) {
	return h + m / 60 + s / 3600;
}
/// 時分秒を秒数にする.
/// example: hms2hs(1, 30, 36) => 5436
/// @param h [in] 時
/// @param m [in] 分
/// @param s [in] 秒
/// @return 時*3600 + 分*60 + 秒
/// @attention h,m,sは正の値であること
inline constexpr double hms2hs(double h, double m=0, double s=0) {
	return h * 3600 + m * 60 + s;
}
/// 時分秒をラジアン角にする.
/// example: hms2rad(1, 30, 36) => 5436 * π / (12*3600)
/// @param h [in] 時
/// @param m [in] 分
/// @param s [in] 秒
/// @return (時*3600 + 分*60 + 秒) * π / (12*3600)
/// @attention h,m,sは正の値であること
inline constexpr double hms2rad(double h, double m=0, double s=0) {
	return hms2hs(h, m, s) * HS2RAD;
}

//------------------------------------------------------------------------
/// 角度 を ラジアン角 にする.
inline constexpr double dd2rad(double dd) { return dd * DD2RAD; }
/// 時角 を ラジアン角 にする.
inline constexpr double hh2rad(double hh) { return hh * HH2RAD; }
/// 秒単位の角度 を ラジアン角 にする.
inline constexpr double ds2rad(double ds) { return ds * DS2RAD; }
/// 秒単位の時角 を ラジアン角 にする.
inline constexpr double hs2rad(double hs) { return hs * HS2RAD; }

//------------------------------------------------------------------------
/// ラジアン角 を 角度 にする.
inline constexpr double rad2dd(double rad) { return rad * RAD2DD; }
/// ラジアン角 を 時角 にする.
inline constexpr double rad2hh(double rad) { return rad * RAD2HH; }
/// ラジアン角 を 秒単位の角度 にする.
inline constexpr double rad2ds(double rad) { return rad * RAD2DS; }
/// ラジアン角 を 秒単位の時角 にする.
inline constexpr double rad2hs(double rad) { return rad * RAD2HS; }

//------------------------------------------------------------------------
/// 角度 を [0...360) の範囲に丸める.
inline double dd2mod1(double dd)   { return fmod1(dd, 360); }
/// 秒単位の角度 を [0...360*3600) の範囲に丸める.
inline double ds2mod1(double ds)   { return fmod1(ds, 360*3600L); }
/// 時間数 を [0...24) の範囲に丸める.
inline double hh2mod1(double hh)   { return fmod1(hh, 24); }
/// 秒数 を [0...24*3600) の範囲に丸める.
inline double hs2mod1(double hs)   { return fmod1(hs, 24*3600L); }
/// ラジアン角 を [0...2π) の範囲に丸める.
inline double rad2mod1(double rad) { return fmod1(rad, 2*PI); }

//------------------------------------------------------------------------
/// 角度 を [-180...180) の範囲に丸める.
inline double dd2mod2(double dd)   { return fmod2(dd, 360); }
/// 秒単位の角度 を [-180*3600...180*3600) の範囲に丸める.
inline double ds2mod2(double ds)   { return fmod2(ds, 360*3600L); }
/// ラジアン角 を [-π...π) の範囲に丸める.
inline double rad2mod2(double rad) { return fmod2(rad, 2*PI); }

//------------------------------------------------------------------------
/// 角度クラス.
class Degree {
	/// 秒単位の角度[″]  １°の3600倍.
	double _ds = 0;

public:
	//----- コンストラクタ -------------------------------------------
	/// デフォルトコンストラクタ.
	Degree() = default;

	/// コンストラクタ - 秒単位の角度[″]で初期化.
	/// @param ds 秒単位の角度[″]
	explicit Degree(double ds)
		: _ds(ds) {}

	/// コンストラクタ - 度分秒で初期化.
	/// @param d 度
	/// @param m 分
	/// @param s 秒
	Degree(double d, double m, double s)
		: _ds(d * 3600 + m * 60 + s) { }

	/// 静的コンストラクタ - 指定の時角で初期化.
	/// @param h 時
	/// @param m 分
	/// @param s 秒
	static Degree asHourAngle(double h, double m, double s) {
		return Degree((h * 3600 + m * 60 + s)  * 15);
	}

	/// 静的コンストラクタ - 指定の角度で初期化.
	/// @param rad ラジアン角
	static Degree asRadian(double rad) {
		return Degree(rad * RAD2DS);
	}

	/// コピーコンストラクタ.
	/// @param a コピー元の角度
	Degree(const Degree& a) = default;

	/// コピー代入.
	/// @param a 代入する角度
	/// @return 代入後の*this
	Degree& operator=(const Degree& a) = default;

	//----- 四則演算 -------------------------------------------------
	/// 単項マイナス.
	/// @param a 元の角度
	/// @return 負にした角度
	friend Degree operator-(const Degree& a) {
		return Degree(-a._ds);
	}
	/// 自己加算.
	Degree& operator+=(const Degree& a) {
		_ds += a._ds; return *this;
	}
	/// 自己減算.
	Degree& operator-=(const Degree& a) {
		_ds -= a._ds; return *this;
	}
	friend Degree operator+(const Degree& a, const Degree& b) {
	/// 加算.
		return Degree(a._ds + b._ds);
	}
	/// 減算.
	friend Degree operator-(const Degree& a, const Degree& b) {
		return Degree(a._ds - b._ds);
	}
	/// 自己係数乗算.
	Degree& operator*=(double k) {
		_ds *= k; return *this;
	}
	/// 自己係数除算.
	Degree& operator/=(double k) {
		_ds /= k; return *this;
	}
	/// 係数乗算.
	friend Degree operator*(const Degree& a, double k) {
		return Degree(a._ds * k);
	}
	/// 係数乗算.
	friend Degree operator*(double k, const Degree& a) {
		return Degree(a._ds * k);
	}
	/// 係数除算.
	friend Degree operator/(const Degree& a, double k) {
		return Degree(a._ds / k);
	}

	//----- 関係演算 -------------------------------------------------
	bool operator==(const Degree& a) const { return _ds == a._ds; } ///< 等値判定.
	bool operator!=(const Degree& a) const { return _ds != a._ds; } ///< 不等値判定.
	bool operator< (const Degree& a) const { return _ds <  a._ds; } ///< 小なり判定.
	bool operator<=(const Degree& a) const { return _ds <= a._ds; } ///< 以下判定.
	bool operator> (const Degree& a) const { return _ds >  a._ds; } ///< 大なり判定.
	bool operator>=(const Degree& a) const { return _ds >= a._ds; } ///< 以上判定.

	//----- 正負判定 -------------------------------------------------
	bool isZero() const  { return _ds == 0; } ///< 0と等しいか?
	bool isPlus() const  { return _ds > 0;  } ///< 0より大か? @note 0以下の判定は !isPlus()で可能.
	bool isMinus() const { return _ds < 0;  } ///< 0より小か? @note 0以上の判定は !isMinus()で可能.

	//----- 符号反転、逆数 -------------------------------------------
	void setNeg() { _ds = -_ds; }     ///< 符号反転する.
	void setInv() { _ds = 1 / _ds; }  ///< 逆数にする. @attention 0の場合はゼロ除算発生.

	//----- 角度の丸め -----------------------------------------------
	/// 360度の範囲で角度を丸め、 [0°...360°) とする
	void mod360() { _ds = fmod1(_ds, 360*3600L); }

	/// ±180度の範囲で角度を丸め、[-180°...180°) とする
	void mod180() { _ds = fmod2(_ds, 360*3600L); }

	/// ±180度の範囲で角度を丸め、さらに±90度以上は角度を折り返してtrueを返す.
	/// @attention 緯度に対してtrueが返るときは、経度に180度加える必要がある
	bool mod90() {
		mod180();
		if (_ds > 90*3600L) {
			_ds =  180*3600L - _ds; return true;
		}
		else if (_ds < -90*3600L) {
			_ds = -180*3600L - _ds; return true;
		}
		else
			return false;
	}

	//----- 角度の定数加算 -------------------------------------------
	void add180() { _ds += 180 * 3600L; } ///< 180度を加算する.
	void sub180() { _ds -= 180 * 3600L; } ///< 180度を減算する.
	void add90()  { _ds +=  90 * 3600L; } ///< 90度を加算する.
	void sub90()  { _ds -=  90 * 3600L; } ///< 90度を減算する.

	//----- 角度取得・設定 -------------------------------------------
	/// 角度 [°].         @see setDegree
	double degree() const    { return _ds / 3600; }
	void setDegree(double d) { _ds =   d  * 3600; }	///< 角度設定. @param d 設定する @copybrief degree @see degree

	/// 分単位の角度 [′]. @see setMinute
	double minute() const    { return _ds / 60; }
	void setMinute(double m) { _ds =   m  * 60; }	///< 角度設定. @param m 設定する @copybrief minute @see minute

	/// 秒単位の角度 [″]. @see setSec
	double sec() const      { return _ds; }
	void setSec(double s)   { _ds = s; }				///< 角度設定. @param s 設定する @copybrief sec @see sec

	/// 時角 [h].         @see setHh
	double hh() const       { return _ds / (15*3600L); }
	void setHh(double h)    { _ds =   h  * (15*3600L); }	///< 時角設定. @param h 設定する @copybrief hh @see hh

	/// 分単位の時角 [m]. @see setHm
	double hm() const       { return _ds / (15*60); }
	void setHm(double m)    { _ds =   m  * (15*60); }	///< 時角設定. @param m 設定する @copybrief hm @see hm

	/// 秒単位の時角 [s]. @see setHs
	double hs() const       { return _ds / 15; }
	void setHs(double s)    { _ds =   s  * 15; }	///< 時角設定. @param s 設定する @copybrief hs @see hs

	/// ラジアン角. @see setRadian
	double radian() const       { return _ds * DS2RAD; }
	void setRadian(double rad)  { _ds =  rad * RAD2DS; } ///< ラジアン角設定. @param rad 設定するラジアン角 @see radian

	//----- 角度の度分秒を設定・取得する -----------------------------
	/// 度分秒で角度を設定する.
	/// @param [in] d,m,s 設定する度分秒.
	void setDms(double d, double m, double s) {
		_ds = d * 3600 + m * 60 + s;
	}
	/// 度分秒で角度を取得する.
	/// @param [out] sign,d,m,s 符号と度分秒の取得先.
	void getDms(char& sign, double& d, double& m, double& s) const {
		sec2ims(_ds, sign, d, m, s);
	}
	/// 度分秒で角度を取得する.
	/// @param [out] sign,d,m,s 符号と度分秒の取得先.
	void getDms(char& sign, int& d, int& m, double& s) const {
		sec2ims(_ds, sign, d, m, s);
	}
	/// 度分秒で角度を取得する.
	/// @param [out] sign,d,m,s 符号と度分秒の取得先.
	/// @param [in] point 秒の小数部をpoint桁に丸める.
	void getDms(char& sign, int& d, int& m, double& s, int point) const {
		sec2ims(round(_ds, point), sign, d, m, s);
	}
	/// 度分で角度を取得する.
	/// @param [out] sign,d,m 符号と度分の取得先.
	void getDm(char& sign, double& d, double& m) const {
		min2im(minute(), sign, d, m);
	}
	/// 度分で角度を取得する.
	/// @param [out] sign,d,m 符号と度分の取得先.
	void getDm(char& sign, int& d, double& m) const {
		min2im(minute(), sign, d, m);
	}
	/// 度分で角度を取得する.
	/// @param [out] sign,d,m 符号と度分秒の取得先.
	/// @param [in] point 分の小数部をpoint桁に丸める.
	void getDm(char& sign, int& d, double& m, int point) const {
		min2im(round(minute(), point), sign, d, m);
	}

	//----- 時角の時分秒を設定・取得する -----------------------------
	/// 時分秒で時角を設定する.
	/// @param [in] h,m,s 設定する時分秒.
	void setHms(double h, double m, double s) {
		_ds = (h * 3600 + m * 60 + s) * 15;
	}
	/// 時分秒で時角を取得する.
	/// @param [out] sign,h,m,s 符号と時分秒の取得先.
	void getHms(char& sign, double& h, double& m, double& s) const {
		sec2ims(hs(), sign, h, m, s);
	}
	/// 時分秒で時角を取得する.
	/// @param [out] sign,h,m,s 符号と時分秒の取得先.
	void getHms(char& sign, int& h, int& m, double& s) const {
		sec2ims(hs(), sign, h, m, s);
	}
	/// 時分秒で時角を取得する.
	/// @param [out] sign,h,m,s 符号と時分秒の取得先.
	/// @param [in] point 秒の小数部をpoint桁に丸める.
	void getHms(char& sign, int& h, int& m, double& s, int point) const {
		sec2ims(round(hs(), point), sign, h, m, s);
	}
	/// 時分で時角を取得する.
	/// @param [out] sign,h,m 符号と時分の取得先.
	void getHm(char& sign, double& h, double& m) const {
		min2im(hm(), sign, h, m);
	}
	/// 時分で時角を取得する.
	/// @param [out] sign,h,m 符号と時分の取得先.
	void getHm(char& sign, int& h, double& m) const {
		min2im(hm(), sign, h, m);
	}
	/// 時分で時角を取得する.
	/// @param [out] sign,h,m 符号と時分の取得先.
	/// @param [in] point 分の小数部をpoint桁に丸める.
	void getHm(char& sign, int& h, double& m, int point) const {
		min2im(round(hm(), point), sign, h, m);
	}

	//----- 文字列化 -------------------------------------------------
	ostream& printDegree(ostream& o) const; ///< 角度を出力する.         例: "180.5"
	ostream& printRadian(ostream& o) const; ///< ラジアン角を出力する.   例: "3.150319" (180.5/180*π)
	ostream& printDms(ostream& o) const;    ///< 角度の度分秒を出力する. 例: "+180d30m0s"
	ostream& printHms(ostream& o) const;    ///< 時角の時分秒を出力する. 例: "+12h30m0s"
	friend ostream& operator<<(ostream& o, const Degree& a) { /// 度の実数値を出力するストリーム出力演算子.
		return a.printDegree(o);
	}
	int sprintDms(char* buf, const char* fmt, int point=0) const; ///< 角度をmod180で正規化して度分秒を書式展開する. @param fmt 書式. nullptr なら "%c%02dd %02dm %02.0fs"
	int sprintDm (char* buf, const char* fmt, int point=1) const; ///< 角度をmod180で正規化して度分を書式展開する.   @param fmt 書式. nullptr なら "%c%02dd %04.1fm"
	int sprintHms(char* buf, const char* fmt, int point=0) const; ///< 時角をmod360で正規化して時分秒を書式展開する. @param fmt 書式. nullptr なら "%02dh %02dm %02.0fs"
	int sprintHm (char* buf, const char* fmt, int point=1) const; ///< 時角のmod360で正規化して時分を書式展開する.   @param fmt 書式. nullptr なら "%02dh %04.1fm"

	//----- 文字列解析 -----------------------------------------------
	/// 度分秒の表記を解析して角度にして返す.
	/// 表記の解析パターン: 空白と省略可能な符号"+"/"-" に続いて "DD?MM'SS" or "DDdMMmSS" or "DD MM SS" or "DD?MM" or "DDMMSS" or "DD.DDD"
	/// @param [in] p 解析文字列バッファ.
	static Degree parseDms(const char* p);
	/// 度分秒の表記を解析して角度にして返す.
	/// 表記の解析パターン: 空白と省略可能な符号"+"/"-" に続いて "HH:MM:SS" or "HHhMMmSS" or "HH MM SS" or "HH?MM" or "HHMMSS" or "HH.HHH"
	/// @param [in] p 解析文字列バッファ.
	static Degree parseHms(const char* p);

	//----- 三角関数 -------------------------------------------------
	friend double sin(const Degree& a)   { return std::sin(a.radian()); }	///< sin関数のオーバロード. @return sin(a)
	friend double cos(const Degree& a)   { return std::cos(a.radian()); }	///< cos関数のオーバロード. @return cos(a)
	friend double tan(const Degree& a)   { return std::tan(a.radian()); }	///< tan関数のオーバロード. @return tan(a)
	void setArcSin(double x)             { setRadian(std::asin(x)); }		///< *this = arcsin(x)
	void setArcCos(double x)             { setRadian(std::acos(x)); }		///< *this = arccos(x
	void setArcTan(double x)             { setRadian(std::atan(x)); }		///< *this = arctan(x)
	void setArcTan2(double y, double x)  { setRadian(std::atan2(y, x)); }	///< *this = atan2(y,x)
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

//------------------------------------------------------------------------
/// 時角クラス.
class HourAngle : public Degree {
public:
	//----- コンストラクタ -------------------------------------------
	/// コンストラクタ - 時角で初期化.
	/// @param h 時
	/// @param m 分
	/// @param s 秒
	HourAngle(double h, double m, double s)
		: Degree((h * 3600 + m * 60 + s)  * 15) {}

	/// コンストラクタ - 秒単位の時角で初期化.
	/// @param hs 秒単位の時角
	HourAngle(double hs)
		: Degree(hs * 15) {}
};//endclass HourAngle

//------------------------------------------------------------------------
/// ラジアン角クラス.
class Radian : public Degree {
public:
	//----- コンストラクタ -------------------------------------------
	/// コンストラクタ - ラジアン角で初期化.
	/// @param rad ラジアン角
	Radian(double rad)
		: Degree(rad * RAD2DS) {}
};//endclass Radian

}//endnamespace util
#endif // DEGREE_H_
