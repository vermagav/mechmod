/**********************************************************
*			ALL PURPOSE TEXTURE LOADER
***********************************************************
*	Purpose: Loads all texture formats including spr
*	Created On: 4/29/04
*	Last Edited: 4/29/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H
#define MAX_FORMATS 34
#define TEX_SPRITE	1		// sprite tex
#define	TEX_OPENGL	2		// only opengl tex

#define	NO_MIPMAP	0
#define	GL_MIPMAP	1
#define	SIGS_MIPMAP	2

struct Texture_s
{
	int				iType;		// sprite or ogl
	std::string		szName;		// name stripped of ext and path
	std::string		szFullName;	// name including path

//	union
	//{
	int			iTexNum;	// if ogl texture
	model_s		*pModel;	// if sprite texture
//	};
	// if ogl load # of frames
	int				iFrames;	// so iTexNum + iFrames = max tex num
	int				iFormat;	// ogl textures can have format
};

class CTexture
{
public:
	// constructor
				CTexture( void );
	// destructor
				~CTexture( void );
	/*
	* IsOpenGL
	* purpose: lets us know if we are running ogl mode or not
	* param void: no param
	* return bool: if we are running in ogl mode or not
	*/
	bool		IsOpenGL( void );
	/*
	* KillTextures
	* purpose: removes loaded textures and resets our texture list
	* param void: no param
	* return void: no param
	*/
	void		KillTextures( void );
	/*
	* Frecache
	* purpose: loads textures for quick access
	* param void: no param
	* return void: no param
	*/
	void		Precache( void );

	/*
	* LoadTexture
	* purpose: load texture by name
	* param char *pTex: name of the texture excluding paths
	* return Texture_s *: info on the texture loaded
	*/
	Texture_s	*LoadTexture( char *pTex );
	/*
	* OverwriteTexture
	* purpose: Overwrites an existing texture that was loaded
	* param int iTex: gl tex num of the texture
	* param char *pTex: name of the texture excluding paths
	* param int iFormat: any perticular format we want the texture in
	* param bool blOld: allows us to update old texture
	* return bool: was the texture replaced
	*/
	bool		OverwriteTexture( int iTex, char *pTex, int iFormat, bool blOld );
	/*
	* CreateEmptyTex
	* purpose: creates an empty texture we can fill
	* param int iWidth: width of texture
	* param int iHeight: height of texture
	* param int &iTex: texture addr
	* param int iType: type of texture 2d, rect or whatever
	* param int iFormat: TYPE THE TEXTURE WILL BE
	* return void: no return
	*/
	void		CreateEmptyTex( int iWidth, int iHeight, unsigned int &iTex, int iType, int iFormat );
private:
	/*
	* CheckMipExt
	* purpose: checks if we have a mip map extension available
	* param void: no param
	* return void: no return
	*/
	void		CheckMipExt( void );
	/*
	* FindTexture
	* purpose: Finds texture if its loaded
	* param std::string *pTex: name of the texture excluding paths
	* return Texture_s *: info on the texture found
	*/
	Texture_s	*FindTexture( std::string *pTex );
	/*
	* NewTexture
	* purpose: creates a new texture
	* param std::string *pTex: name of the texture excluding paths
	* return Texture_s *: info on the texture found
	*/
	Texture_s	*NewTexture( std::string *pTex );
	/*
	* NewSprite
	* purpose: Loads a new sprite file
	* param std::string *pTex: name of the texture excluding paths
	* return Texture_s *: info on the texture found
	*/
	Texture_s	*NewSprite( std::string *pTex );
	/*
	* NewGlTex
	* purpose: loads a new opengl texture
	* param std::string *pTex: name of the texture excluding paths
	* return Texture_s *: info on the texture found
	*/
	Texture_s	*NewGLTex( std::string *pTex );
	/*
	* FilePath
	* purpose: Finds file in one of our paths
	* param std::string *pTex: name of the texture excluding paths
	* param int iType: tells if its an ogl tex or sprite
	* return std::string: the path to the file
	*/
	std::string		FilePath( std::string *pTex, int iType, int &iFrames );
	/*
	* IsGLFile
	* purpose: Finds texture extension for an ogl file
	* param std::string *pTex: name of the texture excluding paths
	* return std::string: the full path to the file
	*/
	std::string		IsGLFile( std::string *pTex );
	/*
	* FindGLPath
	* purpose: Finds path to the gl file
	* param std::string *pTex: name of the texture excluding paths
	* return std::string: the full path to the file
	*/
	std::string		FindGLPath( std::string *pTex, int &iFrames );
	/*
	* LoadGLTexture
	* purpose: Loads an opengl texture after we find its path
	* param Texture_s *pTex: texture info
	* return void: no return
	*/
	void		LoadGLTexture( Texture_s *pTex );

	std::string IsAnim( std::string *pTex, int &iFrames );

	int						m_iMipMap;		// mip map level
	int						m_iTexture;		// current gl tex num
	std::list<Texture_s>	m_Textures;		// array of textures
};

extern CTexture gTexture;
#endif