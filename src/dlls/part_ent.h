/**********************************************************
*			PARTICLE SYSTEM ENTITYS
***********************************************************
*	Purpose: Map based entitys for particle system
*	Created On: 1/9/04
*	Last Edited: 1/9/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

extern int gmsgPartMsg;

struct colorRGBA
{
	// colors
	float r, g, b, a;
};

class CPartBrush : public CBaseEntity
{
public:
	void	Spawn( void );
	void	KeyValue( KeyValueData *pkvd );
	void	MsgClient( CBasePlayer *pPlayer );
private:
	int m_iPPS, m_iBounces, m_iAngLock, m_iFlags;
	float m_flSysLife, m_flDelay, m_flPartDelay, m_flStSize, m_flEdSize, m_flBncScl, m_flGrav, m_flFR, m_flLife;
	colorRGBA m_clEdCol, m_clStCol;
	vec3_t m_vAng, m_vPMaxVel, m_vPMinVel, m_vMaxVel, m_vMinVel;
	char m_rgcSpr[64];
};

class CPartPoint : public CBaseEntity
{
public:
	void	Spawn( void );
	void	KeyValue( KeyValueData *pkvd );
	void	MsgClient( CBasePlayer *pPlayer );
	void	Precache( void );
private:
	int m_iPPS, m_iBounces, m_iAngLock, m_iFlags;
	float m_flSysLife, m_flDelay, m_flPartDelay, m_flStSize, m_flEdSize, m_flBncScl, m_flGrav, m_flFR, m_flLife;
	colorRGBA m_clEdCol, m_clStCol;
	vec3_t m_vAng, m_vPMaxVel, m_vPMinVel, m_vMaxVel, m_vMinVel;
	char m_rgcSpr[64];
};