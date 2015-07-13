#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"gamerules.h"
#include	"player.h"

using namespace std;

// Constructor/Deconstructor
CSaberStyle::CSaberStyle( )
{	
#ifndef CLIENT_DLL
	SetSaber(NULL);
	m_QueuedMoves.clear();
#endif
}

CSaberStyle::~CSaberStyle( )
{	
#ifndef CLIENT_DLL
	SetSaber(NULL);
	m_QueuedMoves.clear();
#endif
}

#ifndef CLIENT_DLL
/*
* Update saber
*/
BOOL CSaberStyle::UpdateSaber( void )
{	if(m_QueuedMoves.empty())
	{	return FALSE;
	}
	return TRUE;
}

/*
* Get next set of saber data
*/
saber_move_s CSaberStyle::GetNextSaberData( void )
{	// Get a copy of the saber move data
	saber_move_s move = (*m_QueuedMoves.begin());
	// Remove queue from list
	m_QueuedMoves.pop_front();
	// Add data to main saber move list
	return move;
}

/*
* Updata data from script
*/
void CSaberStyle::UpdateDataFromScript( void )
{	// Grab styles data
	vector<MechStyle_s>::iterator iterStyle = GetStyle();
	// Styles data not found
	if(iterStyle == NULL)
	{	ALERT(at_console,"Styles cant be found!\n");
		return;
	}

	// Search through combos for a matching move
	BOOL fCombo = FALSE;
	vector<MechCombo_s>::iterator iterCmb;
	ForEachIn(iterCmb,iterStyle->Combos)
	{	// Do basic swings last
		if(iterCmb->szCombo != "basicswing")
		{	
			if(m_pSaber->CanUseMove(iterCmb->szMoves))
			{	fCombo = TRUE;
				saber_move_s NewMove;
				// Set stall varibles
				if(GetVaribleBOOL("stall", iterCmb->szVaribles))
				{	NewMove.fStall = TRUE;
					NewMove.flEstEndTime = GetVaribleFloat("stalltime", iterCmb->szVaribles);
				}
				else
				{	NewMove.fStall = FALSE;
					NewMove.flEstEndTime = 0;
				}

				// Set damage
				NewMove.flDmg = GetVaribleFloat("damage", iterCmb->szVaribles);

				// Set names
				NewMove.strCmbName = iterCmb->szCombo;
				NewMove.strMoveName = iterCmb->szMoves[iterCmb->szMoves.size()-1];

				// Set animation
				NewMove.strAnim = GetVaribleString("anim", iterCmb->szVaribles);

				// Reset various varibles
				NewMove.flStartTime = 0;
				NewMove.fInit = FALSE;
				NewMove.fComplete = FALSE;
				NewMove.prevhitents.clear();

				AddSaberMove(NewMove);
				break;
			}
		}
	}

	if(!fCombo)
	{	ForEachIn(iterCmb,iterStyle->Combos)
		{	// Do basic swings
			if(iterCmb->szCombo == "basicswing")
			{	// Make sure we're pressing the correct direction
				if(iterCmb->szMoves[0] == m_pSaber->GetSwingDirectionString())
				{	// Make sure we're not swinging in the same direction while a stall
					if(m_pSaber->GetCurSwingDirection() == iterCmb->szMoves[0])
					{	if(m_pSaber->GetCurrentSwing() != NULL && m_pSaber->GetCurrentSwing()->fStall)
						{	continue;
						}
					}
					saber_move_s NewMove;
					// reset stall
					NewMove.fStall = FALSE;
					NewMove.fStallFreezeLastFrame = FALSE;
					NewMove.flEstEndTime = 0;

					// Set damage
					NewMove.flDmg = GetVaribleFloat("damage", iterCmb->szVaribles);

					// Set names
					NewMove.strCmbName = iterCmb->szCombo;
					NewMove.strMoveName = iterCmb->szMoves[0];

					// Set animation
					NewMove.strAnim = GetVaribleString("anim", iterCmb->szVaribles);

					// Reset various varibles
					NewMove.flStartTime = 0;
					NewMove.fInit = FALSE;
					NewMove.fComplete = FALSE;
					NewMove.prevhitents.clear();

					AddSaberMove(NewMove);

					// If this is a stall, add an additional move
					if(GetVaribleBOOL("stall", iterCmb->szVaribles))
					{	saber_move_s NewStallMove = NewMove;
						NewStallMove.fStall = TRUE;
						NewStallMove.flEstEndTime = GetVaribleFloat("stalltime", iterCmb->szVaribles);
						string strStallAnim = GetVaribleString("stall_anim", iterCmb->szVaribles);
						if(strStallAnim != "")
						{	NewStallMove.strAnim = strStallAnim;
						}
						else
						{	NewStallMove.fStallFreezeLastFrame = TRUE;
						}
						AddSaberMove(NewStallMove);
					}
					break;
				}
			}
		}
	}
}	

/*
* Get Current Style from parse file
*/
std::vector<MechStyle_s>::iterator CSaberStyle::GetStyle( void )
{	std::vector<MechStyle_s>::iterator iterStyle;
	ForEachIn(iterStyle, g_MMParse.m_Styles)
	{	if(iterStyle->szStyle == m_pSaber->GetCurrentAttribute())
		{	return iterStyle;
		}
	}
	return NULL;
}

/*
* Get Global Data
*/
std::string CSaberStyle::GetGlobalString( char *pDataName )
{	if(GetStyle() == NULL)
	{	return string("");
	}
	return GetVaribleString( pDataName, GetStyle()->szGlobalVaribles );
}
BOOL CSaberStyle::GetGlobalBOOL( char *pDataName )
{	if(GetStyle() == NULL)
	{	return FALSE;
	}
	return GetVaribleBOOL( pDataName, GetStyle()->szGlobalVaribles );
}
float CSaberStyle::GetGlobalFloat( char *pDataName )
{	if(GetStyle() == NULL)
	{	return 0;
	}
	return GetVaribleFloat( pDataName, GetStyle()->szGlobalVaribles );
}
int CSaberStyle::GetGlobalInt( char *pDataName )
{	if(GetStyle() == NULL)
	{	return 0;
	}
	return GetVaribleInt( pDataName, GetStyle()->szGlobalVaribles );
}

/*
* Get Local Varible
*/
std::string CSaberStyle::GetVaribleString( char *pSearch, std::map<std::string,std::string> data )
{	std::map<std::string,std::string>::iterator find = data.find(pSearch);
	if(find != data.end())
	{	return find->second;
	}
	return std::string("");
}

BOOL CSaberStyle::GetVaribleBOOL( char *pSearch, std::map<std::string,std::string> data )
{	std::string strData = GetVaribleString( pSearch, data );
	if(strData == "")
	{	return FALSE;
	}
	if(strData == "true")
	{	return TRUE;
	}
	return FALSE;
}

float CSaberStyle::GetVaribleFloat( char *pSearch, std::map<std::string,std::string> data )
{	std::string strData = GetVaribleString( pSearch, data );
	if(strData == "")
	{	return 0;
	}

	float flData=0;
	int i = sscanf(strData.c_str(), "%f", &flData);
	return flData;
}

int CSaberStyle::GetVaribleInt( char *pSearch, std::map<std::string,std::string> data )
{	std::string strData = GetVaribleString( pSearch, data );
	if(strData == "")
	{	return 0;
	}

	int iData=0;
	int i = sscanf(strData.c_str(), "%i", &iData);
	return iData;
}
#endif
