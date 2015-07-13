/*
* Mechmod base entity weapon system
* @version: 1/18/04
* @author: Joseph 'Pcjoe' Florencio
*/

#ifndef MMWEAPONENTS_H
#define MMWEAPONENTS_H

// Damage
#define	GENERIC_MISSLE_DAMAGE		20
#define VF1J_ALPHABOMB_DAMAGE		120
#define	AF49_WASTEDMISSLE_DAMAGE	150
#define	FREEDOM_5WAY_BR_DAMAGE		100
#define	FREEDOM_5WAY_YG_DAMAGE		60
#define MK2_NUKE_DAMAGE				400
#define FREEDOM_BUSTER_DAMAGE		270

// Speed
#define GENERIC_MISSLE_SPEED		700
#define VF1J_ALPHABOMB_SPEED		1500
#define AF49_WASTEDMISSLE_SPEED		500
#define FREEDOM_5WAY_SPEED			1100
#define MK2_NUKE_SPEED				500
#define FREEDOM_BUSTER_SPEED		1500

// Shoot down reduction
#define GENERIC_MISSLE_SD			0.5
#define AF49_WASTEDMISSLE_SD		0.2
#define MK2_NUKE_SD					0.1

// Explosion radius
#define GENERIC_MISSLE_RADIUS		300
#define VF1J_ALPHABOMB_RADIUS		450
#define AF49_WASTEDMISSLE_RADIUS	550
#define FREEDOM_5WAY_RADIUS			200
#define MK2_NUKE_RADIUS				800
#define FREEDOM_BUSTER_RADIUS		650

// MISC
#define FREEDOM_5WAY_DOTCUT			0.999

class CBaseProjectile : public CBaseEntity
{
public:
	/*
	* Spawn
	*/
	virtual void Spawn( void );

	/*
	* Explode on touch
	*/
	virtual void EXPORT ExplodeTouch( CBaseEntity *pOther );

	/*
	* Add projectile trail
	*/
	virtual void AddTrail( void ) { }

	/*
	* Jason: Remove missle trail
	*/
	virtual void RemoveTrail( void ) { }

	/*
	* Add client renderer
	*/
	virtual void AddClientRenderer( void ) { }

	/*
	* Remove client renderer
	*/
	virtual void RemoveClientRenderer( void ) { }

	/*
	* Emit Explosion FX
	*/
	virtual void EmitExplosionFX( void );

	/*
	* Explode
	*/
	virtual void ExplodeProjectile( void );
	
	/*
	* If this projectile can be "shot down"
	*/
	virtual BOOL CanShootDown( void ) { return FALSE; }

	/*
	* Shoot down projectile with rapid fire
	*/
	virtual void ShootDown( void );

	/*
	* Remove Projectile
	*/
	virtual void Remove( void );

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	virtual int GetStartSpeed( void ) { return 0; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	virtual int GetDamage( void ) { return 0; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	virtual int GetExplosionRadius( void ) { return 0; }

	/*
	* Get Shoot Down Reducer ( if shoot down is permitted )
	* @return float: Reducer
	*/
	virtual float GetShootDownReducer( void ) { return 0; }

	/*
	* Get Charge Amount
	* @return float: Charge amount in percent ( 0 - 1 )
	*/
	virtual float GetChargeAmt( void ) { return 1; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	virtual const char *GetEntClassName( void ) { return ""; }

	/*
	* Get Model
	* @return char*: model name
	*/
	virtual const char *GetModel( void ) { return ""; }

	/*
	* Get Size
	* @return Vector: Size
	*/
	virtual Vector GetSize( void ) { return Vector(0,0,0); }

	/*
	* Added by grOOvy: 26/07/04
	* Get Weapon Increment
	* @return float: Amount of score to be increased
	*/
	virtual float GetScoreIncrement( void ) { return 0; }
    


	int		m_iTrail;		// Trail Sprite
	float	m_flSpeed;		// Speed
	BOOL	m_fExploded;	// This projectile already exploded
	CBasePlayer *m_pOwner;	// Owner of missle
	Vector	vecVelocity;	// Velocity
};

class CBaseMissle : public CBaseProjectile
{	
public:
	/*
	* Precache
	*/
	virtual void Precache( void );

	/*
	* Classify
	*/
	virtual int Classify ( void ) { return CLASS_MISSLE; }

	/*
	* Make Homing Projectile (Missle)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pLock: Entity this missle is locked onto to (NULL if none)
	* @param CBasePlayer *pOwner: Owner of this missle
	*/
	void MakeHomingMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner );

	/*
	* Make Projectile (Missle)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pOwner: Owner of this missle
	*/
	void MakeMissle( Vector vecOrigin, Vector vecDir,  CBasePlayer *pOwner );

	/*
	* Follow think
	*/
	virtual void EXPORT FollowThink( void );

	/*
	* Add missle trail
	*/
	virtual void AddTrail( void );

	// added by jason
	/*
	* Remove missle trail
	*/
	virtual void RemoveTrail( void );
	
	/*
	* If this projectile can be "shot down"
	*/
	virtual BOOL CanShootDown( void ) { return TRUE; }

	/*
	* Remove Missle
	*/
	virtual void Remove( void );

	/*
	* Get Rate of turn
	* @return float: ROT
	*/
	virtual float GetROT( void ) { return 1; }

	/*
	* Get Rate of turn decrease when entitiy isnt visible
	*/
	virtual float GetROTDecrease( void ) { return 0; }

	/*
	* Get Max Viewable DOT
	*/
	virtual float GetMaxDOT( void ) { return VIEW_FIELD_WIDE; }

	float m_flROT;
	float m_flROTDecrease;
	float m_flMaxDot;
	CBaseEntity *m_pLock;	// Entity missle is tracking
};

class CGenericMissle : public CBaseMissle
{
public:	
	/*
	* Make Homing Projectile (Missle)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pLock: Entity this missle is locked onto to (NULL if none)
	* @param CBasePlayer *pOwner: Owner of this missle
	*/
	static CGenericMissle *CreateHomingMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner );

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	int GetStartSpeed( void ) { return GENERIC_MISSLE_SPEED; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	int GetDamage( void ) { return GENERIC_MISSLE_DAMAGE; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	int GetExplosionRadius( void ) { return GENERIC_MISSLE_RADIUS; }

	/*
	* Get Shoot Down Reducer ( if shoot down is permitted )
	* @return float: Reducer
	*/
	float GetShootDownReducer( void ) { return GENERIC_MISSLE_SD; }

	/*
	* Get Charge Amount
	* @return float: Charge amount in percent ( 0 - 1 )
	*/
	float GetChargeAmt( void ) { return 1; }

	/*
	* Get Rate of turn
	* @return float: ROT
	*/
	virtual float GetROT( void ) { return 0.1; }

	/*
	* Get Rate of turn decrease when entitiy isnt visible
	*/
	virtual float GetROTDecrease( void ) { return RANDOM_FLOAT(0.1,0.2); }

	/*
	* Get Max Viewable DOT
	*/
	virtual float GetMaxDOT( void ) { return VIEW_FIELD_WIDE; }

	/*
	* Added by grOOvy: 26/07/04
	* Get Score Increment
	* @return float: score increment for the generic missile
	*/
	virtual float GetScoreIncrement ( void ) { return 3; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	const char *GetEntClassName( void ) { return "genericmissle"; }

	/*
	* Get Model
	* @return char*: model name
	*/
	// modifed by jason
	const char *GetModel( void ) { return "models/w_missle.mdl"; }

	/*
	* Get Size
	* @return Vector: Size
	*/
	Vector GetSize( void ) { return Vector(6,6,6); }
};

class CWastedMissle : public CBaseMissle
{
public:
	/*
	* Precache
	*/
	virtual void Precache( void );

	/*
	* Create Projectile (Missle)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pLock: Entity this missle is locked onto to (NULL if none)
	* @param CBasePlayer *pOwner: Owner of this missle
	*/
	static CWastedMissle *CreateMissle( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner );

	/*
	* Add missle trail
	*/
	void AddTrail( void );

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	int GetStartSpeed( void ) { return AF49_WASTEDMISSLE_SPEED; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	int GetDamage( void ) { return AF49_WASTEDMISSLE_DAMAGE; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	int GetExplosionRadius( void ) { return AF49_WASTEDMISSLE_RADIUS; }

	/*
	* Get Shoot Down Reducer ( if shoot down is permitted )
	* @return float: Reducer
	*/
	float GetShootDownReducer( void ) { return AF49_WASTEDMISSLE_SD; }

	/*
	* Get Charge Amount
	* @return float: Charge amount in percent ( 0 - 1 )
	*/
	float GetChargeAmt( void ) { return 1; }

	/*
	* Get Rate of turn
	* @return float: ROT
	*/
	virtual float GetROT( void ) { return 0.9; }

	/*
	* Get Rate of turn decrease when entitiy isnt visible
	*/
	virtual float GetROTDecrease( void ) { return RANDOM_FLOAT(0.1,0.2); }

	/*
	* Get Max Viewable DOT
	*/
	virtual float GetMaxDOT( void ) { return VIEW_FIELD_WIDE; }

	/*
	* Added by grOOvy: 26/07/04
	* Get Score Increment
	* @return float: score increment for the wasted missile
	*/
	virtual float GetScoreIncrement ( void ) { return 6; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	const char *GetEntClassName( void ) { return "wastedmissle"; }

	/*
	* Get Model
	* @return char*: model name
	*/
	const char *GetModel( void ) { return "models/w_wastedmissle.mdl"; }

	/*
	* Get Size
	* @return Vector: Size
	*/
	Vector GetSize( void ) { return Vector(6,6,6); }
};

class CFreedomBlast : public CBaseMissle
{
public:	
	/*
	* Make Homing Freedom Blast
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pLock: Entity this missle is locked onto to (NULL if none)
	* @param CBasePlayer *pOwner: Owner of this missle
	*/
	static CFreedomBlast *CreateCFreedomBlast( Vector vecOrigin, Vector vecDir, CBasePlayer *pLock, CBasePlayer *pOwner );

	/*
	* Add projectile trail
	*/
	virtual void AddTrail( void ) {}

	/*
	* Remove missle trail
	*/
	virtual void RemoveTrail( void );

	/*
	* Follow think
	*/
	virtual void EXPORT FollowThink( void );

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	int GetStartSpeed( void ) { return FREEDOM_5WAY_SPEED; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	int GetDamage( void ) { return m_flDamage; }

	/*
	* Set Damage
	* @param float flDmg: Damage we're setting
	*/
	void SetDamage( float flDmg ) { m_flDamage = flDmg; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	int GetExplosionRadius( void ) { return FREEDOM_5WAY_RADIUS; }

	/*
	* If this projectile can be "shot down"
	*/
	virtual BOOL CanShootDown( void ) { return FALSE; }

	/*
	* Get Charge Amount
	* @return float: Charge amount in percent ( 0 - 1 )
	*/
	float GetChargeAmt( void ) { return 1; }

	/*
	* Get Rate of turn
	* @return float: ROT
	*/
	virtual float GetROT( void ) { return 0.4; }

	/*
	* Get Rate of turn decrease when entitiy isnt visible
	*/
	virtual float GetROTDecrease( void ) { return 0.0; }

	/*
	* Get Max Viewable DOT
	*/
	virtual float GetMaxDOT( void ) { return VIEW_FIELD_WIDE; }

	/*
	* Added by grOOvy: 26/07/04
	* Get Score Increment
	* @return float: score increment for the freedom blast
	*/
	virtual float GetScoreIncrement ( void ) { return 15; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	const char *GetEntClassName( void ) { return "freedomblast"; }

	/*
	* Get Model
	* @return char*: model name
	*/
	// modifed by jason
	const char *GetModel( void ) { return "models/w_missle.mdl"; }

	/*
	* Get Size
	* @return Vector: Size
	*/
	Vector GetSize( void ) { return Vector(6,6,6); }

private:
	// Damage beam should inflict
	float m_flDamage;
};

class CBaseEnergyProjectile : public CBaseProjectile
{
public:
	/*
	* Make Projectile (Energy)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pOwner: Owner of this energy projectile
	* @param float flCharged: Charged amount (0-1)
	* 
	*/
	virtual void MakeEnergyProjectile( Vector vecOrigin, Vector vecDir, CBasePlayer *pOwner, float flCharged );

	/*
	* Classify
	*/
	virtual int Classify ( void ) { return CLASS_ENERGY; }

	/*
	* Get Charge Amount
	* @return float: Charge amount in percent ( 0 - 1 )
	*/
	virtual float GetChargeAmt( void ) { return m_flChargedPercent; }

	/*
	* Get Max/Min Size of projectile
	* @return int: Size
	*/
	virtual int GetMinSize( void ) { return 1; }
	virtual int GetMaxSize( void ) { return 1; }

	/*
	* Current size taking charge percent into account (int)
	* @return int: Entity Size
	*/
	virtual int GetScaledSize( void ) { return (GetMinSize() + (GetMaxSize()-GetMinSize())*GetChargeAmt()); }

	/*
	* Get Size
	* @return Vector: Size
	*/
	virtual Vector GetSize( void ) { return (Vector(1,1,1)*(GetScaledSize()/2)); }

protected:
	float m_flChargedPercent;
};

class CAlphaBomb : public CBaseEnergyProjectile
{
public:
	/*
	* Precache
	*/
	virtual void Precache( void );

	/*
	* Create Projectile (Alphabomb)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pOwner: Owner of this energy projectile
	* @param float flCharged: Charged amount (0-1)
	*/
	static CAlphaBomb *CreateAlphaBomb( Vector vecOrigin, Vector vecDir,  CBasePlayer *pOwner, float flCharged );

	
	/*
	* Get Max/Min Size of projectile
	* @return int: Size
	*/
	virtual int GetMinSize( void ) { return 5; }
	virtual int GetMaxSize( void ) { return 20; }

	/*
	* Added by grOOvy: 26/07/04
	* Get Score Increment
	* @return float: score increment for the alpha bomb
	*/
	virtual float GetScoreIncrement ( void ) { return 20; }

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	int GetStartSpeed( void ) { return VF1J_ALPHABOMB_SPEED; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	int GetDamage( void ) { return VF1J_ALPHABOMB_DAMAGE; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	int GetExplosionRadius( void ) { return VF1J_ALPHABOMB_RADIUS; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	const char *GetEntClassName( void ) { return "alphabomb"; }

	/*
	* Get Model
	* @return char*: model name
	*/
	const char *GetModel( void ) { return "models/w_alphabomb.mdl"; }

	void RemoveTrail( void );
};

class CMKNuke : public CBaseEnergyProjectile
{
public:
	/*
	* Precache
	*/
	virtual void Precache( void );

	/*
	* Create Projectile (nuke)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pOwner: Owner of this energy projectile
	* @param float flCharged: Charged amount (0-1)
	*/
	static CMKNuke *CreateNuke( Vector vecOrigin, Vector vecDir,  CBasePlayer *pOwner, float flCharged );

	
	/*
	* Get Max/Min Size of projectile
	* @return int: Size
	*/
	virtual int GetMinSize( void ) { return 5; }
	virtual int GetMaxSize( void ) { return 20; }

	/*
	* Added by grOOvy: 26/07/04
	* Get Score Increment
	* @return float: score increment for the alpha bomb
	*/
	virtual float GetScoreIncrement ( void ) { return 50; }

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	int GetStartSpeed( void ) { return MK2_NUKE_SPEED; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	int GetDamage( void ) { return MK2_NUKE_DAMAGE; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	int GetExplosionRadius( void ) { return MK2_NUKE_RADIUS; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	const char *GetEntClassName( void ) { return "nuke"; }

	/*
	* Get Model
	* @return char*: model name
	*/
	const char *GetModel( void ) { return "models/w_nuke.mdl"; }
};

class CBusterBig : public CBaseEnergyProjectile
{
public:
	/*
	* Precache
	*/
	virtual void Precache( void );

	/*
	* Create Projectile (busterbig)
	* @param Vector vecOrigin: Origin of entity
	* @param Vector vecDir: Direction missle will be traveling in
	* @param CBasePlayer *pOwner: Owner of this energy projectile
	* @param float flCharged: Charged amount (0-1)
	*/
	static CBusterBig *CreateBusterBig( Vector vecOrigin, Vector vecDir,  CBasePlayer *pOwner, float flCharged );

	
	/*
	* Get Max/Min Size of projectile
	* @return int: Size
	*/
	virtual int GetMinSize( void ) { return 5; }
	virtual int GetMaxSize( void ) { return 20; }

	/*
	* Added by grOOvy: 26/07/04
	* Get Score Increment
	* @return float: score increment for the alpha bomb
	*/
	virtual float GetScoreIncrement ( void ) { return 30; }

	/*
	* Get Start Speed
	* @return int: Speed
	*/
	int GetStartSpeed( void ) { return FREEDOM_BUSTER_SPEED; }

	/*
	* Get Damage
	* @return int: Damage
	*/
	int GetDamage( void ) { return FREEDOM_BUSTER_DAMAGE; }

	/*
	* Get Explosion Radius
	* @return int: Radius
	*/
	int GetExplosionRadius( void ) { return FREEDOM_BUSTER_RADIUS; }

	/*
	* Get Class Name
	* @return char*: classname
	*/
	const char *GetEntClassName( void ) { return "busterbig"; }

	/*
	* Get Model
	* @return char*: model name
	*/
	const char *GetModel( void ) { return "models/w_alphabomb.mdl"; }

	void RemoveTrail( void );
};
#endif