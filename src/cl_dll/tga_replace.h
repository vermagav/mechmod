/**********************************************
*				TGA REPLACE
*
* created by: Jason "ssjason123" Matson
* created on: 9-21-03
* goal: to create a dynamic loader for tga
* textures, that will load them if they exist
**********************************************/

#ifndef __TEX_REPLACE_H_

#define __TEX_REPLACE_H__


// openil defines


class CTGAReplace
{
public:
	/*
	* Replace all Textures
	* purpose: replaces all map texures with high def textures
	* param void: no param
	* return void: no return
	*/
	void ReplaceAllTextures( void );
private:
};

extern bool isInString( char *string, const char *compare );
extern CTGAReplace gTGAReplace;

#endif