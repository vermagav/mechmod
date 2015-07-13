/**********************************************************
*			ENTITY TRAIL CODE
***********************************************************
*	Purpose: Enhanced entity trails
*	Created On: 3/1/04
*	Last Edited: 3/2/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "triangleapi.h"
#include "com_model.h"
#include "texture.h"
#include "ent_trail.h"
#include "ent_trail_def.h"
//#include "spr_texture.h"
#include	<windows.h>
#include	<gl/gl.h>

#include "gl-triapi.h"
// TODO: CONVERT TO NEW TEXTURE SYSTEM

extern vec3_t g_CameraAngles;

#define DLLEXPORT __declspec( dllexport )

extern "C"
{
	// Added by Pcjoe
	int DLLEXPORT CL_IsThirdPerson( void );
};


CEntTrail::CEntTrail( int iIndex )
{
	CEntTrail( );
	SetEnt( iIndex );
}

CEntTrail::CEntTrail( )
{
	// init our vars
	Init( );
}

void CEntTrail::Init( void )
{
	m_pEnt = NULL;
	m_pHeadPoint = NULL;
	m_pSpr = NULL;
	m_blAlive = true;
	m_iIndex = 0;
	m_iLinks = 0;
	m_flStSize = 5;
	m_flEdSize = 20;
	m_flMaxLinks = 80;
	m_flLinkLength = 30;
	m_flColor[0] = m_flColor[1] = m_flColor[2] = 1.0;
	m_flLineTime = m_flLinkLength*m_flMaxLinks/500;
	m_iRendermode = kRenderTransAdd;
	m_blDeadCheck = false;
	m_iType = TRAIL_ORIGIN;
}

CEntTrail::~CEntTrail( )
{
	// remove things
	RemoveTrail( );
}

void CEntTrail::RemoveTrail( void )
{
	// let everything know the system is dead
	m_blAlive = false;

	if( m_pHeadPoint )
	{
		TrailPoint_s *pTmp = m_pHeadPoint;

		while( pTmp )
		{
			pTmp = RemoveLink( pTmp );
		}

	}

	m_pHeadPoint = NULL;
}

void CEntTrail::AddLink( void )
{
	if( m_pEnt )
	{
		TrailPoint_s *pTrail = new TrailPoint_s;

		pTrail->flStart = gEngfuncs.GetClientTime( );
		pTrail->iFrame = 0;
		pTrail->vPoint = m_pEnt->origin;
		pTrail->vAngle = m_pEnt->angles;
		pTrail->pPrev = NULL;
		pTrail->pNext = m_pHeadPoint;

		if( m_pHeadPoint )
		{
			m_pHeadPoint->pPrev = pTrail;
		}

		m_pHeadPoint = pTrail;

		m_iLinks ++;
	}
}

void CEntTrail::RenderTrail( void )
{
	if( !m_pSpr )
	{
		if( !strstr( "sprites/", m_cSprName ) )
		{
			strcpy( m_cSprName, &m_cSprName[8] );
			int iCount = strlen( m_cSprName );

			m_cSprName[ iCount - 4 ] = '\0';		// \n sucks
		}
		m_pSpr = gTexture.LoadTexture( m_cSprName ); //g_SprTex.Find( m_cSprName )->m_pTex;
	}
	if( m_pHeadPoint )
	{
		if( m_pHeadPoint->pNext )
		{
			if( m_pHeadPoint->pNext->flDist > m_flLinkLength )
			{
				if( !m_blDeadCheck )
					AddLink( );
			}
		}
		else
		{
			if( m_iLinks < m_flMaxLinks && 
						gEngfuncs.GetClientTime() - m_pHeadPoint->flStart > 0.125 )
			{
				if( !m_blDeadCheck )
					AddLink( );
				
			}
		}
		//gEngfuncs.Con_Printf( "Links %i", m_iLinks );
	}
	else
	{
		// if we dont have any links add one if we have an entity
		if( m_pEnt )
		{
			if( !m_blDeadCheck )
				AddLink( );
		}
		else
		{
			// this system is dead
			if( !m_pHeadPoint )
			{
				m_blAlive = false;
			}
		}
	}

	UpdateTrail( );

	// store last renderd link
	TrailPoint_s *pLast = m_pHeadPoint;
	TrailPoint_s *pTmp = m_pHeadPoint;

/*	gEngfuncs.pTriAPI->*/g_CGLTriAPI.CullFace( TRI_NONE );
	// Edit by Pcjoe: Just changing the trail back to normal render mode so they look normal ^_^
	// Commented out your gl stuff for the time being
/*	gEngfuncs.pTriAPI->*/g_CGLTriAPI.RenderMode( m_iRendermode );
//	glEnable( GL_ALPHA_TEST );
//	glAlphaFunc( GL_GREATER, 0 );
/*	gEngfuncs.pTriAPI->*/g_CGLTriAPI.Texture2D( m_pSpr, 0 );
/*	gEngfuncs.pTriAPI->*/g_CGLTriAPI.Begin( TRI_QUAD_STRIP );

	while( pTmp )
	{
		pLast = pTmp;
		
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Color4f( m_flColor[0], m_flColor[1], m_flColor[2], pTmp->flAlpha );

		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.TexCoord2f( pTmp->uvCoord[0].flU, pTmp->uvCoord[0].flV );
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Vertex3fv( pTmp->vTrail[0] );
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.TexCoord2f( pTmp->uvCoord[1].flU, pTmp->uvCoord[1].flV );
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Vertex3fv( pTmp->vTrail[1] );

		pTmp = pTmp->pNext;
	}

	/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.End( );

	/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Begin( TRI_QUAD_STRIP );

	pTmp = m_pHeadPoint;

	while( pTmp )
	{
		pLast = pTmp;

		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Color4f( m_flColor[0], m_flColor[1], m_flColor[2], pTmp->flAlpha );

		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.TexCoord2f( pTmp->uvCoord[0].flU, pTmp->uvCoord[0].flV );
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Vertex3fv( pTmp->vTrail[2] );
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.TexCoord2f( pTmp->uvCoord[1].flU, pTmp->uvCoord[1].flV );
		/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.Vertex3fv( pTmp->vTrail[3] );

		pTmp = pTmp->pNext;
	}

	/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.End( );
	
/*	gEngfuncs.pTriAPI->*/g_CGLTriAPI.RenderMode( kRenderNormal );

	/*gEngfuncs.pTriAPI->*/g_CGLTriAPI.CullFace( TRI_FRONT );

	if( pLast )
	{
		if( m_iLinks > m_flMaxLinks || !pLast->flAlpha )
		{
			pLast = RemoveLink( pLast );
		}
	}
	
}

void CEntTrail::UpdateTrail( void )
{
	// do any per frame updates here
	// calculate trail distance
	float flDist = 0.0;
	vec3_t vTmp;
	TrailPoint_s *pTmp = m_pHeadPoint;

	if( pTmp )
	{
		// current point should be at the origin of the ent
		pTmp->flDist = 0.0;

		if( m_pEnt )
		{
			if( !m_blDeadCheck )
			{
				// Edited by Pcjoe
			///	if( m_iType == TRAIL_ORIGIN )
				if( m_iType == TRAIL_ORIGIN || m_iType == TRAIL_BEAM )
				{
					pTmp->vPoint = m_pEnt->origin;
					pTmp->vAngle = m_pEnt->angles;
				}
				else if( m_iType <= TRAIL_ATTACH3 && m_iType >= TRAIL_ATTACH0 )
				{
					pTmp->vPoint = m_pEnt->attachment[ m_iType - 1 ];
					pTmp->vAngle = m_pEnt->angles;
				}
				else if( m_iType == TRAIL_BONE )
				{
					if( m_Bones != NULL )
					{
						bool blOk;
						// find bone
						vec3_t vTmp = gHUD.m_BonePos.GetBoneOrigin(  m_Bones, m_cBones, blOk );

						if( blOk )
						{
							pTmp->vPoint = vTmp;
							pTmp->vAngle = m_pEnt->angles;
						}
						else
						{
							pTmp->vPoint = vec3_t( 0, 0, 0 );
							pTmp->vAngle = m_pEnt->angles;
						}
					}
					else
					{
						SetBones( m_iIndex );
					}
				}
			}
		}
	}

	while( pTmp )
	{
		if( pTmp->pNext )
		{
			vTmp = pTmp->vPoint - pTmp->pNext->vPoint;
			flDist += fabs( vTmp.Length( ) );
			pTmp->pNext->flDist = flDist;
		}

		pTmp = pTmp->pNext;
	}

	// now we have updated info
	// create the points
	pTmp = m_pHeadPoint;
	vec3_t vRight, vUp, vAng;

	while( pTmp )
	{
		if( CL_IsThirdPerson( ) )
		{
			vAng = g_CameraAngles;
		}
		else
		{
			vAng = gEngfuncs.GetLocalPlayer( )->angles;
		}

		pTmp->uvCoord[1].flU = 0;
		pTmp->uvCoord[1].flV = ( flDist - pTmp->flDist )/m_flLinkLength;
		pTmp->uvCoord[0].flU = 1;
		pTmp->uvCoord[0].flV = ( flDist - pTmp->flDist )/m_flLinkLength;

		//calc deltatime
		float flDeltaTime = gEngfuncs.GetClientTime( ) - pTmp->flStart;

		// Added by Pcjoe
		if(m_iType == TRAIL_BEAM)
		{	// Fade strait on death
			if(m_blDeadCheck)
			{	if(flDeltaTime > 0.2)
				{	pTmp->flAlpha = 0;
				}
				else
				{	pTmp->flAlpha = (0.2-flDeltaTime) / 0.2;
				}
			}
			// Dont fade when being fired
			else
			{	pTmp->flAlpha = 1;
			}
		}
		// end of add by Pcjoe
		else
		{
			pTmp->flAlpha = ( ( ( m_flMaxLinks * m_flLinkLength - pTmp->flDist ) 
						/ ( m_flMaxLinks * m_flLinkLength ) 
						+ ( m_flLineTime - flDeltaTime ) / ( m_flLineTime ) ) )/ 2.0f;
		}

		if( pTmp->flAlpha < 0.0 )
			pTmp->flAlpha = 0.0;
		if( pTmp->flAlpha > 1.0 )
			pTmp->flAlpha = 1.0;

		float flDistSize = m_flEdSize - m_flStSize;
		pTmp->flSize = m_flStSize + ( ( 1 - pTmp->flAlpha ) * flDistSize );

		gEngfuncs.pfnAngleVectors( vAng, NULL, vRight, vUp );

		pTmp->vTrail[0] = pTmp->vPoint + ( vRight ) * pTmp->flSize;
		pTmp->vTrail[1] = pTmp->vPoint - ( vRight ) * pTmp->flSize;

		pTmp->vTrail[2] = pTmp->vPoint + ( vUp ) * pTmp->flSize;
		pTmp->vTrail[3] = pTmp->vPoint - ( vUp ) * pTmp->flSize;

		pTmp = pTmp->pNext;
	}
}

TrailPoint_s *CEntTrail::RemoveLink( TrailPoint_s *pPoint )
{
	if( pPoint )
	{
		TrailPoint_s *pTmp = pPoint->pNext;

		if( pPoint == m_pHeadPoint )
		{
			m_pHeadPoint = pPoint->pNext;
			pTmp = m_pHeadPoint;

			if( m_pHeadPoint )
			{
				m_pHeadPoint->pPrev = NULL;
			}
		}
		else
		{
			pTmp = pPoint->pNext;
		}

		// keep total linkage
		if( pPoint->pNext )
		{
			pPoint->pNext->pPrev = pPoint->pPrev;
		}
		if( pPoint->pPrev )
		{
			pPoint->pPrev->pNext = pPoint->pNext;
		}

		delete pPoint;

		m_iLinks --;

		return pTmp;
	}

	return NULL;
}

void CEntTrail::RemoveWhenDone( void )
{
	m_blDeadCheck = true;
	m_iIndex = -1;

	// Pcjoe: reset life of beams so it's progressivly lower
	if(m_iType == TRAIL_BEAM)
	{
		int iNum=0;
		TrailPoint_s *pTmp = m_pHeadPoint;
		
		while( pTmp )
		{	pTmp->flStart = gEngfuncs.GetClientTime( ) + (m_iLinks-iNum)*0.05;
			iNum++;
			pTmp = pTmp->pNext;
		}
	}
}

void CEntTrail::SetBones( int iIndex )
{
	m_Bones = gHUD.m_BonePos.AddBoneRecordNormal( iIndex );
}
