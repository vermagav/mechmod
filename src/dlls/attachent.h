/*
* Attachment Entity
* @version: 8/08/04
* @author: Joseph 'Pcjoe' Florencio
*/

/*
* Attachment Entity
*/
class CAttachEnt : public CBaseAnimating
{
public:
	/*
	* Spawn
	*/
	void Spawn( void );

	/*
	* Create Model
	*/
	static CAttachEnt *CreateModel( CBasePlayer *pOwner, char *pModel );

	/*
	* Precache
	*/
	void Precache( void );

	/*
	* Animate Think
	*/
	void EXPORT AnimateThink( void );

	/*
	* Change Ent Model
	*/
	void SetModel( char *pModel );

	/*
	* Set Model we're attached to
	*/
	void SetAttachedPlayer( CBasePlayer *pPlayer ) { pPlayer = pAttachPlr; }

	/*
	* Disable attached model (cleanup)
	*/
	void Disable( void );

	// Animation System
	CEntAnimation m_Anim;
private:
	// Attached to this player
	CBasePlayer *pAttachPlr;
};