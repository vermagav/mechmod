/*
* Boost FX
* @version 12/21/03
* @author: Joseph 'Pcjoe' Florencio
*/

class CBoost : public CBaseEntity
{
public:
	static CBoost *CreateBoost( CBasePlayer *pOwner, BOOL fDoubleJump );
	void Precache( void );
	void Remove( void );
	void EXPORT RemoveThink( void );
	void Spawn( void );
	void SendBoostMsg( void );
	BOOL m_fDoubleJump;
private:
	CBasePlayer *m_pOwner;
	float m_flDieTime;
};
