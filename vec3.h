//. Vec3.h - 3D vector
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef VEC3_H_
#define VEC3_H_
#include "defs.h"

namespace util {
using namespace std;
class Degree;

//------------------------------------------------------------------------
//.----- class Mat3x3 ----------------------------------------------------
//------------------------------------------------------------------------
// １行３列のベクトルをアフィン変換する３行３列の行列である
//	加減乗算の他に、回転行列設定、単位行列設定ができる。
class Mat3x3 {
public:
	//----- コンストラクタ -------------------------------------------

	// 単位行列として生成する
	Mat3x3() {
		setE();
	}

	// 回転行列として生成する
	// @param angle 回転角度
	// @param axis  回転軸('X','Y','Z' のいずれかを指定する)
	Mat3x3(const Degree& angle, int axis='Z') {
		setRotate(angle, axis);
	}

	// 指定の3x3要素を持った行列として生成する
	Mat3x3(double m11, double m12, double m13,
	       double m21, double m22, double m23,
	       double m31, double m32, double m33)
		: m11(m11), m12(m12), m13(m13),
		  m21(m21), m22(m22), m23(m23),
		  m31(m31), m32(m32), m33(m33) {}


	//----- 単位行列設定 ---------------------------------------------
	void setE();


	//----- 回転行列設定 ---------------------------------------------
	// ベクトルの直交座標成分が、右手系(親指=x、人差し指=y、中指=z)のとき
	// 回転行列は、ベクトルを次のように回転する。
	//	・軸の＋方向から原点を見て、軸の回りを反時計回り(左ネジ)
	//	・原点から軸の＋方向を見て、軸の回りを時計回り(右ネジ)
	// ベクトルを固定して座標系の回転とみれば次のようになる。
	//	・軸の＋方向から原点を見て、軸の回りを時計回り(右ネジ)
	// @param angle 回転角度
	// @param axis  回転軸('X','Y','Z' のいずれかを指定する)
	void setRotate(const Degree& angle, int axis='Z');


	//----- 加減乗算 -------------------------------------------------
	Mat3x3& operator+=(const Mat3x3&);
	Mat3x3& operator-=(const Mat3x3&);
	Mat3x3& operator*=(const Mat3x3&);

	friend Mat3x3 operator+(const Mat3x3& a, const Mat3x3& b) {
		Mat3x3 c(a); return c += b;
	}

	friend Mat3x3 operator-(const Mat3x3& a, const Mat3x3& b) {
		Mat3x3 c(a); return c -= b;
	}

	friend Mat3x3 operator*(const Mat3x3& a, const Mat3x3& b) {
		Mat3x3 c(a); return c *= b;
	}


	//----- 係数乗除算 -----------------------------------------------
	Mat3x3& operator*=(double k);
	Mat3x3& operator/=(double k);

	friend Mat3x3 operator*(const Mat3x3& a, double k) {
		Mat3x3 c(a); return c *= k;
	}

	friend Mat3x3 operator*(double k, const Mat3x3& a) {
		Mat3x3 c(a); return c *= k;
	}

	friend Mat3x3 operator/(const Mat3x3& a, double k) {
		Mat3x3 c(a); return c /= k;
	}


	//----- 行列データメンバ -----------------------------------------
	// ※外部から直接変更されても困らないので、公開してしまう
	double m11, m12, m13,
	       m21, m22, m23,
	       m31, m32, m33;

};//endclass Mat3x3

// コンストラクタに渡すダミー型。別の引数で渡す数値型の意味付けをする。
class asLtLg {};

//------------------------------------------------------------------------
//.----- class Vec3 : １行３列のベクトル ---------------------------------
//------------------------------------------------------------------------
// １行３列の実数ベクトル。３次元直交座標を保持する
class Vec3 {
public:
	//----- コンストラクタ -------------------------------------------

	// 全て0で生成する
	Vec3()
		: x(0), y(0), z(0) {}

	// 直交座標値で生成する
	Vec3(double x, double y, double z)
		: x(x), y(y), z(z) {}

	// 極座標値で生成する
	// @param radius  動径ｒ
	// @param phi     天頂角φ
	// @param theta   方位角θ
	Vec3(double radius, const Degree& phi, const Degree& theta) {
		setPolar(radius, phi, theta);
	}

	// 緯度経度値で生成する
	// @param radius    動径ｒ
	// @param latitude  緯度
	// @param longitude 経度(東経を正とする)
	Vec3(double radius, const Degree& latitude, const Degree& longitude, asLtLg) {
		setLtLg(radius, latitude, longitude);
	}

	// 直交座標値の配列から生成する
	Vec3(const int*    p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const short*  p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const long*   p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const double* p) : x(p[0]), y(p[1]), z(p[2]) {}
	Vec3(const float*  p) : x(p[0]), y(p[1]), z(p[2]) {}


	//----- 加減算 ---------------------------------------------------
	Vec3& operator+=(const Vec3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vec3& operator-=(const Vec3& a) {
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	friend Vec3 operator+(const Vec3& a, const Vec3& b) {
		return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend Vec3 operator-(const Vec3& a, const Vec3& b) {
		return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
	}


	//----- 係数乗除算 -----------------------------------------------
	Vec3& operator*=(double k) {
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	Vec3& operator/=(double k) {
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	friend Vec3 operator*(const Vec3& a, double k) {
		return Vec3(a.x * k, a.y * k, a.z * k);
	}

	friend Vec3 operator*(double k, const Vec3& a) {
		return Vec3(a.x * k, a.y * k, a.z * k);
	}

	friend Vec3 operator/(const Vec3& a, double k) {
		return Vec3(a.x / k, a.y / k, a.z / k);
	}


	//----- 行列演算 -------------------------------------------------
	Vec3& operator*=(const Mat3x3& m) {
		//             | m11 m12 m13 |
		// (x, y, z) * | m21 m22 m23 |
		//             | m31 m32 m33 |
		double xx = x, yy = y, zz = z;
		x = xx * m.m11 + yy * m.m21 + zz * m.m31;
		y = xx * m.m12 + yy * m.m22 + zz * m.m32;
		z = xx * m.m13 + yy * m.m23 + zz * m.m33;
		return *this;
	}

	friend Vec3 operator*(const Vec3& a, const Mat3x3& b) {
		return Vec3(
			a.x * b.m11 + a.y * b.m21 + a.z * b.m31,
			a.x * b.m12 + a.y * b.m22 + a.z * b.m32,
			a.x * b.m13 + a.y * b.m23 + a.z * b.m33
		);
	}


	//----- 内積 -----------------------------------------------------
	// 内積値 = |a|*|b|*cosθ = a.x * b.x + a.y * b.y + a.z * b.z
	double inner(const Vec3& a) const {
		return x * a.x + y * a.y + z * a.z;
	}


	//----- 平方和 ---------------------------------------------------
	double square() const {
		return x * x + y * y + z * z;
	}


	//----- 方向余弦化 -----------------------------------------------
	void setDirectionCosines();


	//----- 極座標の設定・取得 ---------------------------------------

	// 動径、ベクトルの長さ
	double radius() const;

	// 極座標を得る
	// @param phi     天頂角φ
	// @param theta   方位角θ
	// @return        動径ｒ
	double getPolar(Degree& phi, Degree& theta) const;

	// 極座標を設定する
	// @param radius  動径ｒ
	// @param phi     天頂角φ
	// @param theta   方位角θ
	void setPolar(double radius, const Degree& phi, const Degree& theta);

	// 緯度経度を得る
	// @param latitude  緯度
	// @param longitude 経度(東経を正とする)
	// @return          動径ｒ
	double getLtLg(Degree& latitude, Degree& longitude) const;

	// 緯度経度を設定する
	// @param radius    動径ｒ
	// @param latitude  緯度
	// @param longitude 経度(東経を正とする)
	void setLtLg(double radius, const Degree& latitude, const Degree& longitude);


	//----- 直交座標値データメンバ -----------------------------------
	// ※外部から直接変更されても困らないので、公開してしまう
	double x, y, z;

};//endclass Vec3

}//endnamespace util
#endif // VEC3_H_
