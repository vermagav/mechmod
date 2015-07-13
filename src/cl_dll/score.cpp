
/*/
 --------------------------------
 SCORE MODULE
 ~~~~~~~~~~~~
 Version: 24th January 2004
 Author: Gav "grOOvy" Verma
 --------------------------------
/*/

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>


DECLARE_MESSAGE ( m_Score, Score );
 
int CHudScore::Init ( void )
{
	HOOK_MESSAGE ( Score );
	m_iScore = 0;
	m_iFlags |= HUD_ACTIVE;
	gHUD.AddHudElem ( this );
	return 1;
}

int CHudScore::VidInit ( void )
{	
	return 1;
}


int CHudScore::Draw ( float flTime )
{
	float r, g, b;
	r = 112.0/255.0;
	g = 220.0/255.0;
	b = 17.0/255.0;
	// UnpackRGB(r,g,b, RGB_YELLOWISH); // retarded bish colors
 
	gEngfuncs.pfnDrawSetTextColor(r,g,b);

	int x = ScreenWidth - 100;
	int y = ScreenHeight - 20;
  
	char szScore[64];
	sprintf ( szScore, "Score: %i\n", m_iScore );
	
	// Draw
	DrawConsoleString( x, y, szScore ); 
	return 1;
}

int CHudScore::MsgFunc_Score(const char *pszName,  int iSize, void *pbuf )
{	BEGIN_READ( pbuf, iSize );
	m_iScore = READ_LONG();
	return 1;
}
