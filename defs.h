//. defs.h - common, basic definitions
//	Copyright (C) 1997,1998 hkuno
//	mailto:hkuno.kuno@nifty.ne.jp
#ifndef DEFS_H_
#define DEFS_H_
#include <stdexcept>

//------------------------------------------------------------------------
//. ���X�C�b�`
//------------------------------------------------------------------------
#if (defined(__WIN32__) || defined(_WIN32)) && !defined(WIN32)
	// WIN32���̃X�C�b�`�����ʉ�����
	// BCB�ł�  __WIN32__
	// VC++�ł� _WIN32
	#define WIN32
#endif

//------------------------------------------------------------------------
//. �L���N���X����
//------------------------------------------------------------------------
#if defined(_MSC_VER) || defined(__BORLANDC__)
	#define THREAD	__declspec(thread)
	#define EXPORT	__declspec(export)
	#define IMPORT	__declspec(import)
#else
	// DLL��X���b�h���T�|�[�g���Ȃ����ł͋�}�N���ɂ��Ă���
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
// �������n��^�[�Q�b�g���ɂ���Ă͏C�����K�v�ł���
typedef signed char	int8;
typedef short		int16;
typedef long		int32;
typedef unsigned char	uint8;
typedef unsigned short	uint16;
typedef unsigned long	uint32;

//------------------------------------------------------------------------
//. ��O
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

//. ELEMENTS_OF - �z��v�f���̎擾
#define ELEMENTS_OF(array) \
	(sizeof(array) / sizeof(array[0]))

#endif // DEFS_H_
