/**********************************************************
*					OPENGL P BUFFER 
***********************************************************
*	Purpose: pixel buffer for faster render to texture
*	Created On: 7/21/04
*	Last Edited: 7/21/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"

#include	"gl_pbuffer.h"
#include	"texture.h"

CPixelBuffer g_PBuffer;

PFNWGLGETEXTENSIONSSTRINGARBPROC	wglGetExtensionsStringARB = NULL;
PFNWGLCREATEPBUFFERARBPROC			wglCreatePbufferARB = NULL;
PFNWGLGETPBUFFERDCARBPROC			wglGetPbufferDCARB = NULL;
PFNWGLRELEASEPBUFFERDCARBPROC		wglReleasePbufferDCARB = NULL;
PFNWGLDESTROYPBUFFERARBPROC			wglDestroyPbufferARB = NULL;
PFNWGLQUERYPBUFFERARBPROC			wglQueryPbufferARB = NULL;
PFNWGLGETPIXELFORMATATTRIBIVARBPROC	wglGetPixelFormatAttribivARB = NULL;
PFNWGLGETPIXELFORMATATTRIBFVARBPROC	wglGetPixelFormatAttribfvARB = NULL;
PFNWGLCHOOSEPIXELFORMATARBPROC		wglChoosePixelFormatARB = NULL;
PFNWGLBINDTEXIMAGEARBPROC			wglBindTexImageARB = NULL;
PFNWGLRELEASETEXIMAGEARBPROC		wglReleaseTexImageARB = NULL;


CPixelBuffer::CPixelBuffer( void )
{
	// init vars
	m_blSupported = true;
}

CPixelBuffer::~CPixelBuffer( void )
{
	// destructor
}

void CPixelBuffer::Init( void )
{
	// make sure extension string is available
	wglGetExtensionsStringARB = ( PFNWGLGETEXTENSIONSSTRINGARBPROC ) wglGetProcAddress( "wglGetExtensionsStringARB" );


	if( wglGetExtensionsStringARB )
	{
		// find sub strings

		char *extensions = ( char *)wglGetExtensionsStringARB( wglGetCurrentDC( ) );

		// find strings
		if( !isInString( "WGL_ARB_pixel_format", extensions ) || !isInString( "WGL_ARB_pbuffer", extensions ) )
		{
			m_blSupported = false;
		}

		if( m_blSupported )
		{
			// load our functions
			// Initilaize our pbuffer entry points
			INIT_ENTRY_POINT( wglCreatePbufferARB, PFNWGLCREATEPBUFFERARBPROC );
			INIT_ENTRY_POINT( wglGetPbufferDCARB, PFNWGLGETPBUFFERDCARBPROC );
			INIT_ENTRY_POINT( wglReleasePbufferDCARB, PFNWGLRELEASEPBUFFERDCARBPROC );
			INIT_ENTRY_POINT( wglDestroyPbufferARB, PFNWGLDESTROYPBUFFERARBPROC );
			INIT_ENTRY_POINT( wglQueryPbufferARB, PFNWGLQUERYPBUFFERARBPROC );

			// initilaize the pixel format entry points
			INIT_ENTRY_POINT( wglGetPixelFormatAttribivARB, PFNWGLGETPIXELFORMATATTRIBIVARBPROC );
			INIT_ENTRY_POINT( wglGetPixelFormatAttribfvARB, PFNWGLGETPIXELFORMATATTRIBFVARBPROC );
			INIT_ENTRY_POINT( wglChoosePixelFormatARB, PFNWGLCHOOSEPIXELFORMATARBPROC );

			INIT_ENTRY_POINT( wglBindTexImageARB, PFNWGLBINDTEXIMAGEARBPROC );
			INIT_ENTRY_POINT( wglReleaseTexImageARB, PFNWGLRELEASETEXIMAGEARBPROC );


			Create( );
		}

	}
}

void CPixelBuffer::Create( void )
{
	
	
	// create pbuffer
	m_WindowHDC = wglGetCurrentDC( );
	m_WindowRC = wglGetCurrentContext( );

	int iPixelFormat[] = 
	{
		WGL_SUPPORT_OPENGL_ARB, TRUE,
		WGL_DRAW_TO_PBUFFER_ARB, TRUE,
		WGL_BIND_TO_TEXTURE_RGBA_ARB, TRUE,
		WGL_RED_BITS_ARB, 8,
		WGL_GREEN_BITS_ARB, 8,
		WGL_BLUE_BITS_ARB, 8,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 16,
		WGL_DOUBLE_BUFFER_ARB, FALSE,
		0
	};

	unsigned int iCount = 0;
	int iPixForm;
	wglChoosePixelFormatARB( m_WindowHDC, ( const int* ) iPixelFormat, NULL, 1, &iPixForm, &iCount );

	if( iCount == 0 )
	{
		gEngfuncs.Con_Printf( "Problem Finding Pixel Format\n" );
		m_blSupported = false;
	}

	// Set attribs so we can use render to texture

	int iPBuffAttr[] = 
	{
		WGL_TEXTURE_FORMAT_ARB, WGL_TEXTURE_RGBA_ARB,
		WGL_TEXTURE_TARGET_ARB, WGL_TEXTURE_2D_ARB,
		0
	};

	m_iWidth = 1;
	while( m_iWidth < ScreenWidth )
	{
		m_iWidth *= 2;
	}


	//m_iWidth	= 512;//ScreenWidth;
	m_iHeight	= m_iWidth;//512;//ScreenHeight;
	gTexture.CreateEmptyTex( m_iWidth, m_iHeight, m_iTexture, GL_TEXTURE_2D, GL_RGBA );
	m_hPBuffer	= wglCreatePbufferARB( m_WindowHDC, iPixForm, m_iWidth, m_iHeight, iPBuffAttr );
	m_hDC		= wglGetPbufferDCARB( m_hPBuffer );
	m_hRC		= wglCreateContext( m_hDC );

	if( !m_hPBuffer )
	{
		gEngfuncs.Con_Printf( "Problem Creating PBuffer\n" );
		m_blSupported = false;
	}

	int h, w;
	wglQueryPbufferARB( m_hPBuffer, WGL_PBUFFER_WIDTH_ARB, &w );
	wglQueryPbufferARB( m_hPBuffer, WGL_PBUFFER_HEIGHT_ARB, &h );

	if( h != m_iHeight || w != m_iWidth )
	{
		gEngfuncs.Con_Printf( "Pbuffer Size doesnt match\n" );
		m_blSupported = false;
	}

	if( !wglMakeCurrent( m_hDC, m_hRC ) )
	{
		gEngfuncs.Con_Printf( "Couldnt make Pbuffer Current\n" );
		m_blSupported = false;
	}

	RestoreRender( );
}

void CPixelBuffer::EnablePBuffer( void )
{
	int iFlag = 0;

	wglQueryPbufferARB( m_hPBuffer, WGL_PBUFFER_LOST_ARB, &iFlag );

	if( iFlag )
	{
		gEngfuncs.Con_Printf( "Pbuffer was Lost\n" );
	}


	if( !wglMakeCurrent( m_hDC, m_hRC ) )
	{
		gEngfuncs.Con_Printf( "Couldnt make Pbuffer Current\n" );
	
	}

	
}

void CPixelBuffer::RestoreRender( void )
{
	if( !wglMakeCurrent( m_WindowHDC, m_WindowRC ) )
	{
		gEngfuncs.Con_Printf( "Couldnt Restore Renderer\n" );

	}
}

void CPixelBuffer::BindImage( void )
{
//	glBindTexture( GL_TEXTURE_2D, m_iTexture );

	if( !wglBindTexImageARB( m_hPBuffer, WGL_FRONT_LEFT_ARB ) )
	{
		gEngfuncs.Con_Printf( "Couldnt bind pbuffer to render texture\n" );
	}
}

void CPixelBuffer::ReleaseImage( void )
{
	if( !wglReleaseTexImageARB( m_hPBuffer, WGL_FRONT_LEFT_ARB ) )
	{
		gEngfuncs.Con_Printf( "Couldnt release pbuffer to render texture\n" );
	}
}

void CPixelBuffer::RenderToScreen( void )
{
	
/*	ReleaseImage( );
	RestoreRender( );
	
	
	glBindTexture( GL_TEXTURE_2D, m_iTexture );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	glLoadIdentity( );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix( );
	glLoadIdentity( );

	glOrtho( 0, 1, 1, 0, 0.1, 100 );

	
	glViewport( 0,0, ScreenWidth, ScreenHeight );
	
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glBegin( GL_QUADS );
		glTexCoord2f( 0, 0 );
		glVertex3f( 0, 1, -1 );
		glTexCoord2f( 0, float( ScreenHeight ) /float( m_iHeight ) );
		glVertex3f( 0, 0, -1 );
		glTexCoord2f( float( ScreenWidth )/ float(  m_iWidth ), float( ScreenHeight )/float( m_iHeight )  );
		glVertex3f( 1, 0, -1 );
		glTexCoord2f( float( ScreenWidth )/ float(  m_iWidth ), 0 );
		glVertex3f( 1, 1, -1 );
	glEnd( );

	
	glMatrixMode( GL_PROJECTION );
	glPopMatrix( );
	
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	//ReleaseImage( );
	//EnablePBuffer( );
	EnablePBuffer( );
	
	BindImage( );*/
	

//	glClear( GL_COLOR_BUFFER_BIT );
}
