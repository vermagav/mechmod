/**********************************************************
*			EXPLOSION EFFECTS
***********************************************************
*	Purpose: Std Explosion effect
*	Created On: 1/14/04
*	Last Edited: 1/14/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "pmtrace.h"
#include "pm_defs.h"
#include "triangleapi.h"
#include "com_model.h"
#include "r_efx.h"
#include "event_api.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "psys_man.h"
#include "eff_tracerbullet.h"
#include "eff_explosion.h"
#include "eff_all.h"
#include "gl-triapi.h"


CEffTracerBullet::CEffTracerBullet()
{	m_pTex = gTexture.LoadTexture( "flare1" ); //g_SprTex.Find( "sprites/flare1.spr" );	
	m_flStart = gEngfuncs.GetClientTime( );
	m_flLife = 1.0f;
	m_flPrevTime = m_flCurTime = gEngfuncs.GetClientTime( );
	m_flDistTime = 0;
	m_pTex1 = gTexture.LoadTexture( "blueredmuzzle" );
	//m_pTex2 = gTexture.LoadTexture( "greenmuzzle" );
	//Modified by grOOvy: 29/10/04
	m_pTex2 = gTexture.LoadTexture( "laser1" );
	
}

void CEffTracerBullet::RenderTracer( Particle_s *pPart )
{
	vec3_t vPoint, vecStart, vecEnd;

	// Get points of tracer
	vecStart = pPart->vOrg - pPart->vVel.Normalize() * 45;
	vecEnd = pPart->vOrg + pPart->vVel.Normalize() * 45;

	// Setup drawing

	g_CGLTriAPI.Texture2D( pPart->pTex, pPart->iCurFrame );
	//gEngfuncs.pTriAPI->SpriteTexture( pPart->pTex /*m_pTex->pModel*/, pPart->iCurFrame );
	gEngfuncs.pTriAPI->CullFace( TRI_NONE );
	gEngfuncs.pTriAPI->Begin( TRI_QUADS );
	gEngfuncs.pTriAPI->Color4f( pPart->clColor.r, pPart->clColor.g, pPart->clColor.b, pPart->clColor.a );

	// Draw horizontal strip
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	vPoint = vecEnd + ( pPart->vRight * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	vPoint = vecStart + ( pPart->vRight * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	vPoint = vecStart - ( pPart->vRight * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	vPoint = vecEnd - ( pPart->vRight * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	// Draw verticle strip
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	vPoint = vecEnd + ( pPart->vUp * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );
	
	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	vPoint = vecStart + ( pPart->vUp * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	vPoint = vecStart - ( pPart->vUp * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	vPoint = vecEnd - ( pPart->vUp * pPart->flSize);
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	// Draw flat strip
	float flSize = pPart->flSize / 2;
	gEngfuncs.pTriAPI->TexCoord2f( 1, 0 );
	vPoint = pPart->vOrg - ( pPart->vUp * flSize ) + ( pPart->vRight * flSize );
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );
	
	gEngfuncs.pTriAPI->TexCoord2f( 0, 0 );
	vPoint = pPart->vOrg - ( pPart->vUp * flSize ) - ( pPart->vRight * flSize );
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 0, 1 );
	vPoint = pPart->vOrg + ( pPart->vUp * flSize ) - ( pPart->vRight * flSize );
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->TexCoord2f( 1, 1 );
	vPoint = pPart->vOrg + ( pPart->vUp * flSize ) + ( pPart->vRight * flSize );
	gEngfuncs.pTriAPI->Vertex3fv( vPoint );

	gEngfuncs.pTriAPI->End( );
}

void CEffTracerBullet::RenderPart( Particle_s *pPart )
{	// render quad
	if( m_pTex )
	{
		RenderTracer( pPart );
	}
	else
	{
		m_pTex = gTexture.LoadTexture( "flare1" );
	}
}

bool CEffTracerBullet::TestSystem( void )
{
	// make sure the system should be running
	if( !m_pTex )
	{
		m_pTex = gTexture.LoadTexture( "flare1" ); //g_SprTex.Find( "sprites/flare1.spr" );
	}
	return true;
}

void CEffTracerBullet::UpdateSystem( void )
{
	//mothing to really update as of now
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	m_flPrevTime = m_flCurTime;

	// choose the correct view
	if( CL_IsThirdPerson() )
	{	
		gEngfuncs.pfnAngleVectors( g_CameraAngles, NULL, m_vRight, m_vUp );	
	}
	else
	{	//get angle to player
		m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
		gEngfuncs.pfnAngleVectors( m_vAng, NULL, m_vRight, m_vUp );		
	}
}

bool CEffTracerBullet::TestPart( Particle_s *pPart )
{
	// make sure the particle is worth rendering
	if( g_PartEffects.Collision( pPart ) )
	{
		return false;
	}

	if( pPart->flLife + pPart->flSpawned < gEngfuncs.GetClientTime( ) )
	{
		return false;
	}
	return true;
}

void CEffTracerBullet::UpdatePart( Particle_s *pPart )
{
	// update everything thats worth updating
	g_PartEffects.CalcDeltas( pPart, m_flDistTime );
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;
}

// modifed by jason
void CEffTracerBullet::FillPart( vec3_t vecOrigin, vec3_t vecVelocity, int iType )
{
	// fill particle values
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vOrg = pPart->vPOrg = vecOrigin;
		pPart->flEdSize = pPart->flStSize = 2;
		pPart->flSize = 2.5;
		pPart->vVel = vecVelocity;
		pPart->clStCol.r = 1.0;
		pPart->clStCol.g = 1.0;
		pPart->clStCol.b = 1.0;
		pPart->clStCol.a = 1.0;
		pPart->clEdCol.r = 1.0;
		pPart->clEdCol.g = 1.0;
		pPart->clEdCol.b = 1.0;
		pPart->clEdCol.a = 1.0;
		pPart->clColor.r = 1.0;
		pPart->clColor.g = 1.0;
		pPart->clColor.b = 1.0;
		pPart->clColor.a = 1.0;
		pPart->flGrav = 0;
		pPart->iCurFrame = 0;		// current frame for sprite

		pPart->flLife = 10;
		pPart->flSpawned = gEngfuncs.GetClientTime( );

		if( iType == 0 )
		{
			if( m_pTex )
				pPart->pTex = m_pTex;
			else
				pPart->pTex = m_pTex = gTexture.LoadTexture( "flare1" );//g_SprTex.Find( "sprites/flare1.spr" );
		}
		else if( iType == 1 )
		{
			if( m_pTex1 )
				pPart->pTex = m_pTex1;
			else
				pPart->pTex = m_pTex1 = gTexture.LoadTexture( "blueredmuzzle" );
		}
		else
		{
			if( m_pTex2 )
				pPart->pTex = m_pTex2;
			else
				pPart->pTex = m_pTex2 = gTexture.LoadTexture( "laser1" );
				//Modified by grOOvy: 29/10/04
				//pPart->pTex = m_pTex2 = gTexture.LoadTexture( "greenmuzzle" );
				
		}
	}
}
