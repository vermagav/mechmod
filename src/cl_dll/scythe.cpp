/**********************************************************
*			DEATHSCYTHE SCYTHE
***********************************************************
*	Purpose: Sprite for deathscythe's scythe
*	Created On: 4/15/04
*	Last Edited: 5/21/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "texture.h"			//#include "spr_texture.h"
#include "ds_scythe.h"
#include "gl-triapi.h"
#include	<windows.h>
#include	<gl/gl.h>
#include	"glext.h"
#include	"cg_glow.h"


CScythe g_Scythe;


CScythe::CScythe( )
{
	m_iFrame = 0;
	m_pTex = NULL;
	m_flFrameRate = 0.0f;
}

CScythe::~CScythe( )
{
	m_iFrame = 0;
	m_pTex = NULL;
	m_flFrameRate = 0.0f;
}

bool CScythe::IsDs( int iIndex )
{
	bool blFalse = false;
	cl_entity_t *pEnt = gEngfuncs.GetEntityByIndex( iIndex );

	if( pEnt )
	{
		if( pEnt->model )
		{
			if( pEnt->curstate.playerclass == 3 )//!stricmp( "models/player/deathscythe/deathscythe.mdl", pEnt->model->name ) || !stricmp( "models/p_deathscythe_scythe.mdl", pEnt->model->name ) )
			{
				blFalse = true;
			}
		}
	}
	
	return blFalse;
}

void CScythe::Init( void )
{
	m_pTex = gTexture.LoadTexture( "deathscythe1" );
}

void CScythe::RenderScythe( void )
{
	if( m_pTex )
	{
		PreRender( );
	
		DrawScythe( );

		PostRender( );
	}
}

void CScythe::PreRender( void )
{
	UpdateSprite( );
	

	g_CGLTriAPI.Texture2D( m_pTex, m_iFrame );

	if( gTexture.IsOpenGL( ) )
	{
		int iTemp;
		glGetIntegerv( GL_TEXTURE_BINDING_2D, &iTemp );
		while( iTemp > m_pTex->iTexNum + m_pTex->iFrames )
		{
			glBindTexture( GL_TEXTURE_2D, m_pTex->iTexNum + m_iFrame );
			glGetIntegerv( GL_TEXTURE_BINDING_2D, &iTemp );
		}
			
		
	}

	g_CGLTriAPI.Color4f( 1.0f, 1.0f, 1.0f, 1.0f );

	g_CGLTriAPI.RenderMode( kRenderTransAdd );
	g_CGLTriAPI.CullFace( TRI_NONE );
}

void CScythe::DrawScythe( void )
{
	for( int i = 1; i <= 32; i++ )
	{
		if( IsDs( i ) )
		{
			bool blOk;
			
			std::list<saberfx_s>::iterator iterSaber = gHUD.m_Saberfx.FindSaberfx( i );
			if(iterSaber == NULL)
			{
				continue;
			}
			
			recboneiter iterBone = iterSaber->iterRecPos;
			
			g_CGLTriAPI.Begin( TRI_QUADS );
			
			vec3_t vPoint = gHUD.m_BonePos.GetBoneOrigin( iterBone, "scythe_0", blOk );
			
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			g_CGLTriAPI.Vertex3fv( vPoint );
			
			vPoint = gHUD.m_BonePos.GetBoneOrigin( iterBone, "scythe_1", blOk );
			
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			g_CGLTriAPI.Vertex3fv( vPoint );
			
			vPoint = gHUD.m_BonePos.GetBoneOrigin( iterBone, "scythe_2", blOk );
			
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			g_CGLTriAPI.Vertex3fv( vPoint );
			
			vPoint = gHUD.m_BonePos.GetBoneOrigin( iterBone, "scythe_3", blOk );
			
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			g_CGLTriAPI.Vertex3fv( vPoint );
			
			g_CGLTriAPI.End( );
		}
	}
}

void CScythe::PostRender( void )
{
	g_CGLTriAPI.CullFace( TRI_FRONT );
	g_CGLTriAPI.RenderMode( kRenderNormal );
}

void CScythe::UpdateSprite( void )
{
	if( m_flFrameRate < gEngfuncs.GetClientTime( ) )
	{
		m_flFrameRate = gEngfuncs.GetClientTime( ) + 1.0f/30.0f;
		
		if( m_pTex )
		{
			m_iFrame++;

			if( m_iFrame >= m_pTex->iFrames )
				m_iFrame = 0;
		}
	}
}