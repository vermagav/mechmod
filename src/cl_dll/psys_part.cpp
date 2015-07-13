/**********************************************************
*			PARTICLE SYSTEM FRAMEWORK
***********************************************************
*	Purpose: Base Particle Info for inherited classes
*	Created On: 1/4/04
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
#include "gl-triapi.h"


CParticleSystem::CParticleSystem( void )
{
	// constructor init everything
	m_pHeadPart = NULL;
	m_blSpawn = true;
	m_iSystem = 0;
	m_pTex = NULL;
}

CParticleSystem::~CParticleSystem( void )
{
	// destructor remove everything
	Particle_s *pPart = m_pHeadPart;

	while( pPart )
	{
		Particle_s *pTmp = pPart->pNext;
		ReturnToPool( pPart );
		pPart = pTmp;
	}
}

void CParticleSystem::ReturnToPool( Particle_s *pPart )
{
	//return to the pool
	//first remove from current place in linked list by modifying the prev and next to link
	if( pPart->pPrev )
	{
		pPart->pPrev->pNext = pPart->pNext;
	}
	if( pPart->pNext )
	{
		pPart->pNext->pPrev = pPart->pPrev;
	}
	if( pPart == m_pHeadPart )
	{
		m_pHeadPart = pPart->pNext;
	}

	g_PartPool.ReturnToPool( pPart );
}

void CParticleSystem::ReturnAllParts( void )
{
	// return all the particles to the pool
	Particle_s *pPart = m_pHeadPart, *pTmp;

	while( pPart )
	{
		pTmp = pPart->pNext;
		ReturnToPool( pPart );
		pPart = pTmp;
	}

}

Particle_s *CParticleSystem::AddPart( void )
{
	//add a new particle to the linked list from the pool

	Particle_s *pPart = g_PartPool.DrawFromPool( );

	if( pPart )
	{
		pPart->pPrev = NULL;
		pPart->pNext = m_pHeadPart;
		if( m_pHeadPart )
		{
			m_pHeadPart->pPrev = pPart;
		}
		m_pHeadPart = pPart;
	}

	return pPart;
}

bool CParticleSystem::TestVisability( Particle_s *pPart )
{
	//checks if the particle being drawn is in screen space 
	vec3_t vScr;
	
	return ( ( gEngfuncs.pTriAPI->WorldToScreen( pPart->vOrg, vScr ) ) != 1 );
}


bool CParticleSystem::RenderSystem( void )
{
	// heres where we loop through the system and render it
	if( TestSystem( ) )
	{
		// if test works update it
		UpdateSystem( );

		//now we loop thorugh the parts in the system and render them
		Particle_s *pPart = m_pHeadPart;
		
		int iCount = 0;

		PreRender( );
		while( pPart )
		{
			// first we need to test if the particle should still be living
			Particle_s *pTmp = pPart->pNext;
			if( TestPart( pPart ) )
			{
				UpdatePart( pPart );
				if( TestVisability( pPart ) )
				{
				// then we should update the particle	
					// now draw it
					RenderPart( pPart );
					iCount ++;
				}
			}
			else
			{
				// test show no need for particle remove it
				ReturnToPool( pPart );	
			}
			//move to the next part
			pPart = pTmp;
		}
		// turn it off
		PostRender( );
	//	if( iCount >= 5000 )
		/*{
			gEngfuncs.Con_Printf( "Parts: %i\n", iCount );
		}*/
	}
	else
	{
		//we can break out system doesnt need to be around any more
		return false;
	}

	//every thing was successfull
	return true;
}
void CParticleSystem::PreRender( void )
{
	// turn on blending
	// TODO DEPTH BASED SHADER TO FIX STUFF WITH CG :(
	g_CGLTriAPI.CullFace( TRI_FRONT );
	g_CGLTriAPI.RenderMode( kRenderTransAdd );
	g_CGLTriAPI.DisableDepthMask( );
	//gEngfuncs.pTriAPI->CullFace( TRI_FRONT );
	//gEngfuncs.pTriAPI->RenderMode( kRenderTransAdd );
}

void CParticleSystem::PostRender( void )
{
	//gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
	g_CGLTriAPI.RenderMode( kRenderNormal );
	//g_CGLTriAPI.DisableDepthMask( );
}
