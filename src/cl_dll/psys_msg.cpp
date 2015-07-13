/**********************************************************
*			PARTICLE SYSTEM MESSAGER
***********************************************************
*	Purpose: Recives Msgs from the client and parses them
*			to create our kill our effects
*	Created On: 1/6/04
*	Last Edited: 1/6/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "triangleapi.h"
#include "com_model.h"
//#include "spr_texture.h"
#include "texture.h"
#include "psys_pool.h"
#include "psys_part.h"
#include "psys_man.h"
#include "eff_map.h"
#include "eff_smoke.h"
#include "eff_def.h"
#include "eff_explosion.h"
#include "eff_nuke.h"


DECLARE_MESSAGE( m_PartMsg, PartMsg );

int CHudPartMsg::Init( void )
{	// get only msgs that are PartMsg
	HOOK_MESSAGE( PartMsg );
	m_iFlags = 0;
	//gHUD.AddHudElem(this);
	return 1;
}

int CHudPartMsg::VidInit( void )
{
	// nothing to do here
	return 1;
}

int CHudPartMsg::MsgFunc_PartMsg( const char *pszName, int iSize, void *pbuf )
{
	// here is where we read in the new data for the msg

	// TODO: SET POINT AND BRUSH SYSTEMS, AND REMOVE
	int iIndex, iSys;

	BEGIN_READ( pbuf, iSize );

	iIndex	= READ_SHORT( );
	iSys	= READ_SHORT( );

	if( iSys == EFF_REMOVE )
	{
		// that is all we need to remove a system
		int iStatus;

		iStatus		= READ_BYTE( );	// tells us if we jsut wanna turn system off or remove it

		CParticleSystem *pSys = GetManager()->Find( iSys, iIndex );
		GetManager()->Remove( pSys );
	}
	else if( iSys == EFF_BRUSH  || iSys == EFF_POINT )
	{
		int iPPS, iBounces, iFlags, iAngLock;
		float flSysLife, flDelay, flPartDelay, flStSize, flEdSize, flBncScl, flGrav, flFR, flLife;
		colorRGBA clEdCol, clStCol;
		vec3_t vAng, vPMaxVel, vPMinVel, vMaxVel, vMinVel;
		//char *pTex;
		std::string pTex;
		//now we need to fill everything
		iPPS		= READ_SHORT( );
		iBounces	= READ_SHORT( );
		iFlags		= READ_SHORT( );
		iAngLock	= READ_SHORT( );

		clEdCol.r	= float( READ_BYTE( ) )/255.0f;
		clEdCol.g	= float( READ_BYTE( ) )/255.0f;
		clEdCol.b	= float( READ_BYTE( ) )/255.0f;
		clEdCol.a	= float( READ_BYTE( ) )/255.0f;

		clStCol.r	= float( READ_BYTE( ) )/255.0f;
		clStCol.g	= float( READ_BYTE( ) )/255.0f;
		clStCol.b	= float( READ_BYTE( ) )/255.0f;
		clStCol.a	= float( READ_BYTE( ) )/255.0f;

		flSysLife	= READ_COORD( );
		flDelay		= READ_COORD( );
		flPartDelay	= READ_COORD( );
		flStSize	= READ_COORD( );
		flEdSize	= READ_COORD( );
		flBncScl	= READ_COORD( );
		flGrav		= READ_COORD( );
		flFR		= 1.0f/READ_COORD( );
		flLife		= READ_COORD( );

		vAng.x		= READ_COORD( );
		vAng.y		= READ_COORD( );
		vAng.z		= READ_COORD( );

		vPMaxVel.x	= READ_COORD( );
		vPMaxVel.y	= READ_COORD( );
		vPMaxVel.z	= READ_COORD( );

		vPMinVel.x	= READ_COORD( );
		vPMinVel.y	= READ_COORD( );
		vPMinVel.z	= READ_COORD( );

		vMaxVel.x	= READ_COORD( );
		vMaxVel.y	= READ_COORD( );
		vMaxVel.z	= READ_COORD( );

		vMinVel.x	= READ_COORD( );
		vMinVel.y	= READ_COORD( );
		vMinVel.z	= READ_COORD( );

		pTex		= READ_STRING( );

		// need to strip sprites/ .spr off the file name
		pTex.erase( 0, 8 );
		pTex.erase( pTex.size( ) - 4, pTex.size( ) );

		//gEngfuncs.Con_Printf( "%s\n", pTex.c_str( ) );

		if( iSys == EFF_BRUSH )
		{
			// create a brush system
			GetManager()->AddSystem( new CEffBrush( vec3_t( 0,0,0 ), vMaxVel, vMinVel, vPMaxVel, vPMinVel,
													vAng, clEdCol, clStCol, flBncScl, flGrav,
													flLife, flFR, flStSize, flEdSize, flSysLife, flDelay,
													flPartDelay, iBounces, iFlags, iIndex, iPPS, iAngLock, (char *)pTex.c_str( ) ) );
		}
		else if( iSys == EFF_POINT )
		{
			// create a point sys
			vec3_t vOrg = gEngfuncs.GetEntityByIndex( iIndex )->curstate.origin;
			GetManager()->AddSystem( new CEffPoint( vOrg, vMaxVel, vMinVel, vPMaxVel, vPMinVel,
													vAng, clEdCol, clStCol, flBncScl, flGrav,
													flLife, flFR, flStSize, flEdSize, flSysLife, flDelay,
													flPartDelay, iBounces, iFlags, iIndex, iPPS, iAngLock, (char *)pTex.c_str( ) ) );
		}
	}
	else if( iSys == EFF_SMOKE )
	{
		int iAlive;
		iAlive = READ_BYTE( );

		if( iAlive )
		{
			GetManager()->AddSystem( new CEffSmoke( iIndex ) );
		}
		else
		{
			CParticleSystem *pSys;
			pSys = GetManager()->Find( iSys, iIndex );

			if( pSys )
			{
				pSys->m_blAlive = false;
			}

		}
	}
	// added by jason
	else if( iSys == EFF_WASTEDEXP || iSys == EFF_ALPHABOMB )
	{

		vec3_t vOrg;
		float flSize;

		vOrg.x	= READ_COORD( );
		vOrg.y	= READ_COORD( );
		vOrg.z	= READ_COORD( );

		flSize	= READ_COORD( );

		GetManager()->AddSystem( new CEffWastedExp( vOrg, flSize, iSys ) );
	}
	else if( iSys == EFF_NUKE )
	{
		vec3_t vOrg;
		vOrg.x = READ_COORD( );
		vOrg.y = READ_COORD( );
		vOrg.z = READ_COORD( );

		GetManager( )->AddSystem( new CExploNuke( vOrg ) );
	}
	else
	{
		gEngfuncs.Con_Printf( "No Such System.\n" );
	}

	return 1;
}

