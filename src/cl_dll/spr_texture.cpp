/**********************************************************
*			SPRITE TEXTURE MANAGMENT SYSTEM
***********************************************************
*	Purpose: Manager textures for particle system and 
*			anything else that uses spr files down the road
*	Created On: 1/4/04
*	Last Edited: 1/4/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include "hud.h"
#include "cl_util.h"
#include "com_model.h"
#include "spr_texture.h"

CSprTex g_SprTex;

//constructor
CSprTex::CSprTex( void )
{
	//initalize our vars
	m_pHeadTex = NULL;
}

//destructor 
CSprTex::~CSprTex( void )
{
	//remove all used memory
	RemoveAll( );
}

void CSprTex::RemoveAll( void )
{
	SprTex_s *pTmp = m_pHeadTex;
	while( pTmp )
	{
		pTmp = Remove( pTmp );
	}

	m_pHeadTex = pTmp;
}

SprTex_s *CSprTex::Remove( SprTex_s *pSpr )
{
	//remove the sprtex and keep things intact
	SprTex_s *pTex = NULL;
	
	if( pSpr->m_pNext )
	{
		pSpr->m_pNext->m_pPrev = pSpr->m_pPrev;
	}
	if( pSpr->m_pPrev )
	{
		pSpr->m_pPrev->m_pNext = pSpr->m_pNext;
	}

	//if we are the head texture do fancy stuff
	if( pSpr->m_pNext )
	{
		//we are removeing the head link take care
		//m_pHeadTex = pSpr->m_pNext;
		pTex = pSpr->m_pNext;
		m_pHeadTex = pTex;
	}

	delete pSpr;

	return pTex;
}

SprTex_s *CSprTex::Find( char *pName )
{
	SprTex_s *pTex = NULL;

	//find the texture
	pTex = LookUp( pName );
	//if texture still dosent exist create one
	if( !pTex )
	{
		pTex = Create( pName );
	}

	//if se still dont exist alert and use another sprite
	if( !pTex )
	{
		if( m_pHeadTex )
		{
			gEngfuncs.Con_Printf( "Texture %s Not found! Using %s\n", pName, m_pHeadTex->m_pTex->name );
			pTex = m_pHeadTex;
		}
		//we havent loaded any texture yet so load flare3
		else
		{
			pTex = Create( "sprites/flare3.spr" );
			gEngfuncs.Con_Printf( "Texture %s Not found! Using %s\n", pName, pTex->m_pTex->name );
		}
	}

	return pTex;
}

SprTex_s *CSprTex::LookUp( char *pName )
{
	SprTex_s *pTex = m_pHeadTex;

	while( pTex )
	{
		//look for maching texture names
		if( !strcmp( pName, pTex->m_pTex->name ) )
		{
			return pTex;
		}
		//move to next node
		pTex = pTex->m_pNext;
	}

	return NULL;
}

SprTex_s *CSprTex::Create( char *pName )
{
	SprTex_s *pTex = NULL;
	model_s *pModel = (struct model_s *)gEngfuncs.GetSpritePointer( gEngfuncs.pfnSPR_Load( pName ) );

	//create a new link only if we have a model
	if( pModel )
	{
		pTex = new SprTex_s;

		pTex->m_pNext = m_pHeadTex;
		pTex->m_pPrev = NULL;
		if( m_pHeadTex )
		{
			m_pHeadTex->m_pPrev = pTex;
		}
		pTex->m_pTex = pModel;

		m_pHeadTex = pTex;
	}

	return pTex;
}
