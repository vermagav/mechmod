/**********************************************************
*			NUKE EXPLOSION EFFECT
***********************************************************
*	Purpose: Std Explosion effect
*	Created On: 11/9/04
*	Last Edited: 11/9/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "pmtrace.h"
#include "pm_defs.h"
#include "triangleapi.h"
#include "com_model.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "eff_all.h"
#include "eff_nuke.h"


CExploNuke::CExploNuke( vec3_t vOrg )
{
	m_vOrg = vOrg + vec3_t( 0, 0, 2 );
	m_pTex = gTexture.LoadTexture( "nuke_base" );
	m_pTex1 = gTexture.LoadTexture( "nuke_middle" );
	m_pTex2 = gTexture.LoadTexture( "nuke_top" );
	m_flPrevTime = m_flCurTime = m_flStart = gEngfuncs.GetClientTime( );
	m_flLife = 10.0f;
}

void CExploNuke::RenderPart( Particle_s *pPart )
{
	g_PartEffects.RenderQuad( pPart );
}

bool CExploNuke::TestSystem( void )
{
	if( !m_pTex )
	{
		m_pTex = gTexture.LoadTexture( "flare3" );
	}
	if( !m_pTex1 )
	{
		m_pTex1 = gTexture.LoadTexture( "flare3" );
	}
	if( !m_pTex2 )
	{
		m_pTex2 = gTexture.LoadTexture( "flare3" );
	}

	// TODO : ADD LIFE
	return true;
}

void CExploNuke::UpdateSystem( void )
{
	//mothing to really update as of now
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	if( m_flStart + m_flLife > gEngfuncs.GetClientTime( ) )
	{
		if( m_flDelay < gEngfuncs.GetClientTime( ) )
		{
			float flDelay = ( (m_flDistTime *  m_flLife  ) / EFF_NUKE_SPRITES  );
		//	for( int iCount = 0; iCount < 1; iCount ++ )
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


bool CExploNuke::TestPart( Particle_s *pPart )
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

void CExploNuke::UpdatePart( Particle_s *pPart )
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

void CExploNuke::FillPart( void )
{
	// fill particle values
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( gEngfuncs.GetClientTime( ) - m_flStart < 2 )
	{
		// create low small particles
		if( pPart )
		{
			pPart->vOrg = pPart->vPOrg = m_vOrg;
			pPart->flEdSize = 50;
			pPart->flSize = pPart->flStSize = 16;
			pPart->vVel = g_PartEffects.RandomRoundVec( vec3_t( -16, -16, 4 ), vec3_t( 16, 16, 4 ) );
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

			pPart->flLife = 10.0f;
			pPart->flFrameRate = float(pPart->flLife/m_pTex->iFrames);
			pPart->flSpawned = gEngfuncs.GetClientTime( );

			pPart->pTex = m_pTex;
		}
	}
	else if( gEngfuncs.GetClientTime( ) - m_flStart < 6 )
	{
		// create rising funnel
		if( pPart )
		{
			pPart->vOrg = pPart->vPOrg = m_vOrg ;
			pPart->flEdSize = 26;
			pPart->flSize = pPart->flStSize = 26;
			pPart->vVel = g_PartEffects.RandomRoundVec( vec3_t( -8, -8, 60 ), vec3_t( 8, 8, 80 ) );
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

			pPart->flLife = 6.0f;
			pPart->flFrameRate = float(pPart->flLife/m_pTex1->iFrames);
			pPart->flSpawned = gEngfuncs.GetClientTime( );

			pPart->pTex = m_pTex1;
		}
	}
	else
	{
		// create big top particles
			pPart->vOrg = pPart->vPOrg = m_vOrg + vec3_t( 0, 0, 340 );
			pPart->flEdSize = 120;
			pPart->flSize = pPart->flStSize = 120;
			pPart->vVel = g_PartEffects.RandomRoundVec( vec3_t( -20, -20, -20 ), vec3_t( 20, 20, 20 ) );
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

			pPart->flLife = 2.5f;
			pPart->flFrameRate = float(pPart->flLife/m_pTex2->iFrames)/2.0f;
			pPart->flSpawned = gEngfuncs.GetClientTime( );

			pPart->pTex = m_pTex2;
	}
}