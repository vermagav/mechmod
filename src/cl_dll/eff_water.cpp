/**********************************************************
*				WATER EFFECTS
***********************************************************
*	Purpose: Water effect for flyin over water
*	Created On: 1/20/04
*	Last Edited: 1/20/04
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
#include "eff_water.h"
#include "eff_all.h"

CEffWater::CEffWater( void )
{
	// Inialize our vars
	m_flDistTime = 0;
	m_flCurTime = m_flPrevTime = gEngfuncs.GetClientTime( );
	m_pTex = gTexture.LoadTexture( "splash" );//g_SprTex.Find( "sprites/splash.spr" );
}

bool CEffWater::TestSystem( void )
{
	// Test system for problems
	// we dont need to test this system should exist always

	// No problems
	return true;
}

void CEffWater::UpdateSystem( void )
{
	// Update system vars
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
	
	// check if player is over water if so spawn some parts at waters origin
	for( int iCount = 1; iCount <= MAX_PLAYERS; iCount ++ )
	{
		// find dist to water
		cl_entity_t *pEnt;
		if( iCount == 0 )
		{
			pEnt = gEngfuncs.GetLocalPlayer(  );
		}
		else
		{
			pEnt = gEngfuncs.GetEntityByIndex( iCount );
		}

		if( pEnt->player )
		{
			if( pEnt->curstate.movetype != MOVETYPE_PLAYERBOOST && pEnt->curstate.movetype != MOVETYPE_PLAYERFLY )
			{
				if( pEnt->curstate.velocity.x == 0 && pEnt->curstate.velocity.y == 0 )
				{
					continue;
				}
			}
			// do a 100 unit down traceline from the player
			vec3_t vEnd = pEnt->origin - vec3_t( 0,0, 300 );
			
			// do a trace to see if we can find water
			pmtrace_t *pTrace = gEngfuncs.PM_TraceLine( pEnt->origin, vEnd, PM_TRACELINE_ANYVISIBLE, 2, -1 );
			if( pTrace->fraction != 1.0 && pTrace->fraction != 0.0 )
			{
				// check if end pos is in water
				if( gEngfuncs.PM_PointContents( pTrace->endpos, NULL ) == CONTENTS_WATER )
				{
					// find waters surface to one unit
					vec3_t vCurOrg = pEnt->origin, vEndOrg = pTrace->endpos;
					vec3_t vDist = vEndOrg - vCurOrg;
					vec3_t vHalfDist;
					
					while( fabs( vDist.z ) > 2.0f )
					{
						vHalfDist = (vDist / 2) + vCurOrg;
						
						if( gEngfuncs.PM_PointContents( vHalfDist, NULL ) == CONTENTS_WATER )
						{
							vEndOrg = vHalfDist;
							
							vDist = vEndOrg - vCurOrg;
						}
						else
						{
							vCurOrg = vHalfDist;
							
							vDist = vEndOrg - vCurOrg;
						}
					}
					
					vec3_t vTmp = vDist - vCurOrg;
					m_flDist = fabs( pEnt->origin.z - vCurOrg.z ) ;
					m_vOrg = vCurOrg;
					for( int iUp = 0; iUp < int( fabs( m_flDist -300.0f ) / 75.0f ) + 1; iUp ++ )
					{
						FillPart( );
					}
				}
			}
			else if( pTrace->fraction == 1.0 )
			{
				// check if end pos is in water
				if( gEngfuncs.PM_PointContents( vEnd, NULL ) == CONTENTS_WATER )
				{
					// find waters surface to one unit
					vec3_t vCurOrg = pEnt->origin, vEndOrg = vEnd;
					vec3_t vDist = vEndOrg - vCurOrg;
					vec3_t vHalfDist;
					
					while( fabs( vDist.z ) > 2.0f )
					{
						vHalfDist = (vDist / 2) + vCurOrg;
						
						if( gEngfuncs.PM_PointContents( vHalfDist, NULL ) == CONTENTS_WATER )
						{
							vEndOrg = vHalfDist;
							
							vDist = vEndOrg - vCurOrg;
						}
						else
						{
							vCurOrg = vHalfDist;
							
							vDist = vEndOrg - vCurOrg;
						}
					}
					
					vec3_t vTmp = vDist - vCurOrg;
					m_flDist = fabs( pEnt->origin.z - vCurOrg.z) ;
					m_vOrg = vCurOrg + vec3_t( 0,0,1 );
					for( int iUp = 0; iUp <int( fabs( m_flDist -300.0f )/ 75.0f ) + 1; iUp ++ )
					{
						FillPart( );
					}
				}
			}
		}
	}
}

bool CEffWater::TestPart( Particle_s *pPart )
{
	// Test Particle make sure its worth drawing
	if( !m_pTex )
	{
		m_pTex = gTexture.LoadTexture( "splash" );//g_SprTex.Find( "sprites/splash.spr" );
	}
	if( gEngfuncs.PM_PointContents( pPart->vOrg, NULL ) != CONTENTS_EMPTY )
	{
		return false;
	}
	// No Problems
	return true;
}

void CEffWater::UpdatePart( Particle_s *pPart )
{
	// Update the particle
	//g_PartEffects.Anim( pPart );

	pPart->vPOrg = pPart->vOrg;

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;
}

void CEffWater::RenderPart( Particle_s *pPart )
{
	// Render the particle yay!!
	g_PartEffects.RenderQuad( pPart );
}

void CEffWater::FillPart( void )
{
	// Create a particle
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vOrg = pPart->vPOrg = m_vOrg;
		pPart->flSize = 10;
		float flDist = ( fabs( m_flDist - 300 )/ 300.0f ) * 120;
		pPart->vVel = g_PartEffects.RandomRoundVec( vec3_t( -flDist, -flDist, flDist ), 
						vec3_t( flDist, flDist, flDist ) );
		pPart->clColor.r = 1.0;
		pPart->clColor.g = 1.0;
		pPart->clColor.b = 1.0;
		pPart->clColor.a = 0.2f;
		pPart->flGrav = 1;		// gotta slow gravity down some
		pPart->iCurFrame = 0;			// current frame for sprite

		pPart->pTex = m_pTex;

		pPart->iCurFrame = gEngfuncs.pfnRandomLong( 0, pPart->pTex->iFrames -1 );
	}
}
