/******************************************************************
*				MECHMOD FUNC OBJECTIVE
*******************************************************************
*	Author:		Jason "ssjason123" Matson
*	Created:	1-28-04
*	Purpose:	Objective entity for Seige game rules
******************************************************************/

class CMMObjective : public CBaseEntity
{
public:
	void	Spawn( void );
	void	Precache( void );
	void	Reset( void );
	void	Die( void );
	void	KeyValue( KeyValueData* pkvd );
	virtual int TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, 
							float flDamage, int bitsDamageType );
	bool	IsTeamAlive( int iTeam );
private:
	int		m_iLife;	// entity life
	int		m_iTeam;	// team the entity is on
	bool	m_blAlive;	// living entity?
	int		m_iPoints;	// points for killiing
	int		m_idShard;	// gib model
};
class CNewFuncWind : public CBaseEntity
{
public:
	void	Spawn( void );
	void	KeyValue( KeyValueData *pkvd );
	void	Precache( void );
private:
};
