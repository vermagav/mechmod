/******************************************************************
*				MECHMOD SEIGE GAMERULES
*******************************************************************
*	Author:		Jason "ssjason123" Matson
*	Created:	1-28-04
*	Purpose:	Seige gamemode rules.
******************************************************************/

class CMechModSeige : public CHalfLifeTeamplay
{
public:
	// Constructor
	CMechModSeige( void );

	virtual void	Think( void );
	virtual	BOOL	IsSeige( void );
	virtual	BOOL	IsAllowedToSpawn( CBaseEntity *pEntity );
	void			ResetRound( void );
	void			ResetData( void );

private:
	// Destructor
	~CMechModSeige( void );
	bool			m_blCanSpawn[MAX_TEAMS];
	float			m_flStartTime;
	bool			m_blRoundStart;
};