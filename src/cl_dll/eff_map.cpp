/**********************************************************
*			EFFECTS EXTENDED TO MAPPER
***********************************************************
*	Purpose: Point and brush based system for the mapper to 
*			use
*	Created On: 1/6/04
*	Last Edited: 1/6/03
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
#include "eff_all.h"
#include "eff_map.h"
#include "eff_def.h"

CEffBrush::CEffBrush( vec3_t vOrg, vec3_t vMaxVel, vec3_t vMinVel, vec3_t vPMaxVel, vec3_t vPMinVel,
				vec3_t vAng, colorRGBA clEdCol, colorRGBA clStCol, float flBncScl, float flGrav,
				float flLife, float flFR, float flStSize, float flEdSize, float flSysLife, float flDelay,
				float flPartDelay, int iBounces, int iFlags, int iIndex, int iPPS, int iAngLock, char *pTex )
{
	// init our vars
	// mostly jstu fill in our base part
	m_flPrevTime = m_flCurTime = gEngfuncs.GetClientTime( );

	// system values
	m_iSystem = EFF_BRUSH;
	m_iIndex = iIndex;
	m_iPartsPerSec = iPPS;
	m_flLife = flSysLife;
	m_flDelay = flDelay;
	m_flPartDelay = flPartDelay;
	m_iAngLock	= iAngLock;

	// particle attributes
	m_ptPart.vOrg = vOrg;
	m_ptPart.flSize = m_ptPart.flStSize = flStSize;
	m_ptPart.flEdSize = flEdSize;
	m_ptPart.flBounceScl = flBncScl;
	m_ptPart.clEdCol = clEdCol;
	m_ptPart.clStCol = clStCol;
	m_ptPart.clColor = clStCol;
	if( flGrav != 0.0f )
	{
		m_ptPart.flGrav = 1/flGrav;
	}
	else
	{
		m_ptPart.flGrav = flGrav;
	}
	m_ptPart.flFrameRate = flFR;
	m_ptPart.flLife = flLife;
	m_ptPart.iBounces = iBounces;
	m_ptPart.iFlags = iFlags;
	// TODO STRIP SPRITES/*.SPR OFF FILE
	m_ptPart.pTex = gTexture.LoadTexture( pTex ); //g_SprTex.Find( pTex );
	m_ptPart.vAng = vAng;
	m_ptPart.vPMaxVel = vPMaxVel;
	m_ptPart.vPMinVel = vPMinVel;
	m_ptPart.vMaxVel = vMaxVel;
	m_ptPart.vMinVel = vMinVel;
	m_ptPart.vVel = vec3_t( 0,0,0 );

	m_pEnt = gEngfuncs.GetEntityByIndex( m_iIndex );
}


bool CEffBrush::TestSystem( void )
{
	// jsut make sure system is alive
	if( !m_ptPart.pTex )
	{
		return false;
	}
	if( m_flStart + m_flLife < gEngfuncs.GetClientTime( ) )
	{
		//if system has life make sure it can be killed
		if( m_flLife != -1 )
		{
			if( !GetHead( ) )
			{
				m_blSpawn = false;
				return false;
			}
		}
	}

	// otherwise the system is good to go
	return true;
}

void CEffBrush::UpdateSystem( void )
{
	// spawn more particles here
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	// for if the system is dead but we need to keep it running for existing particles
	if( m_blSpawn )
	{
		//if there is a delay
		if( m_flLastPart + m_flPartDelay < m_flCurTime || m_flPartDelay < 0 )
		{
			for( int iCount = 0; iCount < int( m_iPartsPerSec * m_flDistTime ) + 1; iCount ++ )
			{
				// creates a new particle
				FillPart( );
			}
		}
	}

	m_flPrevTime = m_flCurTime;

	// choose the correct view
	if( m_iAngLock == ANG_LOCK_NONE )
	{
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
	else if( m_iAngLock == ANG_LOCK_X )
	{
		vec3_t vTmp = vec3_t( 0,0,0 );
		if( CL_IsThirdPerson() )
		{	
			vTmp.x = g_CameraAngles.x;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );	
		}
		else
		{	//get angle to player
			m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
			vTmp.x = m_vAng.x;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );		
		}
	}
	else if( m_iAngLock == ANG_LOCK_Y )
	{
		vec3_t vTmp = vec3_t( 0,0,0 );
		if( CL_IsThirdPerson() )
		{	
			vTmp.y = g_CameraAngles.y;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );	
		}
		else
		{	//get angle to player
			m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
			vTmp.y = m_vAng.y;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );		
		}
	}
	else if( m_iAngLock == ANG_LOCK_Z )
	{
		vec3_t vTmp = vec3_t( 0,0,0 );
		if( CL_IsThirdPerson() )
		{	
			vTmp.z = g_CameraAngles.z;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );	
		}
		else
		{	//get angle to player
			m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
			vTmp.z = m_vAng.z;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );		
		}
	}
	else
	{
		// specified angles
		m_vAng = m_ptPart.vAng;
		gEngfuncs.pfnAngleVectors( m_vAng, NULL, m_vRight, m_vUp );
	}
}

bool CEffBrush::TestPart( Particle_s *pPart )
{
	bool blTmp = true;
	// its ok
	if( pPart->iFlags & EFF_FLAGS_BNC )
	{
		if( pPart->iBounces < 0 )
		{
			return false;
		}
	}
	else
	{
		// do collsion test
		if( pPart->iFlags & EFF_FLAGS_COL )
		{
			blTmp = !g_PartEffects.Collision( pPart );
		}
	}

	if( pPart->iFlags & EFF_FLAGS_AOC )
	{
		if( pPart->iBounces < 0 )
		{
			return false;
		}
	}

	if( pPart->iFlags & EFF_FLAGS_LIFE )
	{
		if( pPart->flSpawned + pPart->flLife < gEngfuncs.GetClientTime( ) )
		{
			return false;
		}
	}

	return blTmp;
	
}

void CEffBrush::UpdatePart( Particle_s *pPart )
{
	// update gravity along with all the other things we need to update
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel = pPart->vVel + vec3_t( gEngfuncs.pfnRandomFloat( pPart->vPMinVel.x, pPart->vPMaxVel.x ),
							gEngfuncs.pfnRandomFloat( pPart->vPMinVel.y, pPart->vPMaxVel.y ),
							gEngfuncs.pfnRandomFloat( pPart->vPMinVel.z, pPart->vPMaxVel.z ) );

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;

	if( pPart->iFlags & EFF_FLAGS_LIFE )
	{
		g_PartEffects.CalcDeltas( pPart, m_flDistTime );
	}
	if( pPart->iFlags & EFF_FLAGS_BNC )
	{
		if( g_PartEffects.Collision( pPart ) )
		{
			g_PartEffects.Bounce( pPart );
		}
	}

	if( pPart->iFlags & EFF_FLAGS_AOC )
	{
		g_PartEffects.AnimOnCol( pPart );
	}
	else
	{
		g_PartEffects.Anim( pPart );
	}
}

void CEffBrush::RenderPart( Particle_s *pPart )
{
	// render the quad
	g_PartEffects.RenderQuad( pPart );
}

void CEffBrush::FillPart( void )
{
	//fill particle with all sorts of crap
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vPOrg = pPart->vOrg = m_pEnt->curstate.origin + vec3_t( gEngfuncs.pfnRandomFloat( m_pEnt->curstate.mins.x, m_pEnt->curstate.maxs.x ),
											gEngfuncs.pfnRandomFloat( m_pEnt->curstate.mins.y, m_pEnt->curstate.maxs.y ),
											gEngfuncs.pfnRandomFloat( m_pEnt->curstate.mins.z, m_pEnt->curstate.maxs.z ) );
		pPart->flStSize = m_ptPart.flStSize;
		pPart->flSize = m_ptPart.flSize;
		pPart->flEdSize = m_ptPart.flEdSize;
		pPart->flBounceScl = m_ptPart.flBounceScl;
		pPart->clEdCol = m_ptPart.clEdCol;
		pPart->clStCol = m_ptPart.clStCol;
		pPart->clColor = m_ptPart.clColor;
		pPart->flGrav = m_ptPart.flGrav;			// gotta slow gravity down some
		pPart->iCurFrame = 0;						// current frame for sprite
		pPart->flLastAnim = 0;
		pPart->flSpawned = 0;
		pPart->flFrameRate = m_ptPart.flFrameRate;
		pPart->flLife = m_ptPart.flLife;
		pPart->iBounces = m_ptPart.iBounces;
		pPart->iFlags = m_ptPart.iFlags;
		pPart->pTex = m_ptPart.pTex;
		pPart->vAng = m_ptPart.vAng;
		pPart->vPMaxVel = m_ptPart.vPMaxVel;
		pPart->vPMinVel = m_ptPart.vPMinVel;
		pPart->vMaxVel = m_ptPart.vMaxVel;
		pPart->vMinVel = m_ptPart.vMinVel;
		pPart->vVel = vec3_t( gEngfuncs.pfnRandomFloat( pPart->vMinVel.x, pPart->vMaxVel.x ),
							gEngfuncs.pfnRandomFloat( pPart->vMinVel.y, pPart->vMaxVel.y ),
							gEngfuncs.pfnRandomFloat( pPart->vMinVel.z, pPart->vMaxVel.z ) );
		pPart->flSpawned = gEngfuncs.GetClientTime( );
	}
}


CEffPoint::CEffPoint( vec3_t vOrg, vec3_t vMaxVel, vec3_t vMinVel, vec3_t vPMaxVel, vec3_t vPMinVel,
				vec3_t vAng, colorRGBA clEdCol, colorRGBA clStCol, float flBncScl, float flGrav,
				float flLife, float flFR, float flStSize, float flEdSize, float flSysLife, float flDelay,
				float flPartDelay, int iBounces, int iFlags, int iIndex, int iPPS, int iAngLock, char *pTex )
{
	// init our vars
	// mostly jstu fill in our base part
	m_flPrevTime = m_flCurTime = gEngfuncs.GetClientTime( );

	// system values
	m_iSystem = EFF_BRUSH;
	m_iIndex = iIndex;
	m_iPartsPerSec = iPPS;
	m_flLife = flSysLife;
	m_flDelay = flDelay;
	m_flPartDelay = flPartDelay;
	m_iAngLock	= iAngLock;

	// particle attributes
	m_ptPart.vOrg = vOrg;
	m_ptPart.flSize = m_ptPart.flStSize = flStSize;
	m_ptPart.flEdSize = flEdSize;
	m_ptPart.flBounceScl = flBncScl;
	m_ptPart.clEdCol = clEdCol;
	m_ptPart.clStCol = clStCol;
	m_ptPart.clColor = clStCol;
	m_ptPart.flGrav = 1/flGrav;
	m_ptPart.flFrameRate = flFR;
	m_ptPart.flLife = flLife;
	m_ptPart.iBounces = iBounces;
	m_ptPart.iFlags = iFlags;
	// TODO STRIP SPRITES/*.SPR OFF FILE
	m_ptPart.pTex = gTexture.LoadTexture( pTex );
	m_ptPart.vAng = vAng;
	m_ptPart.vPMaxVel = vPMaxVel;
	m_ptPart.vPMinVel = vPMinVel;
	m_ptPart.vMaxVel = vMaxVel;
	m_ptPart.vMinVel = vMinVel;
	m_ptPart.vVel = vec3_t( 0,0,0 );

	m_pEnt = gEngfuncs.GetEntityByIndex( m_iIndex );
}


bool CEffPoint::TestSystem( void )
{
	// jsut make sure system is alive
	if( !m_ptPart.pTex )
	{
		return false;
	}
	if( g_PartEffects.Collision( &m_ptPart ) )
	{
		return false;
	}
	if( m_flStart + m_flLife < gEngfuncs.GetClientTime( ) )
	{
		//if system has life make sure it can be killed
		if( m_flLife != -1 )
		{
			if( !GetHead( ) )
			{
				m_blSpawn = false;
				return false;
			}
		}
	}

	// otherwise the system is good to go
	return true;
}

void CEffPoint::UpdateSystem( void )
{
	// spawn more particles here
	m_flCurTime = gEngfuncs.GetClientTime( );

	m_flDistTime = m_flCurTime - m_flPrevTime;

	// for if the system is dead but we need to keep it running for existing particles
	if( m_blSpawn )
	{
		//if there is a delay
		if( m_flLastPart + m_flPartDelay < m_flCurTime || m_flPartDelay <= 0 )
		{
			for( int iCount = 0; iCount < int( m_iPartsPerSec * m_flDistTime ) + 1; iCount ++ )
			{
				// creates a new particle
				FillPart( );
			}
		}
	}

	m_flPrevTime = m_flCurTime;

	// choose the correct view
	if( m_iAngLock == ANG_LOCK_NONE )
	{
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
	else if( m_iAngLock == ANG_LOCK_X )
	{
		vec3_t vTmp = vec3_t( 0,0,0 );
		if( CL_IsThirdPerson() )
		{	
			vTmp.x = g_CameraAngles.x;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );	
		}
		else
		{	//get angle to player
			m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
			vTmp.x = m_vAng.x;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );		
		}
	}
	else if( m_iAngLock == ANG_LOCK_Y )
	{
		vec3_t vTmp = vec3_t( 0,0,0 );
		if( CL_IsThirdPerson() )
		{	
			vTmp.y = g_CameraAngles.y;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );	
		}
		else
		{	//get angle to player
			m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
			vTmp.y = m_vAng.y;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );		
		}
	}
	else if( m_iAngLock == ANG_LOCK_Z )
	{
		vec3_t vTmp = vec3_t( 0,0,0 );
		if( CL_IsThirdPerson() )
		{	
			vTmp.z = g_CameraAngles.z;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );	
		}
		else
		{	//get angle to player
			m_vAng = gEngfuncs.GetLocalPlayer( )->curstate.angles;
			vTmp.z = m_vAng.z;
			gEngfuncs.pfnAngleVectors( vTmp, NULL, m_vRight, m_vUp );		
		}
	}
	else
	{
		// specified angles
		m_vAng = m_ptPart.vAng;
		gEngfuncs.pfnAngleVectors( m_vAng, NULL, m_vRight, m_vUp );
	}
}

bool CEffPoint::TestPart( Particle_s *pPart )
{
	bool blTmp = true;
	// its ok
	if( pPart->iFlags & EFF_FLAGS_BNC )
	{
		if( pPart->iBounces < 0 )
		{
			return false;
		}
	}
	else
	{
		// do collsion test
		if( pPart->iFlags & EFF_FLAGS_COL )
		{
			blTmp = !g_PartEffects.Collision( pPart );
		}
	}

	if( pPart->iFlags & EFF_FLAGS_AOC )
	{
		if( pPart->iBounces < 0 )
		{
			return false;
		}
	}

	if( pPart->iFlags & EFF_FLAGS_LIFE )
	{
		if( pPart->flSpawned + pPart->flLife < gEngfuncs.GetClientTime( ) )
		{
			return false;
		}
	}

	return blTmp;
	
}

void CEffPoint::UpdatePart( Particle_s *pPart )
{
	// update gravity along with all the other things we need to update
	pPart->vPOrg = pPart->vOrg;

	pPart->vVel = pPart->vVel + vec3_t( gEngfuncs.pfnRandomFloat( pPart->vPMinVel.x, pPart->vPMaxVel.x ),
							gEngfuncs.pfnRandomFloat( pPart->vPMinVel.y, pPart->vPMaxVel.y ),
							gEngfuncs.pfnRandomFloat( pPart->vPMinVel.z, pPart->vPMaxVel.z ) );

	pPart->vVel.z += -800 * m_flDistTime * pPart->flGrav;

	pPart->vOrg = pPart->vOrg + pPart->vVel * m_flDistTime;

	pPart->vUp = m_vUp;
	pPart->vRight = m_vRight;

	if( pPart->iFlags & EFF_FLAGS_LIFE )
	{
		g_PartEffects.CalcDeltas( pPart, m_flDistTime );
	}
	if( pPart->iFlags & EFF_FLAGS_BNC )
	{
		if( g_PartEffects.Collision( pPart ) )
		{
			g_PartEffects.Bounce( pPart );
		}
	}

	if( pPart->iFlags & EFF_FLAGS_AOC )
	{
		g_PartEffects.AnimOnCol( pPart );
	}
	else
	{
		g_PartEffects.Anim( pPart );
	}
}

void CEffPoint::RenderPart( Particle_s *pPart )
{
	// render the quad
	g_PartEffects.RenderQuad( pPart );
}

void CEffPoint::FillPart( void )
{
	//fill particle with all sorts of crap
	Particle_s *pPart = AddPart( );

	//next we fill in the vars
	if( pPart )
	{
		pPart->vPOrg = pPart->vOrg = m_ptPart.vOrg;
		pPart->flStSize = m_ptPart.flStSize;
		pPart->flSize = m_ptPart.flSize;
		pPart->flEdSize = m_ptPart.flEdSize;
		pPart->flBounceScl = m_ptPart.flBounceScl;
		pPart->clEdCol = m_ptPart.clEdCol;
		pPart->clStCol = m_ptPart.clStCol;
		pPart->clColor = m_ptPart.clColor;
		pPart->flGrav = m_ptPart.flGrav;			// gotta slow gravity down some
		pPart->iCurFrame = 0;						// current frame for sprite
		pPart->flLastAnim = 0;
		pPart->flSpawned = 0;
		pPart->flFrameRate = m_ptPart.flFrameRate;
		pPart->flLife = m_ptPart.flLife;
		pPart->iBounces = m_ptPart.iBounces;
		pPart->iFlags = m_ptPart.iFlags;
		pPart->pTex = m_ptPart.pTex;
		pPart->vAng = m_ptPart.vAng;
		pPart->vPMaxVel = m_ptPart.vPMaxVel;
		pPart->vPMinVel = m_ptPart.vPMinVel;
		pPart->vMaxVel = m_ptPart.vMaxVel;
		pPart->vMinVel = m_ptPart.vMinVel;
		pPart->vVel = vec3_t( gEngfuncs.pfnRandomFloat( pPart->vMinVel.x, pPart->vMaxVel.x ),
							gEngfuncs.pfnRandomFloat( pPart->vMinVel.y, pPart->vMaxVel.y ),
							gEngfuncs.pfnRandomFloat( pPart->vMinVel.z, pPart->vMaxVel.z ) );
		pPart->flSpawned = gEngfuncs.GetClientTime( );
	}
}