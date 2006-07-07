//. jday.h - ユリウス日(暦日)
//	Copyright (C) 1997,1998, 2000 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef JDAY_H_
#define JDAY_H_
#include "defs.h"
namespace astro {
using namespace std;

//------------------------------------------------------------------------
//.----- class Jday : ユリウス日(暦日) -----------------------------------
//------------------------------------------------------------------------
// ユリウス日クラス
//	ユリウス暦、グレゴリオ暦に対応している
//@	陰暦や節気については派生クラスで対応を予定
//
// 天文学的には、ユリウス日や準ユリウス日は世界時(経度0°における時刻)
// で扱うものである。また一日内の時刻も小数として含めて扱うものである。
// しかしながら、このような天文学上の定義を厳密にあてはめて計算を行う
// と、時差や日付変更線も考慮しなければならず使い勝手が悪いので、
// 本クラスで扱うユリウス日は、日の単位で扱い、時刻系も世界時・地方時
// を問わず、とある時刻系における経過日数を測るためのものとした。
// 天文学的な計算を行う場合には、世界時換算や小数部の付加をする必要がある。
//
//	計算可能範囲は ±INT_MAX年であるが、数千万年を超える範囲を計算しても
//	実質的な意味はあまりない。なぜなら地球の自転速度はわずかであるが
//	年々遅くなっているため１年の日数は遥かな未来では少なくなり、遥か
//	な過去では多くなる。よって現代の１年の日数をもとにした暦を適用
//	できないからである。
/*
----- ノート -------------------------------------------------------------
グレゴリオ暦(Gregorius calendar)：
	現行の太陽暦。400年間に97回の閏年がある。
	4で割り切れ、100で割り切れない年を閏年とし、2月29日を設ける。
	400で割り切れる年も閏年とする。
	ローマ教皇グレゴリウス13世が、ユリウス暦1582年10月4日の翌日を、
	グレゴリオ暦1582年10月15日として実施した。
	日本では、太陰太陽暦の明治5年12月2日の翌日を明治6(1873)年1月1日
	として実施した。

ユリウス暦(Julian calendar)：
	4で割り切れる年を閏年とする暦法。
	ローマのジュリアス・シーザーが制定し、BC 46 から実施した。

ユリウス日(Julian day)：
	ユリウス暦 BC 4713(-4712)年1月1日正午(世界時)から起算した通日。
	1582年にグレゴリオ暦へ改暦する際に導入された。年代学において、
	各種の暦に依存しないで日付を統一できて便利である。
	日の始まりが世界時の正午(日本時間の21時)であることに注意すること。
	正午以前はマイナス時間で計算するのが便利である。
	例：
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

準ユリウス日(Modified Julian day)：
	1858年11月17日0時(世界時)から起算した通日。
	1973年に国際天文学連合で採択された。
	現代ではユリウス日が７桁に達しており桁が大きくて計算に不便なので、
	2,400,000.5 を引いて、５桁に納まり、なおかつ世界時午前０時で日が
	変わるようにしたものである。
--------------------------------------------------------------------------
*/
class Jday {
	long d;			// ユリウス日

public:
	//----- 基準となる日付のユリウス日 -------------------------------
	enum {
		AD1900_1_1_12UT = 2415021L,
		AD2000_1_1_12UT = 2451545L,	// 天体暦の起算日、J2000.0
	};

	//----- コンストラクタ -------------------------------------------
	Jday(int year, int month, int day=1) {	// 日付は世界時正午のもの
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

	//----- 日の加減算 -----------------------------------------------
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
		return a.d - b.d; // 差の日数を返す
	}

	//----- 関係演算 -------------------------------------------------
	bool operator==(const Jday& a) const { return d == a.d; }
	bool operator!=(const Jday& a) const { return d != a.d; }
	bool operator< (const Jday& a) const { return d <  a.d; }
	bool operator<=(const Jday& a) const { return d <= a.d; }
	bool operator> (const Jday& a) const { return d >  a.d; }
	bool operator>=(const Jday& a) const { return d >= a.d; }

	//----- 年月日と曜日 (グレゴリオ暦) ------------------------------
	int year() const;
	int month() const;	// 1～12
	int day() const;	// 1～31
	int dayOfWeek() const;	// 0=日、1=月、2=火 ... 6=土

	//----- 日数 -----------------------------------------------------
	long jd() const {	// ユリウス日
		return d;
	}
	long mjd() const {	// 準ユリウス日
		return d - 2400000L;
	}

	//----- J2000.0 --------------------------------------------------
	// グレゴリオ暦の日時[世界時]から J2000.0 (2000年1月1日12:00UT)を元期とする
    // 経過日数、ユリウス年数、ユリウス世紀を求める
    // @param y, m, dd グレゴリオ暦の年月日
    // @return J2000.0 を元期とする経過時間
	static double jd2000(int y, int m, double dd) {	// 日数
		Jday w(y, m, 0);
		return w.jd() - AD2000_1_1_12UT + dd - 0.5;	// 正午が日の変わり目なので 0.5日補正する
	}
	static double jy2000(int y, int m, double dd) {	// ユリウス年数
		return jd2000(y, m, dd) / 365.25;
	}
	static double jc2000(int y, int m, double dd) {	// ユリウス世紀数
		return jd2000(y, m, dd) / 36525;
	}

	//----- 日付設定・取得(グレゴリオ暦) -----------------------------
	void getGdate(int& year, int& month, int& day, int& dayOfWeek) const;
	void setGdate(int year, int month, int day);

	//----- 日付設定・取得(ユリウス暦) -------------------------------
	void getJdate(int& year, int& month, int& day, int& dayOfWeek) const;
	void setJdate(int year, int month, int day);
};

}//endnamesapae astro
#endif // JDAY_H_
