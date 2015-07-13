/**********************************************************
*			TEST EFFECT FOR PARTICLE SYSTEM
***********************************************************
*	Purpose: Test particle system
*	Created On: 1/5/04
*	Last Edited: 1/5/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "com_model.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "eff_test.h"
#include "eff_all.h"

CEffTest::CEffTest( void )
{
	// init our vars
	m_flDistTime = 0;
	m_flCurTime = m_flPrevTime = gEngfuncs.GetClientTime( );
	m_pTex = NULL;
}

bool CEffTest::TestSystem( void )
{
	//check if we are in something before spawning parts
	if( !m_pTex )
	{
		m_pTex = gTexture.LoadTexture( "flare3" ); //g_SprTex.Find( "sprites/flare3.spr" );
	}
	/*if( gEngfuncs.PM_PointContents( vec3_t( 0,0,0 ), NULL ) != CONTENTS_EMPTY )
	{
		return false;
	}*/
	return true;
}

void CEffTest::UpdateSystem( void )
{
	// spawn particles here
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	for( int iCount = 0; iCount < int( EFF_TEST_PART * m_flDistTime ) + 1; iCount ++ )
	{
		// creates a new particle
		FillPart( );
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

void CEffTest::FillPart( void )
{
	//first draw a particle from the pool
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vOrg = pPart->vPOrg = vec3_t( 0,0,0 );
		pPart->flSize = 5;
		pPart->vVel = vec3_t( gEngfuncs.pfnRandomLong( -100,100 ), gEngfuncs.pfnRandomLong( -100,100 ), gEngfuncs.pfnRandomLong( 0,1 ) );
		pPart->clColor.r = 1.0;
		pPart->clColor.g = 1.0;
		pPart->clColor.b = 1.0;
		pPart->clColor.a = 1.0;
		pPart->flGrav = 1/9.8;		//gotta slow gravity down some
		pPart->iCurFrame = 0;		// current frame for sprite

		pPart->pTex = m_pTex;
	}
}

bool CEffTest::TestPart( Particle_s *pPart )
{
	// make sure things are in working order
	// like collision

	if( !pPart->pTex )
	{
		return false;
	}

	if( gEngfuncs.PM_PointContents( pPart->vOrg, NULL ) != CONTENTS_EMPTY )
	{
		return false;
	}

	return true;
}

void CEffTest::UpdatePart( Particle_s *pPart )
{
	// update things on the particle like gravity
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;
}

void CEffTest::RenderPart( Particle_s *pPart )
{
	// wooo finnaly we render the thing
	g_PartEffects.RenderQuad( pPart );
}