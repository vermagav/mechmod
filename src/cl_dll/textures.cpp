/**********************************************************
*			ALL PURPOSE TEXTURE LOADER
***********************************************************
*	Purpose: Loads all texture formats including spr
*	Created On: 4/29/04
*	Last Edited: 4/29/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	<il/il.h>
#pragma comment(lib, "DevIL.lib")

#include	<windows.h>
#include	<gl/gl.h>
#include	"glext.h"

#include	<io.h>

#include	"hud.h"
#include	"cl_util.h"
#include	"r_studioint.h"
#include	"triangleapi.h"
#include	"texture.h"
#include	"tga_replace.h"
extern		engine_studio_api_t IEngineStudio;


CTexture gTexture;
char ilFormats[MAX_FORMATS][5] = { "bmp",
"cut",
"dds",
"gif",
"ico",
"cur",
"jpg",
"jpe",
"jpeg",
"lbm",
"lif",
"lmp",
"mdl",
"mng",
"pcd",
"pcx",
"pic",
"pix",
"png",
"pbm",
"pgm",
"ppm",
"pnm",
"psd",
"pxr",
"sgi",
"bw",
"rgb",
"rgba",
"tga",
"tif",
"tiff",
"wal",
"xpm"};

CTexture::CTexture( void )
{
//	KillTextures( );
}

CTexture::~CTexture( void )
{
	KillTextures( );
}

void CTexture::KillTextures( void )
{
	if( IsOpenGL() )
	{
		ilInit( );
		CheckMipExt( );
	}
	// map textures usualy start around 500-1000 that give the map room for 1000 textures
	m_iTexture = 2000;	// base address for starting gl texture
	m_Textures.clear( );
}

bool CTexture::IsOpenGL( void )
{
	// opengl is hardware mode 1
	return ( IEngineStudio.IsHardware( ) == 1 );
}

Texture_s *CTexture::LoadTexture( char *pTex )
{
	std::string szTex = pTex;
	return FindTexture( &szTex );
}

Texture_s *CTexture::FindTexture( std::string *pTex )
{
	std::list<Texture_s>::iterator itTex = m_Textures.begin();

	// system didnt like being a vector so had to convert to list
	while( itTex != m_Textures.end() )//ForEachIn( itTex, m_Textures )
	{
		if( itTex->szName == *pTex )
		{
		//	gEngfuncs.Con_Printf( "Match Found: %s, %s\n", itTex->szName.c_str(), pTex->c_str() ); 
			return &*itTex;
		}

		itTex++;
	}

	return NewTexture( pTex );
}

Texture_s *CTexture::NewTexture( std::string *pTex )
{
	Texture_s *pNewTex = NULL;

	if( IsOpenGL( ) )
	{
		pNewTex = NewGLTex( pTex );
	}

	if( !pNewTex )
	{
		pNewTex = NewSprite( pTex );
	}

	return pNewTex;
}

Texture_s *CTexture::NewGLTex( std::string *pTex )
{
	std::string szFile;
	int iFrames = 0;

	szFile = FilePath( pTex, TEX_OPENGL, iFrames );

	if( szFile.size() )
	{
		Texture_s NewTexture;

		NewTexture.iType = TEX_OPENGL;
		NewTexture.szName = *pTex;
		NewTexture.szFullName = szFile;
		NewTexture.iFrames = iFrames;			// no animation support yet

		NewTexture.iTexNum = m_iTexture - iFrames;

		NewTexture.iFormat = 0;
		NewTexture.pModel = NULL;

		// Load GL Texture
		if( iFrames == 1 )
		{
			LoadGLTexture( &NewTexture );
		}

		m_Textures.push_back( NewTexture );

		std::list<Texture_s>::reverse_iterator itTex = m_Textures.rbegin();

		return &*itTex;
	}

	return NULL;
}

Texture_s *CTexture::NewSprite( std::string *pTex )
{
	std::string szPath = "sprites/";
	szPath += *pTex;
	szPath += ".spr";//FilePath( pTex, TEX_SPRITE );
	Texture_s NewTexture;

	NewTexture.szName = *pTex;
	NewTexture.szFullName = szPath;

	NewTexture.iType = TEX_SPRITE;
	NewTexture.iFormat = 0;
	
	NewTexture.pModel = ( model_s *)gEngfuncs.GetSpritePointer( gEngfuncs.pfnSPR_Load( szPath.c_str() ) );

	if( NewTexture.pModel )
	{
		if( IsOpenGL( ) )
		{
			int iTemp;
			gEngfuncs.pTriAPI->SpriteTexture( NewTexture.pModel, 0 );
			glGetIntegerv( GL_TEXTURE_BINDING_2D, &iTemp );
			NewTexture.iTexNum = iTemp;
		}
		NewTexture.iFrames = NewTexture.pModel->numframes;
		// ADD to list
		m_Textures.push_back( NewTexture );

		std::list<Texture_s>::reverse_iterator itTex = m_Textures.rbegin();

		return &*itTex;		// bastard casting
	}

	return NULL;
}

std::string CTexture::FilePath( std::string *pPath, int iType, int &iFrames )
{
	std::string szPath;

	//szPath = gEngfuncs.pfnGetGameDirectory( );

	if( iType == TEX_SPRITE )
	{
		szPath = "sprites/";
		szPath += *pPath;
		szPath += ".spr";
	}
	else
	{
		// dont even think about looking if were not in ogl mode
		if( IsOpenGL( ) )
		{
			szPath = FindGLPath( pPath, iFrames );
		}
	}


	return szPath;
}

std::string CTexture::IsGLFile( std::string *pTex )
{
	ilBindImage( m_iTexture ); // can bind it cause it we dont use it yet!!

	for( int i = 0; i < MAX_FORMATS; i++ )
	{
		std::string szFile = *pTex;

		szFile = szFile + "." + ilFormats[i];
		if( ilLoadImage( ( char * )szFile.c_str() ) )
		{
			return szFile;
		}
	}

	// no such file
	return "";
}

std::string CTexture::FindGLPath( std::string *pTex, int &iFrames )
{
	float flRes = gEngfuncs.pfnGetCvarFloat( "cl_gltexres" );
	if( !flRes )
		return "";

	std::string szGLPath, szMapName, szLevel, szFinal;

	szGLPath = gEngfuncs.pfnGetGameDirectory( );
	szGLPath += "/textures";
	szMapName = gEngfuncs.pfnGetLevelName( );
	szMapName.erase( 0, 5 );										// remove maps/
	szMapName.erase( szMapName.size( ) - 4, szMapName.size( ) );	// remove .bsp

	szLevel = IsAnim( pTex, iFrames );

	if( iFrames )
	{
	//	gEngfuncs.Con_Printf( "Someone Said We Have Frames\n" );
		return szLevel;
	}

	// model check
	std::string szTmp = *pTex;
	szTmp.erase( szTmp.length() -1, szTmp.length() );
	szFinal = szGLPath + "/" + szTmp + "/" + *pTex;

//	gEngfuncs.Con_Printf( "%s\n", szFinal.c_str() );
	szLevel = IsGLFile( &szFinal );

	if( szLevel.size( ) )
	{
		iFrames++;
		m_iTexture++;
		return szLevel;
	}

	for( int i= 4-flRes; i < 6; i ++ )
	{
		szFinal = szGLPath + "/" + szMapName + "/";

		switch( i )
		{
		case 0:
			szFinal += "highest/";
			break;
		case 1:
			szFinal += "high/";
			break;
		case 2:
			szFinal += "medium/";
			break;
		case 3:
			szFinal += "low/";
			break;
		case 4:
			break;
		case 5:
			szFinal = szGLPath + "/";
			break;
		default:
			break;
		}

		szFinal += *pTex;

		szLevel = IsGLFile( &szFinal );

		if( szLevel.size() )
		{
			iFrames++;
			m_iTexture++;
			return szLevel;
		}
	}

	return "";
	/*if( flRes >= 4 )	// highest
	{
		szLevel = "highest";
		szFinal = szGLPath + "/" + szMapName + "/" + szLevel + "/" + *pTex;
		// first path is textures/mapname/highest - high - medium - low - mapname - textures
		szFinal = IsGLFile( &szFinal );
	}

	if( !szFinal.size() && flRes >= 3 )
	{
		szLevel = "high";
		szFinal = szGLPath + "/" + szMapName + "/" + szLevel + "/" + *pTex;
		szFinal = IsGLFile( &szFinal );
	}
	if( !szFinal.size() && flRes >= 2 )
	{
		szLevel = "medium";
		szFinal = szGLPath + "/" + szMapName + "/" + szLevel + "/" + *pTex;
		szFinal = IsGLFile( &szFinal );
	}
	if( !szFinal.size() && flRes >= 1 )
	{
		szLevel = "low";
		szFinal = szGLPath + "/" + szMapName + "/" + szLevel + "/" + *pTex;
		szFinal = IsGLFile( &szFinal );
	}
	if( !szFinal.size() )
	{
		szFinal = szGLPath + "/" + szMapName + "/" + *pTex;
		szFinal = IsGLFile( &szFinal );
	}
	if( !szFinal.size() )
	{
		szFinal = szGLPath + "/" + *pTex;
		szFinal = IsGLFile( &szFinal );
	}
	if( szFinal.size() )
	{
		iFrames++;
		m_iTexture++;
		return szFinal;
	}
	return "";*/

}

std::string CTexture::IsAnim( std::string *pTex, int &iFrames )
{
	// loop while files exist
	std::string szFile, szPreFile, szLastFile;
	
	char szInt[8];

	iFrames = 0;


	szFile = gEngfuncs.pfnGetGameDirectory( );
	szFile += "/textures/";
	szFile += *pTex;
	szPreFile = szFile;
	szFile += "/";

	// checks if folder exists and uses data inside it :D

//	gEngfuncs.Con_Printf( "%s\n", szFile.c_str() );
	
	if( !_access( szFile.c_str( ), 00 ) )
	{
		szFile += *pTex;
		szPreFile = szFile;
	}

	szFile = szPreFile;

//	szLastFile = szFile;
	
	sprintf( szInt, "%i", iFrames );
	szFile += szInt;			// texture # 0 - whatever
	szFile = IsGLFile( &szFile );

	while( szFile.size( ) )
	{
		Texture_s NewTexture;

		iFrames++;
		NewTexture.iTexNum = m_iTexture++;
		NewTexture.szFullName = szFile;
		szLastFile = szFile;
		//gEngfuncs.Con_Printf( "Loaded %s\n", szFile.c_str() );
		LoadGLTexture( &NewTexture );

		
		sprintf( szInt, "%i", iFrames );
		szFile = szPreFile;
		szFile += szInt;
		szFile = IsGLFile( &szFile );
	}
	
//	m_iTexture += iFrames;

	return szLastFile;
}

void CTexture::LoadGLTexture( Texture_s *pTex )
{
	// load texture as a gl_tex

	ilBindImage( pTex->iTexNum );
	glBindTexture( GL_TEXTURE_2D, pTex->iTexNum );

	if( m_iMipMap == SIGS_MIPMAP )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
	}
	else if( m_iMipMap == GL_MIPMAP )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

	ilLoadImage( ( char * )pTex->szFullName.c_str() );

	glTexImage2D( GL_TEXTURE_2D, 0, ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL ),
						ilGetInteger( IL_IMAGE_WIDTH ), ilGetInteger( IL_IMAGE_HEIGHT ),
						0, ilGetInteger( IL_IMAGE_FORMAT ), GL_UNSIGNED_BYTE,
						ilGetData( ) );

}

void CTexture::CheckMipExt( void )
{
	char *pExt;
	pExt = strdup( ( char * ) glGetString( GL_EXTENSIONS ) );

	float flVer = atof( ( char * ) glGetString( GL_VERSION ) );

	if( isInString( "GL_SGIS_generate_mipmap", pExt ) || isInString( "GL_EXT_generate_mipmap", pExt ))
	{
		m_iMipMap = SIGS_MIPMAP;
	}
	else if( flVer >= 1.4 )
	{
		m_iMipMap = GL_MIPMAP;
	}
	else
	{
		gEngfuncs.Con_Printf( "MipMap extension not found\n" );
		m_iMipMap = NO_MIPMAP;
	}

}

bool CTexture::OverwriteTexture( int iTex, char *pTex, int iFormat, bool blOld )
{
	// make sure we havent loaded the texture yet
	std::list<Texture_s>::iterator itTex = m_Textures.begin();

	if( blOld )
	{
		//ForEachIn( itTex, m_Textures )
		while( itTex != m_Textures.end( ) )
		{
			if( itTex->iTexNum == iTex )
			{
				return true;
			}

			itTex++;
		}
	}

	if( IsOpenGL( ) )
	{
		std::string szFile = pTex;
		int iFrames = 0;

		szFile = FilePath( &szFile, TEX_OPENGL, iFrames );

		if( szFile.size() )
		{
			Texture_s NewTexture;

			NewTexture.iType = TEX_OPENGL;
			NewTexture.szName = pTex;
			NewTexture.szFullName = szFile;
			NewTexture.iFrames = iFrames;			// no animation support yet
			NewTexture.iTexNum = iTex;
			NewTexture.iFormat = iFormat;

		//	gEngfuncs.Con_Printf( "%s, %i\n", szFile.c_str( ), iTex );
			// Load GL Texture
			LoadGLTexture( &NewTexture );

			m_Textures.push_back( NewTexture );
			return true;
		}
	}

	return false;
}

void CTexture::CreateEmptyTex( int iWidth, int iHeight, unsigned int &iTex, int iType, int iFormat )
{
	// takes too long on large screens
	unsigned char *pData = new unsigned char[iWidth*iHeight* 4];

//	memset( pData, 0, (iWidth*iHeight* 4) );

	
	iTex = m_iTexture++;

	glBindTexture( iType, iTex );

	// not having these kills me
	glTexParameteri( iType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri( iType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri( iType, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( iType, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
//	glTexImage2D( iType, 0, GL_RGBA,
//						iWidth, iHeight,
//						0, GL_RGBA, GL_UNSIGNED_BYTE,
//						pData );
	glCopyTexImage2D( iType, 0, iFormat, 0, 0, iWidth, iHeight, 0 );

	Texture_s NewTexture;

	NewTexture.iType = TEX_OPENGL;
	NewTexture.szName = m_iTexture;
	NewTexture.iFrames = 0;
	NewTexture.iFormat = GL_RGBA;
	NewTexture.iTexNum = iTex;

	m_Textures.push_back( NewTexture );

	delete [] pData;
}

void CTexture::Precache( void )
{
	// make sure these get loaded on level change cuase they may take 
	// quite a while to load at runtime

	// precahce particle spritesm
	// custom effects get precached as soon as the server sends there info
	LoadTexture( "alphabomb" );
	LoadTexture( "fexplo1" );
	LoadTexture( "splash" );
	LoadTexture( "flare1" );
	LoadTexture( "flare3" );
	LoadTexture( "ballsmoke" );
	LoadTexture( "deathscythe1" );
	LoadTexture( "playershadow" );

}

