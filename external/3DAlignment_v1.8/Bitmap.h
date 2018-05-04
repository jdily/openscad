
#ifndef DWORD
#define WINAPI
typedef unsigned long DWORD;
typedef short WCHAR;
typedef void * HANDLE;
#define MAX_PATH	PATH_MAX
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int BOOL;
#endif

// typedef struct POINT_
// {
// 	int x, y;
// }POINT;

// struct BITMAPFILEHEADER
// {
//         WORD  bfType; // 2
//         DWORD bfSize; // 6
//         WORD  bfReserved1; // 8
//         WORD  bfReserved2; // 10
//         DWORD bfOffBits; // 14
// }; // should add to 14 bytes


#define uint16 unsigned short
#define uint32 unsigned long
#define uint8  unsigned char
#define uint64 unsigned long long
#pragma pack(1)
typedef struct 
{

 uint16 bfType;  //specifies the file type "BM" 0x424d
 uint32 bfSize;  //specifies the size in bytes of the bitmap file
 uint16 bfReserved1;  //reserved; must be 0
 uint16 bfReserved2;  //reserved; must be 0
 uint32 bfOffBits;  
} BITMAPFILEHEADER;
#pragma pack(0)  // restore normal structure packing rules

typedef struct {
  uint32 biSize;
  uint64  biWidth;
  uint64  biHeight;
  uint16  biPlanes;
  uint16  biBitCount;
  uint32 biCompression;
  uint32 biSizeImage;
  uint64  biXPelsPerMeter;
  uint64  biYPelsPerMeter;
  uint32 biClrUsed;
  uint32 biClrImportant;
} BITMAPINFOHEADER;

void WriteBitmap(unsigned char *Buff, int x, int y, char *filename);
void WriteBitmap8(unsigned char *sBuff, int x, int y, char *filename);
void WriteRAW(unsigned char *Buff, int x, int y, char *filename);
