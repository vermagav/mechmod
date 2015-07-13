/**********************************************************
*				SMOKE EFFECT
***********************************************************
*	Purpose: Smoke effect for missles
*	Created On: 1/18/04
*	Last Edited: 1/18/04
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
#include "eff_smoke.h"
#include "eff_all.h"
#include "eff_def.h"

CEffSmoke::CEffSmoke( int iIndex )
{
	m_flDistTime = 0;
	m_flCurTime = m_flPrevTime = gEngfuncs.GetClientTime( );
	m_iIndex = iIndex;
	m_pEnt = gEngfuncs.GetEntityByIndex( m_iIndex );

	m_pTex = gTexture.LoadTexture( "ballsmoke" ); //g_SprTex.Find( "sprites/ballsmoke.spr" );
	m_iSystem = EFF_SMOKE;

	m_blAlive = true;

	m_flDelay = gEngfuncs.GetClientTime( );
}

bool CEffSmoke::TestSystem( void )
{

	if( !m_blAlive )
	{
		if( !GetHead( ) )
		{
			return false;
		}
	}
	return true;
}

void CEffSmoke::UpdateSystem( void )
{
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	m_flPrevTime = m_flCurTime;
	
	if( m_blAlive )
	{
		if( m_flDelay < gEngfuncs.GetClientTime( ) )
		{
			FillPart( );
			m_flDelay = gEngfuncs.GetClientTime() + ( m_flDistTime ) ;
		}
	}

	

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

bool CEffSmoke::TestPart( Particle_s *pPart )
{
	if( pPart->iCurFrame == pPart->pTex->iFrames -1 )
	{
		return false;
	}
	
	if( pPart->flSpawned + pPart->flLife < gEngfuncs.GetClientTime( ))
	{
		return false;
	}
	return true;
}

void CEffSmoke::UpdatePart( Particle_s *pPart )
{
	g_PartEffects.Anim( pPart );
	g_PartEffects.CalcDeltas( pPart, m_flDistTime );
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;
}

void CEffSmoke::RenderPart( Particle_s *pPart )
{
	g_PartEffects.RenderQuad( pPart );
}

void CEffSmoke::FillPart( void )
{
	// fill particle values
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vOrg = m_pEnt->origin;
		pPart->flEdSize = 15;
		pPart->flStSize = 2;
		pPart->flSize = 2;
		pPart->vVel = vec3_t( 0,0,0 );
		pPart->clStCol.r = 0.5;
		pPart->clStCol.g = 0.5;
		pPart->clStCol.b = 0.5;
		pPart->clStCol.a = 1.0;
		pPart->clEdCol.r = 0.5;
		pPart->clEdCol.g = 0.5;
		pPart->clEdCol.b = 0.5;
		pPart->clEdCol.a = 0.0;
		pPart->clColor.r = 1.0;
		pPart->clColor.g = 1.0;
		pPart->clColor.b = 1.0;
		pPart->clColor.a = 1.0;
		pPart->flGrav = 0;
		pPart->iCurFrame = gEngfuncs.pfnRandomLong( 0, 3 );		// current frame for sprite
		pPart->iBounces = 1;
		pPart->flLife = 1;
		pPart->flFrameRate = pPart->flLife/m_pTex->iFrames;
		pPart->flLastAnim = gEngfuncs.GetClientTime() + pPart->flFrameRate;
		pPart->pTex = m_pTex;

		
		pPart->flSpawned = gEngfuncs.GetClientTime( );
	}
}