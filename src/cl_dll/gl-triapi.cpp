/**********************************************************
*				OPENGL TRIAPI WRAPPER
***********************************************************
*	Purpose: calls approprite gl or tri func
*	Created On: 5/20/04
*	Last Edited: 5/20/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"
#include	"studio_util.h"
#include	"triangleAPI.h"
#include	"texture.h"
#include	<windows.h>
#include	<gl/gl.h>

#include	"r_studioint.h"
#include	"gl-triapi.h"
#include	"cg_glow.h"

extern		engine_studio_api_t IEngineStudio;
extern		bool g_blForceNormal;
CGLTriAPI	g_CGLTriAPI;

CGLTriAPI::CGLTriAPI( void )
{
	// decied to use a bool so we dont have the overhead of a function call all the time
	//m_blOGL = IsOGL( );
	m_blOGL = false;
}

void CGLTriAPI::OGLActive( void )
{
	// Edited by Pcjoe: Sorry jason, using regular tri-api calls in the wrapper
	// so the sprites look normal ^_^;
	// editied back by jason
	m_blOGL = IsOGL( );
//	m_blOGL = false;
}

bool CGLTriAPI::IsOGL( void )
{
	return ( IEngineStudio.IsHardware( ) == 1 );
}

void CGLTriAPI::Begin( int iPrim )
{
	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->Begin( iPrim );
	}
	else
	{
		switch( iPrim )
		{
		case TRI_TRIANGLES:
			glBegin( GL_TRIANGLES );
			break;
		case TRI_TRIANGLE_FAN:
			glBegin( GL_TRIANGLE_FAN );
			break;
		case TRI_QUADS:
			glBegin( GL_QUADS );
			break;
		case TRI_POLYGON:
			glBegin( GL_POLYGON );
			break;
		case TRI_LINES:
			glBegin( GL_LINES );
			break;
		case TRI_TRIANGLE_STRIP:
			glBegin( GL_TRIANGLE_STRIP );
			break;
		case TRI_QUAD_STRIP:
			glBegin( GL_QUAD_STRIP );
			break;
		default:
			break;
		}
		//glBegin( iPrim );
	}
}

void CGLTriAPI::End( void )
{
	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->End( );
	}
	else
	{
		glEnd( );
	}
}

void CGLTriAPI::Color4f( float r, float g, float b, float a )
{
	float flColor[4] = {r,g,b,a};

	Color4fv( flColor );
}

void CGLTriAPI::Color4fv( float *flColor )
{
//	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->Color4f( flColor[0], flColor[1], flColor[2], flColor[3] );
	}
/*	else
	{
		glColor4f( flColor[0], flColor[1], flColor[2], flColor[3] );
	}*/
}

void CGLTriAPI::Color4fv( colorRGBA clColor )
{
	float flColor[4] = {clColor.r, clColor.g, clColor.b, clColor.a };

	Color4fv( flColor );
}

void CGLTriAPI::Color4ub( unsigned char r, unsigned char g, unsigned char b, unsigned char a )
{
	unsigned char cColor[4] = {r,g,b,a};

	Color4ubv( cColor );
}

void CGLTriAPI::Color4ubv( unsigned char *cColor )
{
	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->Color4ub( cColor[0], cColor[1], cColor[2], cColor[3] );
	}
	else
	{
		glColor4ubv( cColor );
	}
}

void CGLTriAPI::TexCoord2f( float u, float v )
{
	float flUV[2] = {u, v};

	TexCoord2fv( flUV );
}

void CGLTriAPI::TexCoord2fv( float *flUV )
{
	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->TexCoord2f( flUV[0], flUV[1] );
	}
	else
	{
		glTexCoord2fv( flUV );
	}
}

void CGLTriAPI::CullFace( TRICULLSTYLE iCull )
{
	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->CullFace( iCull );
	}
	else
	{
		if( TRI_NONE == iCull )
		{
			glDisable( GL_CULL_FACE );
		}
		else
		{
			glEnable( GL_CULL_FACE );
			glCullFace( GL_FRONT );
		}
	}
}

void CGLTriAPI::Vertex3f( float x, float y, float z )
{
	float flPoint[3] = {x, y, z};

	Vertex3fv( flPoint );
}

void CGLTriAPI::Vertex3fv( float *flPoint )
{
	if( !m_blOGL )
	{
		gEngfuncs.pTriAPI->Vertex3f( flPoint[0], flPoint[1], flPoint[2] );
	}
	else
	{
		glVertex3fv( flPoint );
	}
}

void CGLTriAPI::Texture2D( model_s *pTex, int iTex )
{
	// since we have no ogl equivelent we will jsut use the tri api call
	gEngfuncs.pTriAPI->SpriteTexture( pTex, iTex );
	// or if we had the base texture in pTex we could do pTex + iTex for ogl but we dont
}

void CGLTriAPI::Texture2D( Texture_s *pTex, int iFrame )
{
	if( pTex->iType == TEX_SPRITE )
	{
		Texture2D( pTex->pModel, iFrame );
	}
	else
	{
		glBindTexture( GL_TEXTURE_2D, pTex->iTexNum + iFrame ); 
	}
}

void CGLTriAPI::RenderMode( int iMode )
{
//	if( !m_blOGL )
	{
		if( g_blForceNormal )
		{
			//gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
			glEnable( GL_BLEND );
			glBlendFunc( GL_ZERO, GL_SRC_COLOR  );
		}
		else
		{
			gEngfuncs.pTriAPI->RenderMode( iMode );
		}
	}
/*	else
	{
		// chances are i need to enable blending but time will tell
		
		// case for each mode
		switch( iMode )
		{
		case kRenderNormal:
			glBlendFunc( GL_ONE, GL_ZERO );
			glDisable( GL_BLEND );
			//glDepthMask(GL_TRUE); 
			break;
		case kRenderTransAlpha:
			glEnable( GL_BLEND );
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glDepthMask(GL_FALSE); 
			break;
		case kRenderTransAdd:
			glEnable( GL_BLEND );
			glBlendFunc( GL_ONE, GL_ONE );
			//glDepthMask(GL_FALSE);
			break;
		default:
			// I DONT KNOW THE OTHER MODES :(
		//	glEnable( GL_BLEND );
			gEngfuncs.pTriAPI->RenderMode( iMode );
			//glDepthMask(GL_FALSE);
			break;
		}
	}*/
}

void CGLTriAPI::Brightness( float flBrightness )
{
	gEngfuncs.pTriAPI->Brightness( flBrightness );
}

void CGLTriAPI::DisableDepthMask( void )
{
	if( m_blOGL )
	{
	//	glDisable( GL_DEPTH_TEST );
		glDepthMask(GL_FALSE);
	}
}

void CGLTriAPI::EnableDepthMask( void )
{
	if( m_blOGL )
	{
	//	glEnable( GL_DEPTH_TEST );
		glDepthMask(GL_TRUE);
	}
}
