/**********************************************************
*			NEW ALPHABOMB EFFECT
***********************************************************
*	Purpose: New Alphabomb Effect, Since model takes lighting
*	Created On: 8/31/04
*	Last Edited: 8/31/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "pmtrace.h"
#include "pm_defs.h"
#include "triangleapi.h"
#include "com_model.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "eff_all.h"
#include "gl-triapi.h"
#include "alpha_bomb.h"
#include "r_studioint.h"
extern		engine_studio_api_t IEngineStudio;

CAlphaBomb g_AlphaBomb;

CAlphaBomb::CAlphaBomb( void )
{
	Reset( );
}

void CAlphaBomb::Initialize( void )
{
	Reset( );

	m_pTex = gTexture.LoadTexture( "alphafly" );
}

void CAlphaBomb::Reset( void )
{
	m_pTex = NULL;
	m_iFrame = 0;
	m_flLastFrame = 0;
}

void CAlphaBomb::LoopEffect( void )
{
	cl_entity_t *pEnt;
	for( int i = 1; i < 512; i++ )
	{
		pEnt = gEngfuncs.GetEntityByIndex( i );

		if( pEnt )
		{
			if( pEnt->model )
			{
				if( !stricmp( pEnt->model->name, "models/w_alphabomb.mdl" ) )
				{
					int iContents = gEngfuncs.PM_PointContents( pEnt->origin, NULL );
					if( pEnt->curstate.msg_time + 1 > gEngfuncs.GetClientTime( ) )
					{
						if( iContents == CONTENTS_EMPTY || iContents == CONTENTS_WATER )
						{
							Render( pEnt );
						}
					}
				}
			}
		}
	}
}

void CAlphaBomb::Render( cl_entity_t *pEnt )
{
	vec3_t vOrg = pEnt->origin;

	g_CGLTriAPI.Texture2D( m_pTex, m_iFrame );

	g_CGLTriAPI.Color4f( 1, 1, 1, 1 );
	g_CGLTriAPI.CullFace( TRI_NONE );
	g_CGLTriAPI.RenderMode( kRenderTransAdd );
	g_CGLTriAPI.DisableDepthMask( );

//	gEngfuncs.Con_Printf( "Charge: %f\n", pEnt->curstate.frame );
	vec3_t vOrg1, vOrg2, vOrg3, vOrg4;
	
	
	if( IEngineStudio.IsHardware( ) )
	{
		// Draw 3 quads
		g_CGLTriAPI.Begin( TRI_QUADS );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 1, 1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			vOrg2 = ( vec3_t( -1, 1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg2 );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( -1, -1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			vOrg4 = ( vec3_t( 1, -1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg4 );
		g_CGLTriAPI.End( );



		g_CGLTriAPI.Begin( TRI_QUADS );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 1, 0, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			vOrg2 = ( vec3_t( -1, 0, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg2 );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( -1, 0, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			vOrg4 = ( vec3_t( 1, 0, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg4 );
		g_CGLTriAPI.End( );

		g_CGLTriAPI.Begin( TRI_QUADS );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 0, 1, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			vOrg2 = ( vec3_t( 0, -1, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg2 );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( 0, -1, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			vOrg4 = ( vec3_t( 0, 1, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg4 );
		g_CGLTriAPI.End( );

		g_CGLTriAPI.EnableDepthMask( );
	}
	else
	{
	// Draw 3 quads
		g_CGLTriAPI.Begin( TRI_TRIANGLES );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 1, 1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			vOrg2 = ( vec3_t( -1, 1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg2 );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( -1, -1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
		g_CGLTriAPI.End( );


		g_CGLTriAPI.Begin( TRI_TRIANGLES );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( -1, -1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			vOrg4 = ( vec3_t( 1, -1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg4 );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 1, 1, 0 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
		g_CGLTriAPI.End( );


		g_CGLTriAPI.Begin( TRI_TRIANGLES );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 1, 0, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			vOrg2 = ( vec3_t( -1, 0, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg2 );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( -1, 0, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
		g_CGLTriAPI.End( );

		g_CGLTriAPI.Begin( TRI_TRIANGLES );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( -1, 0, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			vOrg4 = ( vec3_t( 1, 0, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg4 );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 1, 0, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
		g_CGLTriAPI.End( );

		g_CGLTriAPI.Begin( TRI_TRIANGLES );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 0, 1, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			vOrg2 = ( vec3_t( 0, -1, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg2 );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( 0, -1, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
		g_CGLTriAPI.End( );

		g_CGLTriAPI.Begin( TRI_TRIANGLES );
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			vOrg3 = ( vec3_t( 0, -1, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg3 );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			vOrg4 = ( vec3_t( 0, 1, -1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg4 );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			vOrg1 = ( vec3_t( 0, 1, 1 ) * ( pEnt->curstate.frame * MAX_ALPHABOMB ) );
			g_CGLTriAPI.Vertex3fv( vOrg + vOrg1 );
		g_CGLTriAPI.End( );
	}


	if( m_flLastFrame < gEngfuncs.GetClientTime( ) )
	{
		m_iFrame ++;

		m_flLastFrame = gEngfuncs.GetClientTime( ) + ALPHA_FRAME;
		if( m_iFrame >= m_pTex->iFrames )
		{
			m_iFrame = 0;
		}
	}
}
