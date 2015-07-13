/**********************************************
*				TGA REPLACE
*
* created by: Jason "ssjason123" Matson
* created on: 9-21-03
* goal: to create a dynamic loader for tga
* textures, that will load them if they exist
**********************************************/

#include "hud.h"
#include "cl_util.h"
#include "pm_defs.h"
#include "com_model.h"
#include "tga_replace.h"
#include "gl_bored.h"
#include "texture.h"


/*
* Is In String
* purpose: finds if the smaler string is in the larger string
* param string: the string we want to find
* param compare: the string we are looking through
* return bool: if the string was found or not
*/
bool isInString( char *string, const char *compare );


CTGAReplace gTGAReplace;

void CTGAReplace::ReplaceAllTextures( void )
{
	model_t *pModel = gEngfuncs.GetEntityByIndex( 0 )->model;
	//open mapname txt for writing


	//used to be looping through alot more but it was all gay and stuff on map changes
	int iNumTex = pModel->numtextures;

	while( iNumTex -- )
	{
		gTexture.OverwriteTexture( pModel->textures[iNumTex]->gl_texturenum, 
									pModel->textures[iNumTex]->name, 0, true );
	}

}


bool isInString( char *string, const char *compare )
{
	int iPos = 0;
	int iMaxPos = strlen( compare ) - 1;
	int	iLen = strlen( string ) - 1;
//gEngfuncs.Con_Printf( "%s\n", compare );
	for( int i = 0; i < iMaxPos; i++ )
	{
		iPos = 0;
		while( tolower(string[iPos]) == tolower(compare[i]) )
		{
			if( iPos == iLen )
				return true;
			
			iPos++;
			i++;	
		}
	}

	return false;
}
