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
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "eff_explosion.h"
#include "eff_all.h"
#include "eff_def.h"


CEffExplosion::CEffExplosion( vec3_t vOrg )
{
	m_vOrg = vOrg;		//set explosion origin
	m_pTex = gTexture.LoadTexture( "fexplo1" ); //g_SprTex.Find( "sprites/fexplo1.spr" );	
	m_flStart = gEngfuncs.GetClientTime( );
	m_flLife = 1.0f;
	m_flPrevTime = m_flCurTime = gEngfuncs.GetClientTime( );
	m_flPrevTime -= .25;
}

void CEffExplosion::RenderPart( Particle_s *pPart )
{
	// render quad
	g_PartEffects.RenderQuad( pPart );
}

bool CEffExplosion::TestSystem( void )
{
	// make sure the system should be running
	if( !m_pTex )
	{
		m_pTex = gTexture.LoadTexture( "flare3" );//g_SprTex.Find( "sprites/flare3.spr" );
	}

	if( m_flStart + m_flLife < gEngfuncs.GetClientTime( ) )
	{
		if( !GetHead( ) )
		{
			return false;
		}
	}
	return true;
}

void CEffExplosion::UpdateSystem( void )
{
	//mothing to really update as of now
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	if( m_flStart + m_flLife > gEngfuncs.GetClientTime( ) )
	{
		if( m_flDelay < gEngfuncs.GetClientTime( ) )
		{
			float flDelay = ( (m_flDistTime * ( m_flLife * ( 1/ m_flDistTime ) ) ) / EFF_EXPLO_PART  );
			for( int iCount = 0; iCount < 1; iCount ++ )
			{
				// creates a new particle
				FillPart( );
			}
			m_flDelay = flDelay + gEngfuncs.GetClientTime( );
		}
	}

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

bool CEffExplosion::TestPart( Particle_s *pPart )
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

void CEffExplosion::UpdatePart( Particle_s *pPart )
{
	// update everything thats worth updating
	g_PartEffects.Anim( pPart );
	g_PartEffects.CalcDeltas( pPart, m_flDistTime );
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;
}

void CEffExplosion::FillPart( void )
{
	// fill particle values
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vOrg = pPart->vPOrg = m_vOrg;
		pPart->flEdSize = pPart->flStSize = 15;
		pPart->flSize = 15;
		pPart->vVel = g_PartEffects.RandomRoundVec( vec3_t( -7, -7, -7 ), vec3_t( 7, 7, 7 ) );
		pPart->clStCol.r = 1.0;
		pPart->clStCol.g = 1.0;
		pPart->clStCol.b = 1.0;
		pPart->clStCol.a = 1.0;
		pPart->clEdCol.r = 1.0;
		pPart->clEdCol.g = 1.0;
		pPart->clEdCol.b = 1.0;
		pPart->clEdCol.a = 0.2;
		pPart->clColor.r = 1.0;
		pPart->clColor.g = 1.0;
		pPart->clColor.b = 1.0;
		pPart->clColor.a = 1.0;
		pPart->flGrav = 0;
		pPart->iCurFrame = 0;		// current frame for sprite

		pPart->flLife = 2.5;
		pPart->flSpawned = gEngfuncs.GetClientTime( );

		pPart->pTex = m_pTex;
	}
}


CEffWastedExp::CEffWastedExp( vec3_t vOrg, float flSize, int iType )
{
	m_flSize = flSize/7;
	m_vOrg = vOrg;		//set explosion origin
	if( iType == EFF_WASTEDEXP )
		m_pTex = gTexture.LoadTexture( "fexplo1" ); //g_SprTex.Find( "sprites/fexplo1.spr" );
	else
		m_pTex = gTexture.LoadTexture( "alphabomb" ); //g_SprTex.Find( "sprites/alphabomb.spr" );
	m_flStart = gEngfuncs.GetClientTime( );
	m_flLife = 1.5f;
	m_flPrevTime = m_flCurTime = gEngfuncs.GetClientTime( );
	m_flPrevTime -= .25;
}

void CEffWastedExp::RenderPart( Particle_s *pPart )
{
	// render quad
	g_PartEffects.RenderQuad( pPart );
}

bool CEffWastedExp::TestSystem( void )
{
	// make sure the system should be running
	if( !m_pTex )
	{
		m_pTex = gTexture.LoadTexture( "flare3" ); //g_SprTex.Find( "sprites/flare3.spr" );
	}

	if( m_flStart + m_flLife < gEngfuncs.GetClientTime( ) )
	{
		if( !GetHead( ) )
		{
			return false;
		}
	}
	return true;
}

void CEffWastedExp::UpdateSystem( void )
{
	//mothing to really update as of now
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	if( m_flStart + m_flLife > gEngfuncs.GetClientTime( ) )
	{
		if( m_flDelay < gEngfuncs.GetClientTime( ) )
		{
			float flDelay = ( (m_flDistTime * ( m_flLife * ( 1/ m_flDistTime ) ) ) / 60.0f  );
			for( int iCount = 0; iCount < 1; iCount ++ )
			{
				// creates a new particle
				FillPart( );
			}
			m_flDelay = flDelay + gEngfuncs.GetClientTime( );
		}
	}

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

bool CEffWastedExp::TestPart( Particle_s *pPart )
{
	// make sure the particle is worth rendering
	/*if( g_PartEffects.Collision( pPart ) )
	{
		return false;
	}*/

	if( pPart->flLife + pPart->flSpawned < gEngfuncs.GetClientTime( ) )
	{
		return false;
	}
	return true;
}

void CEffWastedExp::UpdatePart( Particle_s *pPart )
{
	// update everything thats worth updating
	g_PartEffects.Anim( pPart );
	g_PartEffects.CalcDeltas( pPart, m_flDistTime );
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;
}

void CEffWastedExp::FillPart( void )
{
	// fill particle values
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vOrg = pPart->vPOrg = m_vOrg;
		pPart->flEdSize = 200;
		pPart->flStSize = 50;
		pPart->flSize = 50;

		pPart->vVel = g_PartEffects.RandomRoundVec( vec3_t( -m_flSize, -m_flSize, -m_flSize ),
												vec3_t( m_flSize, m_flSize, m_flSize ) );
		pPart->clStCol.r = 1.0;
		pPart->clStCol.g = 1.0;
		pPart->clStCol.b = 1.0;
		pPart->clStCol.a = 1.0;
		pPart->clEdCol.r = 1.0;
		pPart->clEdCol.g = 1.0;
		pPart->clEdCol.b = 1.0;
		pPart->clEdCol.a = 0.0;
		pPart->clColor.r = 1.0;
		pPart->clColor.g = 1.0;
		pPart->clColor.b = 1.0;
		pPart->clColor.a = 1.0;
		pPart->flGrav = 0;
		pPart->iCurFrame = 0;		// current frame for sprite
		pPart->flLife = 2.5f;
		pPart->flFrameRate = pPart->flLife/( float )m_pTex->iFrames;

		pPart->flSpawned = gEngfuncs.GetClientTime( );

		pPart->pTex = m_pTex;
	}
}

