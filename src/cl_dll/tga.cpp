/*******************************************************
*					TGA FILE 
*
* credit to: Evan Pipho (Terminate) and his tut on nehe
* ported to hl by: jason "ssjason123" matson
* date: 9-21-03
*******************************************************/
#include "hud.h"
#include "cl_util.h"
#include "tga.h"

CTGA gTGA;

void CTGA::Init( void )
{
	GLubyte compare[12] = { 0,0, 2,0,0,0,0,0,0,0,0,0 };
	for( int i = 0; i < 12; i++ )
	{
		uTGAcompare[i] = compare[i];
	}
	// compressed tga header
	compare[2] = 10;
	for( i = 0; i< 12; i++ )
	{
		cTGAcompare[i] = compare[i];
	}
}

bool CTGA::LoadTGA( Texture *texture, char *filename )
{
	FILE *fTGA;
	//gEngfuncs.Con_Printf( "%s\n", filename );
	fTGA = fopen( filename, "rb" );

	if( fTGA == NULL )
	{
		//error do an alert
		//gEngfuncs.Con_Printf( "%s\n", filename );
		return false;
	}
	//gEngfuncs.Con_Printf( "%s\n", filename );

	//attempt to read the file header
	if( fread( &tgaheader, sizeof( TGAHeader ), 1, fTGA ) == 0 )
	{
		//error do an alert
		return false;
	}

	//check if file header matches the uncompressed header
	if( memcmp( uTGAcompare, &tgaheader, sizeof( tgaheader ) ) == 0 )
	{
		// load an uncompressed TGA
		LoadUncompressedTGA( texture, filename, fTGA );
	}
	// if the file header matches the compressed header
	else if( memcmp( cTGAcompare, &tgaheader, sizeof( tgaheader ) ) == 0 )
	{
		//load a compressed tga
		LoadCompressedTGA( texture, filename, fTGA );
	}
	else
	{
		//texture doesnt match valid tga types
		//also give an error
		gEngfuncs.Con_Printf( "Invalid Type\n" );
		return false;
	}

	return true;
}

// load an uncompressed tga
bool CTGA::LoadUncompressedTGA( Texture *texture, char *filename, FILE *fTGA )
{
	//attempt to read the next 6 bytes
	if( fread( tga.header, sizeof( tga.header ), 1, fTGA ) == 0 )
	{
		//send an error
		return false;
	}

	texture->width = tga.header[1] * 256 + tga.header[0];	//calc width
	texture->height = tga.header[3] * 256 + tga.header[2];	//calc height
	texture->bpp = tga.header[4];							//calc bits per pixel
	tga.Width = texture->width;								//copy with into local structure
	tga.Height = texture->height;							//copy height into local structure
	tga.Bpp = texture->bpp;									//copy bpp into local structure

	//make sure all info is valid
	if(( texture->width <= 0 ) || ( texture->height <= 0 ) || (( texture->bpp != 24 ) && ( texture->bpp != 32 ) ) )
	{
		///send an error
		return false;
	}

	if( texture->bpp == 24 )								// is it a 24 bpp image
	{
		texture->type = GL_RGB;								// if so set it to rgb
	}
	else													// if its not 24 its 32
	{
		texture->type = GL_RGBA;							// so set it to rgba
	}

	tga.bytesPerPixel = ( tga.Bpp / 8 );					// calc the bytes per pixel
	//calc memory needed to store image
	tga.imageSize = ( tga.bytesPerPixel *tga.Width * tga.Height );

	//allocate memory
	texture->imageData = ( GLubyte * )malloc( tga.imageSize );
	if( texture->imageData == NULL )
	{
		//send an error
		return false;
	}

	//attempt to read all the image data
	if( fread( texture->imageData, 1, tga.imageSize, fTGA ) != tga.imageSize )
	{
		//give em another error
		return false;
	}

	//start the loop
	for( GLuint cswap = 0; cswap < ( int )tga.imageSize; cswap += tga.bytesPerPixel )
	{
		//1st byte xor 3rd byte xor 1st byte xor 3rd byte 
		texture->imageData[cswap] ^= texture->imageData[cswap+2] ^=
		texture->imageData[cswap] ^= texture->imageData[cswap+2]; 
	}

	fclose( fTGA );											// close the file
	return true;											// return successful
}

bool CTGA::LoadCompressedTGA( Texture *texture, char *filename, FILE *fTGA )
{
	if( fread( tga.header, sizeof( tga.header ), 1, fTGA ) == 0 )
	{
		//send error
		return false;
	}
	texture->width = tga.header[1] * 256 + tga.header[0];
	texture->height = tga.header[3] * 256 + tga.header[2];
	texture->bpp = tga.header[4];
	tga.Width = texture->width;
	tga.Height = texture->height;
	tga.Bpp = texture->bpp;

	if( (texture->width <= 0 ) || ( texture->height <= 0 ) || ( ( texture->bpp != 24 ) && ( texture->bpp != 32 ) ) )
	{
		//send an error
		return false;
	}

	if( texture->bpp == 24 )					// is it a 24 bpp image
	{
		texture->type = GL_RGB;					// set it to rgb if so
	}
	else										// if not it has to be 32
	{
		texture->type = GL_RGBA;				// so set it to rgba
	}

	tga.bytesPerPixel	= ( tga.Bpp / 8 );
	tga.imageSize		= ( tga.bytesPerPixel * tga.Width * tga.Height );

	// allocate memory to store image data
	texture->imageData	= ( GLubyte * )malloc( tga.imageSize );
	if( texture->imageData == NULL )
	{
		//send error
		return false;
	}

	GLuint pixelcount	= tga.Height * tga.Width;			//number of pixels
	GLuint currentpixel	= 0;								//current pixel we are reading data from
	GLuint currentbyte	= 0;								//current byte we are writing into imagedata
	//storage for 1 pixel
	GLubyte * colorbuffer = ( GLubyte *)malloc( tga.bytesPerPixel );

	do														// start loop
	{
		GLubyte chunkheader = 0;							// variable to store the value of the id chunk
		if( fread( &chunkheader, sizeof( GLubyte ), 1, fTGA ) == 0 )	// attempt to read the chucks header
		{
			// send an error
			if(fTGA != NULL)												// If file is open
			{
				fclose(fTGA);												// Close file
			}
			if(texture->imageData != NULL)									// If there is stored image data
			{
				free(texture->imageData);									// Delete image data
			}
			return false;
		}

		if( chunkheader < 128 )								// if the chunk is raw
		{
			chunkheader++;									// add 1 to the value to get total number of raw pixels
			//start pixel reading loop
			for( short counter = 0; counter < chunkheader; counter++ )
			{
				//try to read 1 pixel
				if( fread( colorbuffer, 1, tga.bytesPerPixel, fTGA ) != tga.bytesPerPixel )
				{
					//send error
					return false;
				}

				texture->imageData[currentbyte		] = colorbuffer[2];		//write the r byte
				texture->imageData[currentbyte + 1	] = colorbuffer[1];	//write the g byte
				texture->imageData[currentbyte + 2	] = colorbuffer[0];	//write the b byte

				if( tga.bytesPerPixel == 4 )							// if its a 32 bpp image
				{
					texture->imageData[currentbyte + 3] = colorbuffer[3];	//write the a byte
				}
				// increment the byte counter by the number of bytes in a pixel
				currentbyte += tga.bytesPerPixel;
				currentpixel++;	//increment the number of pixels by 1
			}
		}
		else														// if its an rle header
		{
			chunkheader -= 127;										// subtract 127 to get rid of the id bit

			// read the next pixel
			if( fread( colorbuffer, 1, tga.bytesPerPixel, fTGA ) != tga.bytesPerPixel )
			{
				//send an error
				if(fTGA != NULL)												// If file is open
				{
					fclose(fTGA);												// Close file
				}
				if(texture->imageData != NULL)									// If there is stored image data
				{
					free(texture->imageData);									// Delete image data
				}
				return false;
			}

			//start the loop
			for( short counter = 0; counter < chunkheader; counter++ )
			{
				//copy the r byte
				texture->imageData[currentbyte		] = colorbuffer[2];
				//copy the g byte
				texture->imageData[currentbyte + 1	] = colorbuffer[1];
				//copy the b byte
				texture->imageData[currentbyte + 2	] = colorbuffer[0];
				if( tga.bytesPerPixel == 4 )						// if its a 32 bpp image
				{
					// copy the a byte
					texture->imageData[currentbyte + 3] = colorbuffer[3];
				}

				currentbyte += tga.bytesPerPixel;					// increment the byte counter
				currentpixel++;										// increment the pixel counter
			}
		}
	}while( currentpixel < pixelcount );							// more pixels to read?

	fclose( fTGA );													//close file
	return true;
}
