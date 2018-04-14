

#pragma once

//===============
//   TYPES
//===============

enum
{
	CMP_NONE = 0,
	CMP_LZSS
};

enum
{
	TYP_NONE = 0,
	TYP_LABEL
};

#define TYP_LUMPY 64 // 64 + grab command number
#define TYP_PALETTE 64
#define TYP_QTEX 65
#define TYP_QPIC 66
#define TYP_SOUND 67
#define TYP_MIPTEX 68

struct qpic_t
{
	int32_t width{0}, height{0};
	byte data[4]{}; ///< variably sized
};

struct wadinfo_t
{
	char identification[4]{}; ///< should be WAD2 or 2DAW
	int32_t numlumps{0};
	int32_t infotableofs{0};
};


void W_CleanupName(const char *in, char *out);

void *W_GetLumpName(const char *name);
void *W_GetLumpNum(int num);

void SwapPic(qpic_t *pic);