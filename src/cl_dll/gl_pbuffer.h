/**********************************************************
*					OPENGL P BUFFER 
***********************************************************
*	Purpose: pixel buffer for faster render to texture
*	Created On: 7/21/04
*	Last Edited: 7/21/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/
#ifndef	GL_PBUFFER
#define GL_PBUFFER

#include	<windows.h>
#include	<gl/gl.h>
#include	<gl/wglext.h>

extern bool isInString( char *string, const char *compare );

class CPixelBuffer
{
public:
			CPixelBuffer( );
			~CPixelBuffer( );

	void	Init( void );
	void	Create( void );
	void	EnablePBuffer( void );
	void	RestoreRender( void );
	void	BindImage( void );
	void	ReleaseImage( void );
	unsigned int	iGetTex( void ) { return m_iTexture; }
	void	RenderToScreen( void );
private:
	bool	m_blSupported;
	int		m_iHeight;
	int		m_iWidth;
	unsigned int	m_iTexture;
	HDC		m_WindowHDC;
	HDC		m_hDC;
	HGLRC	m_hRC;
	HPBUFFERARB	m_hPBuffer;
	HGLRC	m_WindowRC;
};

#define INIT_ENTRY_POINT( funcname, type )					\
	funcname = ( type ) wglGetProcAddress( #funcname );		\
	if( !funcname )											\
	gEngfuncs.Con_Printf( "#funcname() not Initialized!\n" );

extern CPixelBuffer g_PBuffer;

#endif