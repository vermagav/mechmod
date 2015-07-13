/******************************************************************
*					DECAL MSGS
******************************************************************
* Created By: Jason "ssjason123" Matson
* Created On: 2-8-04
* Last Edited: 2-8-04
* Purpose: Easy decal removal system
******************************************************************/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "r_efx.h"

DECLARE_MESSAGE( m_DecalMsg, DecalMsg );

int CHudDecalMsg::Init( void )
{
	HOOK_MESSAGE( DecalMsg );

	return 1;
}

int CHudDecalMsg::VidInit( void )
{
	return 1;
}

int CHudDecalMsg::MsgFunc_DecalMsg( const char *pszName, int iSize, void *pbuf )
{
	int iStatus;
	BEGIN_READ( pbuf, iSize );
		iStatus = READ_BYTE( );

	if( iStatus )
	{
		RemoveDecals( );
	}
	return 1;
}

void CHudDecalMsg::RemoveDecals( void )
{
	for( int i = 0; i< CVAR_GET_FLOAT( "r_decals" ); i++ )
	{
		gEngfuncs.pEfxAPI->R_DecalRemoveAll( i );
	}
}