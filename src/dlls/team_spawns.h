/**********************************************************
*			TEAM SPAWN ENTITY
***********************************************************
*	Purpose: Team Spawn Points
*	Created On: 1/14/04
*	Last Edited: 1/14/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#define		TEAM_OBSERVER	0
#define		TEAM_EARTH		1
#define		TEAM_SPACE		2

class CTeamSpawn : public CBaseEntity
{
public:
	void	Spawn( void );
	void	KeyValue( KeyValueData *pkvd );
private:
};