/********************************************************** 
*                         MM Script Parser  
*********************************************************** 
*     Purpose: Parse melee scripts into usefull info 
*     Created On: 5/22/04 
*     Last Edited: 5/22/04 
*     Created By: Jason "ssjason123" Matson 
*     Created For: MechMod 
**********************************************************/

struct MechScript_s
{
	std::vector<std::string> ScriptData;
	std::string	szName;
};

struct MechCombo_s
{
	std::string szCombo;
	std::vector<std::string> szMoves;
	std::map<std::string,std::string> szVaribles; 
};

struct MechStyle_s
{
	std::string szStyle;
	std::vector<MechCombo_s> Combos;
	std::map<std::string,std::string> szGlobalVaribles; 
};

class CMech_Parse
{
public:
	~CMech_Parse( void );
	CMech_Parse( void );
	bool ParseScript( std::string File );
	void PrintScript( std::list<MechScript_s>::iterator Script );
	std::vector<MechStyle_s> m_Styles;
	std::vector<std::string> m_Precache;
private:
	// checks if line is a comment
	void RemoveComments( std::string *pLine );
	bool ReadScript( char *pFile, MechScript_s *pScript );
	void RemoveNullLines( MechScript_s *pScript );
	void MakeLower( const char *pLine );
	void RemoveLeadingSpaces( std::string *Line );

	void SortScript( MechScript_s *pScript );

	void ParsePrecache( int &iLine, MechScript_s *pScript );
	void ParseStyle( int &iLine, MechScript_s *pScript );
	void ParseCombo( int &iLine, MechScript_s *pScript );
	void ParseCommands( int &iLine, MechScript_s *pScript );
	bool NotOperand( char cOperand );

	std::list<MechScript_s> m_Scripts;
};

extern CMech_Parse g_MMParse;
// end jason

class CSaber;

class CSaberStyle
{
public:
	// Constructor/Deconstructor
	CSaberStyle();
	~CSaberStyle();

#ifndef CLIENT_DLL
	/*
	* Set saber owner
	* @param CSaber *pSaber: Saber base class
	*/
	void SetSaber( CSaber *pSaber ) { m_pSaber = pSaber; }
	
	/*
	* Reset Queued List
	*/
	void ResetQueuedList( void ) { if(!m_QueuedMoves.empty()) m_QueuedMoves.clear(); }

	/*
	* Update saber
	* @return BOOL: If we should attempt to update saber data
	*/
	BOOL UpdateSaber( void );

	/*
	* Get next set of saber data
	*/
	saber_move_s GetNextSaberData( void );

	/*
	* Add Saber Move
	*/
	void AddSaberMove( saber_move_s move ) { m_QueuedMoves.push_back( move ); }

	/*
	* Updata data from script
	*/
	void UpdateDataFromScript( void );

	/*
	* Get Global Data
	*/
	std::string GetGlobalString( char *pDataName );
	BOOL GetGlobalBOOL( char *pDataName );
	float GetGlobalFloat( char *pDataName );
	int GetGlobalInt( char *pDataName );

	/*
	* Get Current Style from parse file
	*/
	std::vector<MechStyle_s>::iterator GetStyle( void );

	/*
	* Get Local Varible
	*/
	std::string GetVaribleString( char *pSearch, std::map<std::string,std::string> data );
	BOOL GetVaribleBOOL( char *pSearch, std::map<std::string,std::string> data );
	float GetVaribleFloat( char *pSearch, std::map<std::string,std::string> data );
	int GetVaribleInt( char *pSearch, std::map<std::string,std::string> data );
private:
	CSaber *m_pSaber;
	std::list<saber_move_s> m_QueuedMoves;
#endif
};