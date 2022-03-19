/* Tells the compiler not to add padding for these structs. This may
   be useful when reading/writing to binary files.
   http://stackoverflow.com/questions/3318410/pragma-pack-effect
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Max_len_str_int 10
#define Fileheader_size_tokens 8
#define Infoheader_size_tokens 22
#define Len_str_bmpname 25
#define Len_str_jsonname 26
#define Suma_cifrelor 45

#pragma pack(1)

typedef struct 
{
    unsigned char  fileMarker1; /* 'B' */
    unsigned char  fileMarker2; /* 'M' */
    unsigned int   bfSize; /* File's size */
    unsigned short unused1; /* Aplication specific */
    unsigned short unused2; /* Aplication specific */
    unsigned int   imageDataOffset; /* Offset to the start of image data */
} bmp_fileheader;

typedef struct 
{
    unsigned int   biSize; /* Size of the info header - 40 bytes */
    signed int     width; /* Width of the image */
    signed int     height; /* Height of the image */
    unsigned short planes;
    unsigned short bitPix; /* Number of bits per pixel = 3 * 8 (for each channel R, G, B we need 8 bits */
    unsigned int   biCompression; /* Type of compression */
    unsigned int   biSizeImage; /* Size of the image data */
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} bmp_infoheader;

typedef struct pixel{
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Pixel;

#pragma pack()

typedef struct bitmap{
    int width;
    int height;
    Pixel **map;
} Bitmap;

typedef struct bmp_image{
    bmp_fileheader *file_header;
    bmp_infoheader *info_header;
    Bitmap *bitmap;
} Bmp_image;

typedef struct square{
    Pixel **map;
} Square;
