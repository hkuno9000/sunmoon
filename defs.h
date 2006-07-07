//. defs.h - common, basic definitions
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef DEFS_H_
#define DEFS_H_
#include <stdexcept>

//------------------------------------------------------------------------
//. 環境スイッチ
//------------------------------------------------------------------------
#if (defined(__WIN32__) || defined(_WIN32)) && !defined(WIN32)
	// WIN32環境のスイッチを共通化する
	// BCBでは  __WIN32__
	// VC++では _WIN32
	#define WIN32
#endif

//------------------------------------------------------------------------
//. 記憶クラス属性
//------------------------------------------------------------------------
#if defined(_MSC_VER) || defined(__BORLANDC__)
	#define THREAD	__declspec(thread)
	#define EXPORT	__declspec(export)
	#define IMPORT	__declspec(import)
#else
	// DLLやスレッドをサポートしない環境では空マクロにしておく
	#define THREAD
	#define EXPORT
	#define IMPORT
#endif

//------------------------------------------------------------------------
//. signed/unsigned type alias
//------------------------------------------------------------------------
typedef unsigned int	uint;
typedef signed   char	schar;
typedef unsigned char	uchar;
typedef unsigned short	ushort;
typedef unsigned long	ulong;

//------------------------------------------------------------------------
//. bit-sized int
//------------------------------------------------------------------------
// ※処理系やターゲット環境によっては修正が必要である
typedef signed char	int8;
typedef short		int16;
typedef long		int32;
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint32;

//------------------------------------------------------------------------
//. 例外
//------------------------------------------------------------------------
class IOException : public std::runtime_error {
public: IOException(const char* msg) : std::runtime_error(msg) {}
};

class FileNotOpened : public IOException {
public: FileNotOpened(const char* msg) : IOException(msg) {}
};

class NullPointer : public std::logic_error {
public: NullPointer(const char* msg) : std::logic_error(msg) {}
};

#define NULLCHECK(p) { if (p == 0) throw NullPointer(""); }

//------------------------------------------------------------------------
//. macros
//------------------------------------------------------------------------

//. ELEMENTS_OF - 配列要素数の取得
#define ELEMENTS_OF(array) \
	(sizeof(array) / sizeof(array[0]))

#endif // DEFS_H_
