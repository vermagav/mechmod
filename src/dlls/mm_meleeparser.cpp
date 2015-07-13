#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"monsters.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"gamerules.h"
#include	"player.h"
#include	"animation.h"
#include	"mmweapons.h"
#include	<fstream>

CMech_Parse g_MMParse;

using namespace std;


CMech_Parse::~CMech_Parse( void )
{
	m_Scripts.clear( );
	m_Styles.clear();
}

CMech_Parse::CMech_Parse( void )
{
	m_Scripts.clear( );	// make sure its empty
	m_Styles.clear();
}


bool CMech_Parse::ParseScript( std::string File )
{
	char szDir[32];
	GET_GAME_DIR( szDir );
	std::string FilePath = szDir;
	FilePath += '/';
	FilePath += File;

	bool blOk = false;

	// check if script is loaded
	if( !m_Scripts.empty( ) )
	{
		list<MechScript_s>::iterator Script = m_Scripts.begin( );

		while( Script != m_Scripts.end( ) )
		{
			if(  Script->szName == File )
			{
			//	PrintScript( Script );

				ALERT( at_console, "already Loaded\n" );
				return true;
			}

			Script++;
		}
	}

	
	MechScript_s newScript;
	newScript.szName = File;
	
	blOk = ReadScript( (char * )FilePath.c_str(), &newScript );

	m_Scripts.push_front( newScript );
	

	if( blOk == true )
	{
		SortScript( &newScript );

	//	PrintScript( m_Scripts.begin( ) );

		//ALERT( at_console, "%i\n", m_Scripts.size( ) );
	}
	else
	{
		// remove script
		ALERT( at_console, "no file at: %s\n", FilePath );
		m_Scripts.pop_front( );
	}


	return blOk;
}


void CMech_Parse::RemoveComments( std::string *pLine )
{

	if( pLine )
	{
		const char	*pData = pLine->c_str();
		std::string	szData;
		int		i = 0;
		bool	blStillData = true;

		while( blStillData )		// while usefull data is found
		{
			if( *pData == '\0' )		// check end of string
			{
				blStillData = false;
				//szData += pData[0];
			}
			else if( !memcmp( pData,"//", sizeof( char ) * 2 ) )
			{
				pData++;
				i++;
				blStillData = false;
			}
			else
			{
				szData += pData[0];
			}

			pData++;
			i++;					// move to next char
		}

		//strcpy( (char *)pLine, szData );	// store new string in old location
		*pLine = szData;
	}

}

bool CMech_Parse::ReadScript( char *pFile, MechScript_s *pScript )
{
	if( pFile )
	{
		ifstream In( pFile );

		if( In )					// lets us know file exists
		{
			// read through file to find # of lines for mem allocation
			int iSize = 0, i = 0;
			std::string szTemp;

			i = 0;
			std::string TmpStr;

			while( getline( In, TmpStr, '\n' ) )
			{
				
				pScript->ScriptData.push_back( TmpStr );
				MakeLower( pScript->ScriptData[i].c_str( ) );
				RemoveLeadingSpaces( &pScript->ScriptData[i] );
				RemoveComments( &pScript->ScriptData[i] );
				
				i++;
			}

			RemoveNullLines( pScript );

			In.close( );

			return true;
		}

		return false;
	}

	return false;
}

void CMech_Parse::PrintScript( list<MechScript_s>::iterator Script )
{

	/*vector<std::string>::iterator String = Script->ScriptData.begin();

	while( String != Script->ScriptData.end() )
	{
		ALERT( at_console, "%s\n", String->c_str() );
		String++;
	}*/

	// Print Style, Combo, Commands
	vector<MechStyle_s>::iterator Style = m_Styles.begin();

	while( Style != m_Styles.end() )
	{
		// Print style name
		ALERT( at_console, "%s\n", Style->szStyle.c_str() );

		// Print Global Varibles
		ALERT( at_console, "global varibles:\n");
		std::map<std::string,std::string>::iterator iterVars;
		ForEachIn( iterVars, Style->szGlobalVaribles )
		{	ALERT( at_console,"%s=%s\n", iterVars->first.c_str(), iterVars->second.c_str());
		}
		ALERT( at_console,"\n");

		vector<MechCombo_s>::iterator Combo = Style->Combos.begin();
		while( Combo != Style->Combos.end() )
		{	// Print combo name
			ALERT( at_console, "%s\n", Combo->szCombo.c_str() );
			
			// Print Local varibles
			ALERT( at_console,"local varibles:\n");
			ForEachIn( iterVars, Combo->szVaribles )
			{	ALERT( at_console,"%s=%s\n", iterVars->first.c_str(), iterVars->second.c_str());
			}
			ALERT( at_console,"\nMoves:\n");

			// Print combo move list
			vector<std::string>::iterator Moves = Combo->szMoves.begin();
			while( Moves != Combo->szMoves.end() )
			{
				ALERT( at_console, "%s\n", Moves->c_str() );

				Moves++;
			}
			Combo++;
		}

		Style++;
	}

	vector<std::string>::iterator Precache;

	ForEachIn( Precache, g_MMParse.m_Precache )
	{
		ALERT( at_console, "Precache: %s\n", Precache->c_str() );
	}
}

void CMech_Parse::RemoveNullLines( MechScript_s *pScript )
{
	// first count valid lines
	vector<std::string>::iterator String = pScript->ScriptData.begin();
	int i = 0, iElements = 0;

	while( String != pScript->ScriptData.end() )
	{
		if( String->empty( ) )
		{
			String = pScript->ScriptData.erase( String );

		}
		else
		{
			String++;
		}
	}
}

void CMech_Parse::RemoveLeadingSpaces( std::string *Line )
{
	if( !Line->empty() )
	{
		const char *pData = Line->c_str();

		// 9 is the tab char, and ' ' is a space remove em
		while( *pData == ' ' || *pData == 9 )
		{
			pData ++;
		}

		*Line = pData;
	}
}


void CMech_Parse::SortScript( MechScript_s *pScript )
{
	int i = 0;


	for( i = 0; i < pScript->ScriptData.size(); i++ )
	{
		if( *pScript->ScriptData[i].c_str() == '<' )
		{
			// parse out the precache
			ParsePrecache( i, pScript );
		}
		else if( *pScript->ScriptData[i].c_str() == '[' )	//start of style code
		{
			// call style extractor
			ParseStyle( i, pScript );
		}
	}
}

void CMech_Parse::ParsePrecache( int &iLine, MechScript_s *pScript )
{
	int iPos = 1;

	while( pScript->ScriptData[iLine].c_str()[iPos] == ' ' || pScript->ScriptData[iLine].c_str()[iPos] == 9 )
	{
		iPos ++;
	}

	if( !memcmp( &pScript->ScriptData[iLine].c_str()[iPos], "precache", sizeof( char ) * 8 ) )
	{
		iLine++;	//moves to the {
		iLine++;	// moves to the vaild line

		while( *pScript->ScriptData[iLine].c_str() != '}' )
		{
			m_Precache.push_back( pScript->ScriptData[iLine] );
			iLine++;
		}
	}
}

void CMech_Parse::ParseStyle( int &iLine, MechScript_s *pScript )
{
	// we have found the start of a style tag
	// frist parse out style name
	int i = 1;		// we already know the first char
	//char szNewStyle[64];
	bool blGo = true;

	MechStyle_s Style;
	m_Styles.push_back( Style );
	// this allows us to edit it
	vector<MechStyle_s>::reverse_iterator pStyle = m_Styles.rbegin();

	while( pScript->ScriptData[iLine].c_str()[i] != ']' )
	{
		// store in our script var
		// remove spaces
		if( pScript->ScriptData[iLine].c_str()[i] == ' ' || pScript->ScriptData[iLine].c_str()[i] == 9 )
		{
			i++;
		}
		else
		{
			pStyle->szStyle += pScript->ScriptData[iLine].c_str()[i];
			i++;
		}
	}
//	ALERT(at_console,"STYLE PARSE: %s\n", pStyle->szStyle.c_str());
//	ALERT(at_console,"STYLE PARSE: %s\n", pStyle->szStyle.c_str());

	iLine ++;

	//ALERT( at_console, "%s\n", Style->szStyle.c_str() );

	while( blGo == true && iLine < pScript->ScriptData.size() )
	{
		if( *pScript->ScriptData[iLine].c_str() == '(' ) // we have found a combo or global set
		{
			int i = 1;

			while( pScript->ScriptData[iLine].c_str()[i] == ' ' || pScript->ScriptData[iLine].c_str()[i] == 9 )
			{
				i++;
			}
			ParseCombo( iLine, pScript );
			continue;
		}

		if( *pScript->ScriptData[iLine].c_str() == '[' )
		{
			int i = 1;

			while( pScript->ScriptData[iLine].c_str()[i] == ' ' || pScript->ScriptData[iLine].c_str()[i] == 9 )
			{
				i++;
			}

			int iStyle = 3;

			if( !memcmp( &pScript->ScriptData[iLine].c_str()[i], "end", sizeof( char ) * iStyle ) )
			{
				blGo = false;
			}
		}

		if(blGo)
			iLine++;	// should move us to next line
	}

}

void CMech_Parse::ParseCombo( int &iLine, MechScript_s *pScript )
{
	int i = 1;
	MechCombo_s newCombo;

	vector<MechStyle_s>::reverse_iterator Style = m_Styles.rbegin();

	Style->Combos.push_back( newCombo );

	vector<MechCombo_s>::reverse_iterator pCombo = Style->Combos.rbegin();

	while( pScript->ScriptData[iLine].c_str()[i] != ')' )
	{
		if( pScript->ScriptData[iLine].c_str()[i] == ' ' || pScript->ScriptData[iLine].c_str()[i] == 9 )
		{
			i++;
		}
		else
		{
			// store combo name
			pCombo->szCombo += pScript->ScriptData[iLine].c_str()[i];
			i++;
		}
	}

	iLine++;	// next line is supposed to be a {
	iLine++;	// should be next valid line

	while( *pScript->ScriptData[iLine].c_str() != '}' )
	{
		// parse command
		ParseCommands( iLine, pScript );
		iLine ++;
	}

	// gets us off the  } command
	iLine++;

	// If our combo is a "global", remove it
	if(pCombo->szCombo == "global")
	{	Style->Combos.pop_back();
	}
}

void CMech_Parse::MakeLower( const char *pLine )
{
	char *pData = ( char * )pLine;
	int iSize = strlen( pLine );

	for( int i = 0; i < iSize ; i++ )
	{
		*pData = tolower( *pData );
		pData++;
	}
}

void CMech_Parse::ParseCommands( int &iLine, MechScript_s *pScript )
{
	// parse all commands out of lines
	// read to end of first word so  '=' or '\n'

	std::string String;

	int iPos = 0;

	while( pScript->ScriptData[iLine].c_str()[iPos] != '=' && pScript->ScriptData[iLine].c_str()[iPos] != '\n' )
	{
		if( pScript->ScriptData[iLine].c_str()[iPos] == ' ' || pScript->ScriptData[iLine].c_str()[iPos] == 9 )
		{
			iPos++;
		}
		else
		{
			// store string 
			// store this elsewhere since its not really a command
			String += pScript->ScriptData[iLine].c_str()[iPos];
			iPos++;
		}
	}

	// move off of the = sign
	iPos++;

	vector<MechStyle_s>::reverse_iterator Style = m_Styles.rbegin();
	vector<MechCombo_s>::reverse_iterator Combo = Style->Combos.rbegin();
	if( String == "combo" && Combo->szCombo != "global" )
	{

		while( pScript->ScriptData[iLine].c_str()[iPos] != '\n' && pScript->ScriptData[iLine].c_str()[iPos] != '\0' )
		{
			std::string ComboData;
			Combo->szMoves.push_back( ComboData );
			vector<std::string>::reverse_iterator pData = Combo->szMoves.rbegin();

			while( NotOperand( pScript->ScriptData[iLine].c_str()[iPos] ) )
			{
				if( pScript->ScriptData[iLine].c_str()[iPos] == ' ' || pScript->ScriptData[iLine].c_str()[iPos] == 9 )
				{
					iPos++;
				}
				else
				{
					// store string 
					*pData += pScript->ScriptData[iLine].c_str()[iPos];
					iPos++;
				}
			}

			if( pScript->ScriptData[iLine].c_str()[iPos] != '\n' && pScript->ScriptData[iLine].c_str()[iPos] != '\0' )
			{
				//move off of command, but not end line
				iPos++;
			}

			// command found process it
		}
	}
	// Parse like a normal varible
	else
	{	vector<MechStyle_s>::reverse_iterator Style = m_Styles.rbegin();
		vector<MechCombo_s>::reverse_iterator Combo = Style->Combos.rbegin();
		std::string strVarData;

		while( pScript->ScriptData[iLine].c_str()[iPos] != '\n' && pScript->ScriptData[iLine].c_str()[iPos] != '\0' )
		{
			if( pScript->ScriptData[iLine].c_str()[iPos] == ' ' || pScript->ScriptData[iLine].c_str()[iPos] == 9 )
			{
				iPos++;
			}
			else
			{	// store string 
				strVarData += pScript->ScriptData[iLine].c_str()[iPos];
				iPos++;
			}
		}
		// Add to varible
		if(Combo->szCombo == "global")
		{	Style->szGlobalVaribles[String] = strVarData;
		}
		else
		{	Combo->szVaribles[String] = strVarData;
		}
	}

	// otherwise no more commands to parse out
}

bool CMech_Parse::NotOperand( char cOperand )
{
	bool blIsOp = true;

	if( cOperand == '+' )
		blIsOp = false;
	else if( cOperand == '\n' )
		blIsOp = false;
	else if( cOperand == '\0' )
		blIsOp = false;

	return blIsOp;
}