/**********************************************************
*					GL Shadow effect
***********************************************************
*	Purpose: opengl shadow effect test
*	Created On: 10/19/04
*	Last Edited: 10/19/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"
#include	"texture.h"
#include	"gl-triapi.h"
#include	"pm_defs.h"
#include	"pmtrace.h"
#include	"gl_bored.h"

#include	<list>
#include	<windows.h>
#include	<gl/gl.h>


CShadow g_Shadow;

CShadow::CShadow( void )
{
	m_pTex = NULL;
}

void CShadow::LoadTex( void )
{
	m_pTex = gTexture.LoadTexture( "playershadow" );
}

void CShadow::RenderShadow( void )//vec3_t vOrg )
{

	if( !gTexture.IsOpenGL( ) )
	{
		return;
	}

	if( !gEngfuncs.pfnGetCvarFloat( "gl_shadow" ) )
	{
		return;
	}

	std::list<vec3_t>::iterator iterVector = m_vOrgs.begin( );
	while( iterVector != m_vOrgs.end( ) )
	{
		vec3_t vOrg = *iterVector;
		iterVector++;

		float flEnd = vOrg.z;
		// only render the shadow on depth fail
		glDepthFunc( GL_GEQUAL );
		//glEnable( GL_BLEND );
		glEnable( GL_TEXTURE_2D );
		glDepthMask( GL_FALSE );

		if( !m_pTex )
		{
			m_pTex = gTexture.LoadTexture( "playershadow" );
		}

		if( !m_pTex )
		{
			return;
		}


		g_CGLTriAPI.Texture2D( m_pTex, 0 );
		g_CGLTriAPI.RenderMode( kRenderTransColor );

		glDisable( GL_CULL_FACE );

		glBegin( GL_QUADS );
			glTexCoord2f( 1, 1 );

			vOrg = *iterVector;
			iterVector++;
			if( flEnd < vOrg.z )
				vOrg.z = flEnd;
			glVertex3fv( vOrg - vec3_t( 0, 0, 1 ) );
			
			vOrg = *iterVector;
			iterVector++;
			if( flEnd < vOrg.z )
				vOrg.z = flEnd;
			glTexCoord2f( 1, 0 );
			glVertex3fv( vOrg - vec3_t( 0, 0, 1 ) );

			vOrg = *iterVector;
			iterVector++;
			if( flEnd < vOrg.z )
				vOrg.z = flEnd;
			glTexCoord2f( 0, 0 );
			glVertex3fv( vOrg - vec3_t( 0, 0, 1 ) );

			vOrg = *iterVector;
			iterVector++;
			if( flEnd < vOrg.z )
				vOrg.z = flEnd;
			glTexCoord2f( 0, 1 );
			glVertex3fv( vOrg - vec3_t( 0, 0, 1 ) );
		glEnd( );

		glBlendFunc( GL_ONE, GL_ZERO );
		glDisable( GL_BLEND );

		glEnable( GL_CULL_FACE );
		glDepthFunc( GL_LEQUAL );
		glDepthMask( GL_TRUE );
	}

	m_vOrgs.clear( );
}

void CShadow::SetShadowOrg( vec3_t vOrg )
{
	cl_entity_t *pEnt = gEngfuncs.GetLocalPlayer( );
	

	vec3_t vShadowOrg = gEngfuncs.PM_TraceLine( vOrg, vOrg - vec3_t( 0, 0, 4096 ), PM_TRACELINE_PHYSENTSONLY, 2, -1 )->endpos ;

	pmtrace_t *pTrace = gEngfuncs.PM_TraceLine( pEnt->origin, vShadowOrg, PM_TRACELINE_PHYSENTSONLY, 2, -1 );
	
	if( pTrace->fraction != 1.0 )
	{
		return;
	}

	// cant use traceline in studio render casue it casuses problems
	m_vOrgs.push_back(  gEngfuncs.PM_TraceLine( vOrg, vOrg - vec3_t( 0, 0, 4096 ), PM_TRACELINE_PHYSENTSONLY, 2, -1 )->endpos );
	m_vOrgs.push_back(  gEngfuncs.PM_TraceLine( vOrg + vec3_t( 10, 10, 0 ), vOrg + vec3_t( 10, 10, -4096 ), PM_TRACELINE_PHYSENTSONLY, 2, -1 )->endpos );
	m_vOrgs.push_back(  gEngfuncs.PM_TraceLine( vOrg + vec3_t( 10, -10, 0 ), vOrg + vec3_t( 10, -10, -4096 ), PM_TRACELINE_PHYSENTSONLY, 2, -1 )->endpos );
	m_vOrgs.push_back(  gEngfuncs.PM_TraceLine( vOrg + vec3_t( -10, -10, 0 ), vOrg + vec3_t( -10, -10, -4096 ), PM_TRACELINE_PHYSENTSONLY, 2, -1 )->endpos );
	m_vOrgs.push_back(  gEngfuncs.PM_TraceLine( vOrg + vec3_t( -10, 10, 0 ), vOrg + vec3_t( -10, 10, -4096 ), PM_TRACELINE_PHYSENTSONLY, 2, -1 )->endpos );
}
