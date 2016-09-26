//. atime.h - 天文時刻
//	Copyright (C) 1997,1998,2015 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef ATIME_H_
#define ATIME_H_
#include "jday.h"

namespace astro {
using namespace std;

//------------------------------------------------------------------------
//.----- class AstroTime : 天文時刻 --------------------------------------
//------------------------------------------------------------------------
// ユリウス日、日の秒数を保持し、必要に応じて恒星時等を計算する
// ※ユリウス日は世界時12時に日付が変る。
// ※グリニジ基準天体(平均太陽)
//	太陽は地球の公転運動により天球の黄道上を一年でほぼ一周する。
//	天球は地球の自転運動により一日でほぼ一回転する。
//	日の出・日の入りを生活時間の基準としている我々は、天球上の
//	太陽が一回転するのに要する時間を一日としているが、太陽は
//	黄道上を進んでいるため、赤経方向でみると赤道と交わる春分秋分
//	付近では進みが遅く、夏至冬至付近では進みが早くなる。つまり
//	季節によって一日の長さが違ってしまう。これでは不便なので、
//	赤道上を一定速度で一年一周する仮想の太陽を、時刻の基準とする。
//	この仮想太陽の赤経が「平均太陽」である。
// ※世界時(UT0,UT1,UT2)
//	世界時は天体観測を元にして決定される。
//	天体位置観測から求めた地方平均恒星時(LMST)と、観測地の採用
//	経度λからUT0 を得る。
//	次に極運動による観測地の経度変化(極運動によるもの)補正Δλ
//	を加えて、観測地によらない世界共通の UT1 を得る。
//	極運動は自転軸が半径10mぐらいの範囲で変動する現象であり、
//	角度にすると±0.3″(時角で0.02s)ぐらいの変化となる。
//	さらに地球の自転速度変化の季節変動（潮汐力による地球の変形や、
//	偏西風の季節変化が原因とされる）ΔTsを加えて、秒の進みを平滑
//	化した UT2 を得る。1960年にUTCが登場するまでは、このUT2が日常
//	に用いる時間として利用されていた。
//	λ = 観測地の採用天文緯度
//	φ = 観測地の採用天文緯度
//	 t = 年初からの経過時間[年単位]
//	 x,y = 極運動による極の位置
//	Δλ = -(x sinλ + y cosλ) tanφ  (およそ ±0.3″tanφ)
//	ΔTs = 0.022″ain2πt - 0.012″cos2πt - 0.006″sin4πt + 0.007″cos4πt
//	αm = グリニジ基準天体(平均太陽)
//	UT0 = LMST - λ - αm + 12h
//	UT1 = UT0 + Δλ
//	UT2 = UT1 + ΔTs
// ※協定世界時(UTC)
//	国際原子時(TAI)に閏秒を入れ、地球の自転に基づく世界時(UT1)との
//	時刻差を ±0.9秒以内に管理した時間である。我々が日常使っている
//	時間である。
//	月末最終秒に閏秒(１秒)を追加するか抜くことで、時刻差を±0.9秒
//	以内調整する。その時期は、国際地球回転観測事業(IERS)の中央局が
//	決定する。
//	1976～1997の20年間で、合計 15 秒の閏秒が挿入されている。
//	1997年7月1日から1999年1月1日までは UTC - TAI = -31秒である。
// ※国際地球回転観測事業(IERS:International Earch Rotation Service)
//	世界各地の天文台において、UTCを基準として恒星の位置観測を行い
//	UT0 - UTC を求める。これをIERS中央局(フランス)が整理して、
//	極位置(x,y)、UT1 - UTC、UTC - TAIの確定値、速報値、予測値を
//	公表している。WWW上では http://hpiers.obspm.fr/ から公表値を
//	入手できる。
// ※DUT1
//	各国の標準時電波報時(日本ではJJY)が報じる UT1 - UTC の予測値
//	(0.1秒単位)のこと。UTCからUT1の予測値を求めるために使用する。
// ※グリニッジ平均恒星時(GMST)
//	GMSTとUT1の関係は以下の式で定められている。
//	出典：理科年表 世界時の項より
//	GMST = UT1 - 12h + αm
//	αm  = 18h 41m 50.54841s
//		+ 8640184.812866s Tu + 0.093104s Tu^2 - 0.0000062s Tu^3
//	Tu = 2000年1月1日12時(UT1)からユリウス世紀単位で測った世界時
//	日本標準時JST(UTC + 9h)から、精度の高いUT1を得るにはJJY等の
//	無線報時やIERSからDUT1(0.1秒単位)を得る必要がある。
// ※力学時(TD:Dynamical Time)
//	世界時(UT)は予測不可能な閏秒の調整が入るため一様な時刻系ではなく、
//	惑星の軌道計算等の天体力学で用いるには都合が悪い。
//	そこで時間経過が一様な時刻系として用意されたのが力学時である。
//	一般相対性理論から、空間位置によって時刻の進みは均質でないため、
//	太陽系重心を基準とした惑星の位置計算には、
//	「太陽系力学時(TDB:Barycentric Dynamical Time)」を用い、
//	地心視位置の計算には、
//	「地球力学時(TDT:Terrestrial Dynamical Time)」を用いる。
//	TDTは国際原子時(TAI)に対して、TDT = TAI + 32.184秒 と定められて
//	いる。TDTとTDBの差は ±0.002秒以内である。

class AstroTime {
	Jday d;		// ユリウス日 (世界時正午のユリウス日)
	double s;	// ユリウス日の秒数(0～86400.0) ※世界時正午が0
	double dut1;	// UT1 - UTC の値。0.1秒の精度。
	int leapSec;	// 閏秒、TAI - UTC の値。1997.7～1999.1までは +31秒

protected:
	//----- 秒が日を越えないようにする -------------------------------
	void adjust();

	//----- 各時刻系の日の秒数 ---------------------------------------
	// UT1[秒]         ※正午を0とする
	double ut1() const	{ return s + dut1; }

	// 国際原子時[秒]  ※正午を0とする
	double tai() const	{ return s + leapSec; }

	// 地球力学時[秒]  ※正午を0とする
	double tdt() const	{ return tai() + 32.184; }

public:
	//----- コンストラクタ -------------------------------------------
	// ※dut1は0.0、leapSecはinitLeapSecにする。
	//   厳密な計算のためには、setDUT1(),setLeapSec()で設定すること。

	// 閏秒の初期値
	static int initLeapSec;

	/// 閏秒の初期値説明.
	static const char* initLeapText;

	// 現在時刻で生成する
	AstroTime();

	// 指定の日付と時刻で生成する
	// @param jd  世界時正午の日付
	// @param utc 世界時０時からの経過秒数
	explicit AstroTime(const Jday& jday, double utc = 0);


	//----- 現在時刻に更新する ---------------------------------------
	// ※dut1とleapSecは変更しない
	//   厳密な計算のためには、setDUT1(),setLeapSec()で設定すること。
	void update();           // 秒単位の更新
	void updateSystemTime(); // ミリ秒単位の更新


	//----- 時刻の設定・取得 -----------------------------------------
	// ※dut1とleapSecは変更しない
	//   厳密な計算のためには、setDUT1(),setLeapSec()で設定すること。
	void set(const Jday& jday, double utc = 0) {
		d = jday; s = utc - 12*3600L; adjust();
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


	//----- 協定世界時からUT1への予測補正値設定 ----------------------
	void setDUT1(double _dut1) {
		dut1 = _dut1; // UT1 - UTC の値 [秒]
	}


	//----- 協定世界時から国際原子時への変換値(閏秒)設定 -------------
	void setLeapSec(int _leapSec) {
		leapSec = _leapSec; // 閏秒、TAI - UTC の値 [秒]
	}


	//----- 日と秒の加減算 -------------------------------------------
	void addSec(double n) { s += n; adjust(); }
	void addDay(long n)   { d += n; adjust(); }
	void subSec(double n) { s -= n; adjust(); }
	void subDay(long n)   { d -= n; adjust(); }

	//----- 関係演算 -------------------------------------------------
	bool operator==(const AstroTime& a) const { return d == a.d && s == a.s; }
	bool operator!=(const AstroTime& a) const { return d != a.d || s != a.s; }
	bool operator< (const AstroTime& a) const { return d < a.d || (d == a.d && s <  a.s); }
	bool operator<=(const AstroTime& a) const { return d < a.d || (d == a.d && s <= a.s); }
	bool operator> (const AstroTime& a) const { return d > a.d || (d == a.d && s >  a.s); }
	bool operator>=(const AstroTime& a) const { return d > a.d || (d == a.d && s >= a.s); }


	//----- 日の小数付きのユリウス日 ---------------------------------

	// ユリウス日[UTC]
	double jd() const {
		return d.jd() + s / 86400L;
	}

	// ユリウス日[UT1]
	double jd1() const {
		return d.jd() + ut1() / 86400L;
	}

	// J2000.0(力学時2000年1月1日12時)からの経過日数[TDT]
	// ※TDTとTDBの違い(±0.002秒)が問題にならないような精度の計算
	//   であれば、太陽系力学時(TDB)による経過日数として用いて良い。
	double j2000() const {
		return d.jd() - Jday::AD2000_1_1_12UT + tdt() / 86400L;
	}


	//----- 恒星時 ---------------------------------------------------
	// グリニジ平均恒星時[秒] [0～86400)
	double gmst() const;

};//endclass AstroTime

}//endnamespace astro
#endif // ATIME_H_
