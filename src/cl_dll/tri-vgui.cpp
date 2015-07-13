/**********************************************************
*			TRI API VGUI SCREENS FOR MORE FLEXABILITY
***********************************************************
*	Purpose: Displays triapi stuff behind vgui
*	Created On: 9/13/04
*	Last Edited: 9/15/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#include	"hud.h"
#include	"cl_util.h"
#include	"studio_util.h"
#include	"texture.h"
#include "r_studioint.h"

#include	"gl-triapi.h"
#include	"tri-vgui.h"

#include	<windows.h>
#include	<gl/gl.h>

vec3_t		g_vViewOrg;
vec3_t		g_vecCamAngles;
vec3_t		g_vPlayerAngles;


extern engine_studio_api_t IEngineStudio;
#define DLLEXPORT __declspec( dllexport )
extern "C"
{
	int DLLEXPORT CL_IsThirdPerson( void );
};

CClassMenu g_ClassMenu;
CTeamMenu g_TeamMenu;

CTriMenu::CTriMenu( void )
{
	m_pTex = NULL;
	m_vPos[0] = m_vPos[1] = m_vPos[2] = m_vPos[3] = vec2_t( 0, 0 );
	m_strTex.erase( );
}

CTriMenu::~CTriMenu( void )
{
}

void CTriMenu::UpdateTexture( const char *pTex )
{
	
	if( pTex )
	{
		// no need to delete texture if updating to a new one
		// will get removed on map change
	//	gEngfuncs.Con_Printf( "%s\n", pTex );
		m_pTex = gTexture.LoadTexture( (char *)pTex );

		m_strTex = pTex;
	}
}

void CTriMenu::UpdatePosition( vec2_t vStart, float flHeight, float flWidth )
{
	m_vPos[0] = vStart;
	m_vPos[1] = vStart + vec2_t( 0, flHeight );
	m_vPos[2] = vStart + vec2_t( flWidth, flHeight );
	m_vPos[3] = vStart + vec2_t( flWidth, 0 );
}

void CTriMenu::Draw( void )
{
	if( m_pTex )
	{
		g_CGLTriAPI.Color4f( 1, 1, 1, 1 );
		g_CGLTriAPI.RenderMode( kRenderNormal );
		g_CGLTriAPI.CullFace( TRI_NONE );
		g_CGLTriAPI.Texture2D( m_pTex, 0 );
	//	gEngfuncs.Con_Printf( "%i, Texture\n", m_pTex->iTexNum );

		g_CGLTriAPI.Begin( TRI_QUADS );
			// need to find ortho coords for this
			g_CGLTriAPI.TexCoord2f( 0, 0 );
			g_CGLTriAPI.Vertex3fv( MakePointOrtho( m_vPos[1] ) );
			g_CGLTriAPI.TexCoord2f( 0, 1 );
			// gEngfuncs.Con_Printf( "%f\n", m_vPos[0].x );
			g_CGLTriAPI.Vertex3fv( MakePointOrtho( m_vPos[0] ) );//m_vPos[0].x, m_vPos[0].y, 5 );
			g_CGLTriAPI.TexCoord2f( 1, 1 );
			g_CGLTriAPI.Vertex3fv( MakePointOrtho( m_vPos[3] ) );
			g_CGLTriAPI.TexCoord2f( 1, 0 );
			g_CGLTriAPI.Vertex3fv( MakePointOrtho( m_vPos[2] ) );
			
		g_CGLTriAPI.End( );
	}
	else
	{
		UpdateTexture( m_strTex.c_str( ) );
	}

}

vec3_t CTriMenu::MakePointOrtho( vec2_t vOrg )
{
	vec3_t vForward, vRight, vUp, vTotalOrg, vFRight, vFUp;

	if(CL_IsThirdPerson())
	{
		extern vec3_t g_vecCamAngles;
		AngleVectors( g_vecCamAngles, vForward, vRight, vUp );
	}
	// Use first person angles
	else
	{
		AngleVectors( g_vPlayerAngles , vForward, vRight, vUp );
	}

	vTotalOrg = g_vViewOrg + vForward*5;
	
	float flTmp[3] = {1, 1, 0};
	flTmp[0] = ( 2* vOrg.x ) - 1;
	flTmp[1] = ( 2 *vOrg.y) - 1;

	/*vFRight = */gEngfuncs.pTriAPI->ScreenToWorld( flTmp, vFRight ); 
	// software mode fallback casue screentoworld doesnt work there
	if( !IEngineStudio.IsHardware( ) )
	{
		vFRight = vRight+(vRight*( 6.5* vOrg.x - 3.75 )/.5 );
		vFUp = vUp+(vUp*( 4.9*vOrg.y - 2.95 )/.5);

		vFRight = vFRight + vTotalOrg + vFUp + ( vForward * 1.5 );
	}

	// add forward so it doesnt clip the view
	return vFRight;//vTotalOrg + vFRight + vFUp + ( vForward * 1.5 );
}

void CClassMenu::Render( void )
{
	m_TriMenu.Draw( );
	m_TriMenu1.Draw( );
	m_TriMenu2.Draw( );
	m_TriMenu3.Draw( );
}

void CClassMenu::Initialize( void )
{
	Reset( );
	m_TriMenu.UpdateTexture( "class3" );
	m_TriMenu.UpdatePosition( vec2_t( .5f, 0 ), .5f, .5f );
	m_TriMenu1.UpdateTexture( "class4" );
	m_TriMenu1.UpdatePosition( vec2_t( 0, 0 ), .5f, .5f );
	m_TriMenu2.UpdateTexture( "class1" );
	m_TriMenu2.UpdatePosition( vec2_t( 0, .5f ), .5f, .5f );
	m_TriMenu3.UpdateTexture( "class2" );
	m_TriMenu3.UpdatePosition( vec2_t( .5f, .5f ), .5f, .5f );
}

void CClassMenu::Reset( void )
{
	m_TriMenu.m_pTex = NULL;
	m_TriMenu1.m_pTex = NULL;
	m_TriMenu2.m_pTex = NULL;
	m_TriMenu3.m_pTex = NULL;
}

void CTeamMenu::Initialize( void )
{
	Reset( );
	m_TriMenu.UpdateTexture( "team3" );
	m_TriMenu.UpdatePosition( vec2_t( .5f, 0 ), .5f, .5f );
	m_TriMenu1.UpdateTexture( "team4" );
	m_TriMenu1.UpdatePosition( vec2_t( 0, 0 ), .5f, .5f );
	m_TriMenu2.UpdateTexture( "team1" );
	m_TriMenu2.UpdatePosition( vec2_t( 0, .5f ), .5f, .5f );
	m_TriMenu3.UpdateTexture( "team2" );
	m_TriMenu3.UpdatePosition( vec2_t( .5f, .5f ), .5f, .5f );
}

void CTeamMenu::Reset( void )
{
	m_TriMenu.m_pTex = NULL;
	m_TriMenu1.m_pTex = NULL;
	m_TriMenu2.m_pTex = NULL;
	m_TriMenu3.m_pTex = NULL;
}

void CTeamMenu::Render( void )
{
	m_TriMenu.Draw( );
	m_TriMenu1.Draw( );
	m_TriMenu2.Draw( );
	m_TriMenu3.Draw( );
}
