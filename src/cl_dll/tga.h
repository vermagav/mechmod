/*******************************************************
*			TGA FILE HEADER
*
* credit to: Evan Pipho (Terminate) and his tut on nehe
* ported to hl by: jason "ssjason123" matson
* date: 9-21-03
*******************************************************/

#ifndef __TEXTURE_H__						// see if header has been defined yet
#define __TEXTURE_H__						// if not, Define it

#include <windows.h>
#include <stdio.h>
#include <gl\gl.h>
//#include <gl\glut.h>

typedef struct
{
	GLubyte *imageData;						// holds all color values for the tga
	GLuint	bpp;							// holds number of bits per pixel
	GLuint	width;							// width of the tga
	GLuint	height;							// height of the tga
	GLuint	texID;							// texture id for use with glBindTexture
	GLuint	type;							// data stored in *imagedata rgba or rgb
} Texture;

typedef struct
{
	GLubyte Header[12];						// file header to determie file type
} TGAHeader;

typedef struct
{
	GLubyte header[6];						// holds the first 6 useful bytes of the file
	GLuint	bytesPerPixel;					// number fo bytes per pixel 3 or 4
	GLuint	imageSize;						// amount of memory needed to hold the image
	GLuint	type;							// the type of image, rgb or rgba
	GLuint	Height;							// height of image
	GLuint	Width;							// width of image
	GLuint	Bpp;							// number of bits per pixil 24 or 32
} TGA;


class CTGA
{
public:
	TGAHeader	tgaheader;						// used to store our file header
	TGA			tga;							// used to store file info

	// uncompressed tga header
	GLubyte uTGAcompare[12];// = { 0,0, 2,0,0,0,0,0,0,0,0,0 };
	// compressed tga header
	GLubyte cTGAcompare[12];// = { 0,0,10,0,0,0,0,0,0,0,0,0 };

	void Init( void );	//set vars to what we need them to
	bool LoadTGA( Texture *texture, char *filename );
	// load an uncompressed file
	bool LoadUncompressedTGA( Texture *, char *, FILE * );
	// load a compressed file
	bool LoadCompressedTGA( Texture *, char *, FILE * );
private:
};

extern CTGA gTGA;

#endif										// __TEXTURE_H__ end inclusion guard