/*
* Mechmod base weapon system
* @version: 1/14/04
* @author: Joseph 'Pcjoe' Florencio
*/

#ifndef MMWEAPONS_H
#define MMWEAPONS_H

#pragma warning (disable:4530)
#pragma warning (disable:4786)

#include "mmweaponents.h"

#define MAX_LOCKS	8
#define MAX_MISSLE_LOCKS	16
#define MAX_MISSLES	16

/*
* Power Cost
*/
#define VF1J_MISSLE_COST	35
#define AF49_WMISSLE_COST	200
#define AF49_MISSLE_COST	35
#define MK2_BARRELS_COST	200
#define MK2_NUKE_COST		500
#define FREEDOM_5WAY_COST	150
#define FREEDOM_BUSTERBIG_COST	200


/*
* Slots
*/
#define WP_SLOT_MELEE		1
#define WP_SLOT_RAPID		2
#define WP_SLOT_LOCKON		3
#define WP_SLOT_CHARGE		4
#define WP_SLOT_MISC		5

/*
* Weapon positions in slots
*/
// Melee
#define WP_POS_VF1JSABER	0
#define WP_POS_DSCYTHE		1
#define WP_POS_FREEDOMSABER	2
#define WP_POS_AF49TWINBLADE 3

// Rapid
#define WP_POS_VF1JRAPID	0
#define WP_POS_AF49RAPID	1
#define WP_POS_DSSHIELDGUN	2
#define WP_POS_FREEDOMRAPID	3
#define WP_POS_MK2RAPID		4

// Lockon
#define WP_POS_VF1JHOMING	0
#define WP_POS_AF49HOMING	1
#define WP_POS_AF49WASTED	2
#define WP_POS_FREEDOM5WAY	3
#define WP_POS_MK2HOMING	4
#define WP_POS_MK2BARRELS	5

// Charge
#define WP_POS_VF1JALPHA	0
#define WP_POS_FREEDOMBIG	1
#define WP_POS_MK2NUKE		2

// Misc

// Lock data
typedef struct
{	int		iIndex;
	float	flTime;
	bool	blLocked;
}lockdata_t;

/*
* -------------------
* Base Classes
* -------------------
*/

class CMMWeapon : public CBasePlayerWeapon
{
public:
	/*
	* Spawn
	*/
	virtual void Spawn( );

	/*
	* Precache
	*/
	virtual void Precache( ) = 0;

	/*
	* Custom Spawn
	*/
	virtual void CustomSpawn( void ) {};

	/*
	* Add Weapon to player
	* @param CBasePlayer *pPlayer: Player we're adding this weapon to
	*/
	virtual int AddToPlayer( CBasePlayer *pPlayer );

	/*
	* Use Decrement
	* @return BOOL: If Decrement is being used ( only used for client weapons )
	*/
	virtual BOOL UseDecrement( void );

	/*
	* Use power for primary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerPrimary( void ) { return FALSE; }

	/*
	* Use power for secondary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerSecondary( void ) { return FALSE; }

	/*
	* Get information about the current weapon
	* @param ItemInfo *p: Item information struct
	* @return int: always return 1 (?)
	*/
	virtual int GetItemInfo( ItemInfo *p );

	/*
	* Get item info for specific weapon
	*/
	virtual void GetItemInfoCustom( ItemInfo *p ) {};

	/*
	* Deploy weapon
	* @return BOOL: If this weapon was deployed successfully
	*/
	virtual BOOL Deploy( );
	
	/*
	* Get Weight
	*/
	virtual int GetWeight( void ) { return 0; }

	/*
	* Mechmod weapon
	*/
	virtual BOOL MMWeapon( void ) { return TRUE; }

	/*
	* Melee Weapon
	*/
	virtual BOOL MeleeWeapon( void ) { return FALSE; }

	/*
	* Rapid Fire Weapon
	*/
	virtual BOOL RapidFireWeapon( void ) { return FALSE; }

	/*
	* Charge Weapon
	*/
	virtual BOOL ChargeWeapon ( void ) { return FALSE; }

	/*
	* Lockon Weapon
	*/
	virtual BOOL LockonWeapon( void ) { return FALSE; }

	/*
	* Reload
	*/
	virtual void Reload( void ) { return; };

	/*
	* Get weapon hud position
	*/
	virtual int iPosition( void ) { return 0; }

	
	int		m_iPrimaryMax;	// Max amount of ammo in a "clip" for non-powered weapons
	int		m_iSecondaryMax;// Max amount of ammo in a "clip" for non-powered weapons
	int		m_iPrimaryCur;	// Current amount of primary ammo for non-powered weapons
	int		m_iSecondaryCur;// Current amount of secondary ammo for non-powered weapons
protected:
	/*
	* Set view model
	* @param char *pModel: Model name
	*/
	virtual void SetV_model( char *pModel );

	/*
	* Set player (weapon) model
	* @param char *pModel: Model name
	*/
	virtual void SetP_model( char *pModel );

	/*
	* Set animation prefix
	* @param char *pPrefix: Prefix name
	*/
	virtual void SetAnimPrefix( char *pPrefix );

	/*
	* Set classname
	* @param char *pName: Classname
	*/
	virtual void SetClassname( char *pName );

	/*
	* Set weapon ID
	*/
	virtual void SetID( int iId ) { m_iId = iId; }

	/*
	* Get ID
	*/
	virtual int GetID( void ) { return 0; }

	int		m_iWeight;		// Weapon weight
	char	*m_pV_model;	// Inside model
	char	*m_pP_model;	// Outside model ( the player is holding )
	char	*m_pAnimPrefix;	// Animation prefix for this weapon
};

class CBaseMelee : public CMMWeapon
{
public:	
	/*
	* Spawn
	*/
	virtual void Spawn( );

	/*
	* Deploy weapon
	* @return BOOL: If this weapon was deployed successfully
	*/
	virtual BOOL Deploy( );

	/*
	* Holster
	*/
	virtual void Holster( int skiplocal = 0  );

	/*
	* Start SaberFX
	*/
	virtual void StartSaberFX( void );

	/*
	* Stop SaberFX
	*/
	virtual void StopSaberFX( void );

	/*
	* Primary Attack
	*/
	virtual void PrimaryAttack() { }

	/*
	* Secondary Attack
	*/
	virtual void SecondaryAttack() { }
	/*
	* Weapon Idle
	*/
	virtual void WeaponIdle( void ) { }

	/*
	* Get Weight
	*/
	virtual int GetWeight( void ) { return 100; }
	
	/*
	* Use power for primary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerPrimary( void ) { return TRUE; }

	/*
	* Use power for secondary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerSecondary( void ) { return TRUE; }

	/*
	* Melee Weapon
	*/
	virtual BOOL MeleeWeapon( void ) { return TRUE; }

	/*
	* Item Slot
	*/
	virtual int iItemSlot( void ) { return WP_SLOT_MELEE; }
};

class CBaseRapidFireGun : public CMMWeapon
{
public:
	/*
	* Spawn
	*/
	virtual void Spawn( );
	
	/*
	* Deploy weapon
	* @return BOOL: If this weapon was deployed successfully
	*/
	virtual BOOL Deploy( );

	/*
	* Holster
	*/
	virtual void Holster( int skiplocal = 0  );

	/*
	* Get information about the current weapon
	* @param ItemInfo *p: Item information struct
	* @return int: always return 1 (?)
	*/
	virtual int GetItemInfo( ItemInfo *p );

	/*
	* Primary Attack
	*/
	virtual void PrimaryAttack();

	/*
	* Secondary Attack
	*/
	virtual void SecondaryAttack();

	/*
	* Emit Tracer FX
	* @param int flags: flags for event
	* @param Vector vecDir: Direction fire is going at
	*/
	virtual void EmitPrimaryTracerFX( int flags, Vector vecDir ) {}
	virtual void EmitSecondaryTracerFX( int flags, Vector vecDir ) {}

	/*
	* Shoot Bullet
	* @param Vector vecSrc: Source of bullet
	* @param Vector vecAiming: Direction bullet is going in
	* @param Vector: Direction the blast will head in
	*/
	virtual Vector ShootBullet( Vector vecSrc, Vector vecAiming );
	
	/*
	* Rapid Fire Weapon
	*/
	virtual BOOL RapidFireWeapon( void ) { return TRUE; }
	
	/*
	* Max Rounds
	*/
	virtual int MaxRounds( void ) { return 50; }

	/*
	* Reload Time
	*/
	virtual float ReloadTime( void ) { return 1; }

	/*
	* Default Reload
	* @param iAmount: Amount we're reloading
	* @param float flDelay: Delay until reload is compelte
	*/
	virtual BOOL DefaultReload( float flDelay );

	/*
	* Weapon Idle
	*/
	virtual void WeaponIdle( void );

	//added by jason
	/*******************
	* SetDamage
	* @param iDmg: amount of damage
	* @return void: no return
	*******************/
	virtual void SetDamage( int iDmg ) { m_iDamage = iDmg; }
	
	//added by grOOvy
	/*******************
	* SetBulletType
	* @param iBltType: type of bullet
	* @return void: no return
	*******************/
	virtual void SetBulletType ( int iBltType ) { m_iBulletType = iBltType; }

	/*
	* Item Slot
	*/
	virtual int iItemSlot( void ) { return WP_SLOT_RAPID; }

protected:
	/*
	* Primary/Secondary Fire Rate
	*/
	virtual float PrimaryFireRate( void ) { return 0.1; }
	virtual float SecondaryFireRate( void ) { return 0.1; }

	/*
	* Aiming vector
	*/
	virtual Vector GetAiming( void ) { return m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES ); }	

	/*
	* Reload
	*/
	virtual void Reload( void );

	/*
	* Can Lockon to Missles
	*/
	virtual BOOL CanLockMissles( void ) { return TRUE; }

	/*
	* Max amount of missle locks for this weapon
	* @return int: amout of locks
	*/
	virtual int MaxLocks( void ) { return 16; }

	/*
	* Delay between missle lockons
	* @return float: Delay between lockons
	*/
	virtual float LockDelay( void ) { return 0.1; }

	/*
	* Max Lock Distance
	*/
	virtual int MaxLockDistance( void ) { return 3500; }

	/*
	* In lock range
	* @param Vector vecOrigin: Origin of entity that's in range
	* @param float &flDot: Dot product from test
	* @return BOOL: If it's in range
	*/
	virtual BOOL InLockRange( Vector vecOrigin, float &flDot );

	/*
	* Get Best Lock ( shooting missles down )
	* @return CBaseMissle*: Missle that we're going to shoot down
	*/
	virtual CBaseMissle *GetBestLock( void );

	/*
	* Check Locks: Remove locks for locked-on dead players
	*/
	virtual void CheckLocks( void );

	/*
	* Is Locked
	* @param int iIndex: index of player we're testing
	* @return BOOL: If the player is locked
	*/
	virtual BOOL IsLocked( int iIndex );

	/*
	* Add Lock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void AddLock( int iIndex );

	/*
	* Remove Lock
	* @param bool blAll: If we should remove ALL of the locks for the specified index
	* @param int iIndex: Index of locked enemy
	*/
	virtual void RemoveLock( int iIndex );

	/*
	* Clear Locks
	*/
	virtual void ClearLocks( void );

	/*
	* Clear Client Locks
	*/
	virtual void ClearClientLocks( void );

	/*
	* Add Client Lock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void AddClientLock( int iIndex );
	
	/*
	* Add Client PreLock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void AddClientPreLock( int iIndex );

	/*
	* Remove Client Lock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void RemoveClientLock( int iIndex );

	/*
	* Is attempting to lock onto an entity
	* @return BOOL: If we're attempting a lockon
	*/
	virtual BOOL IsAttemptingLockon( void );

	/*
	* Add attempt lock
	* @param CBaseMissle *pMissle: Missle we're attempting to lockon to
	*/
	virtual void AddAttemptLock( CBaseMissle *pMissle );

	/*
	* Handle attempt lockon
	*/
	virtual BOOL HandleAttemptLockon( void );

	/*
	* Reset attempt lockon
	*/
	virtual void ResetAttemptLockon( void );

	int			m_iCurrentLocks;		// Missles locked
	float		m_flAttemptLockTime;	// Time the attempted lock started
	lockdata_t	m_LockData[MAX_MISSLE_LOCKS];// Lock information
	CBaseMissle *m_pAttemptLock;		// Attempting to lock this missle
	int			m_iShell;				// Precache shell
	int			m_iDamage;				// Damage amount
	// added by grOOvy
	int			m_iBulletType;			// Bullet Type
};

class CBaseChargeGun : public CMMWeapon
{
public:
	/*
	* Spawn
	*/
	virtual void Spawn( );
	
	/*
	* Deploy weapon
	* @return BOOL: If this weapon was deployed successfully
	*/
	virtual BOOL Deploy( );

	/*
	* Holster
	*/
	virtual void Holster( int skiplocal = 0 );

	/*
	* Primary Attack
	*/
	virtual void PrimaryAttack();

	/*
	* Secondary Attack
	*/
	virtual void SecondaryAttack();

	/*
	* Use power for primary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerPrimary( void ) { return TRUE; }

	/*
	* Use power for secondary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerSecondary( void ) { return TRUE; }

	/*
	* Weapon Idle
	*/
	virtual void WeaponIdle( void );

	/*
	* Fire Attack
	* @param float flCharged: Current amount charged
	*/
	virtual void FireAttack( float flCharged ) { }

	/*
	* Charge Weapon
	*/
	virtual BOOL ChargeWeapon ( void ) { return TRUE; }

	/*
	* Item Slot
	*/
	virtual int iItemSlot( void ) { return WP_SLOT_CHARGE; }
protected:
	/*
	* Max Charge
	*/
	virtual float MaxCharge( void ) { return 100; }

	/*
	* Min Charge To Fire
	*/
	virtual float MinCharge( void ) { return 40; }

	/*
	* Charge Rate
	*/
	virtual float ChargeRate( void ) { return 20; }

	/*
	* Use Power Rate
	*/
	virtual float UsePowerRate( void ) { return 50; }

	/*
	* Max Charge
	* @return BOOL: If we're charged at the max
	*/
	virtual BOOL ChargeMax( void ) { return (MaxCharge()==m_flCurCharge ? TRUE : FALSE); }
	
	/*
	* Can Fire ( over min charge )
	*/
	virtual BOOL CanFire( void ) { return (m_flCurCharge > MinCharge() ? TRUE : FALSE); }

	/*
	* Get Percent Charge
	*/
	virtual float GetPercentCharge( void ) { return m_flCurCharge/MaxCharge(); }

	/*
	* Update Client Data
	*/
	virtual void UpdateClient( void );

	/*
	* Add Charge
	* @param float flAmount: Amount of charge we're adding
	*/
	virtual void AddCharge( float flAmount );

	/*
	* Start Charge
	*/
	virtual void StartCharge( void );

	/*
	* Handle Charge Fire: Calls FireAttack function if the player can fire
	*/
	virtual void HandleChargeFire( void );
	
	/*
	* Reset Charge
	*/
	virtual void ResetCharge( void );


	// added by jason
	virtual void StartChargeFunc( void ) {}
	virtual void EndChargeFunc( void ) {}


	BOOL	m_fCharging;	// Currently charging
	float	m_flCurCharge;	// Current charge amount
};

class CBaseLockOn : public CMMWeapon
{
public:
	/*
	* Spawn
	*/
	virtual void Spawn( );

	/*
	* Deploy weapon
	* @return BOOL: If this weapon was deployed successfully
	*/
	virtual BOOL Deploy( );

	/*
	* Holster
	*/
	virtual void Holster( int skiplocal = 0  );

	/*
	* Primary Attack
	*/
	virtual void PrimaryAttack( );

	/*
	* Secondary Attack
	*/
	virtual void SecondaryAttack( );

	
	/*
	* Use power for primary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerPrimary( void ) { return TRUE; }

	/*
	* Use power for secondary fire
	* @return BOOL: If we're using power
	*/
	virtual BOOL UsePowerSecondary( void ) { return TRUE; }

	/*
	* Weapon Idle
	*/
	virtual void WeaponIdle( void );

	/*
	* Lockon Weapon
	*/
	virtual BOOL LockonWeapon( void ) { return TRUE; }

	/*
	* Item Slot
	*/
	virtual int iItemSlot( void ) { return WP_SLOT_LOCKON; }
protected:
	/*
	* Called to fire attack
	* @param int iIndex: Entity we're firing at
	*/
	virtual void FireAttack(int iIndex) {};

	/*
	* Multi-lock ability ( ability to lock onto more then one entity )
	*/
	virtual BOOL MultiLock( void ) { return TRUE; }

	/*
	* Max amount of locks for this weapon
	* @return int: amout of locks
	*/
	virtual int MaxLocks( void ) { return 8; }

	/*
	* Power Cost
	* @return int: Power cost
	*/
	virtual int PowerCost( void ) { return 10; }

	/*
	* Delay between lockons
	* @return float: Delay between lockons
	*/
	virtual float LockDelay( void ) { return 0.5; }

	/*
	* Delay between fires
	* @return float: Delay between fires
	*/
	virtual float FireDelay( void ) { return 0.5; }

	/*
	* Max Lock Distance
	*/
	virtual int MaxLockDistance( void ) { return 3500; }

	/*
	* Fire Sound
	*/
	virtual string Firesound( void ) { return string("player/missle_fire.wav"); }

	/*
	* Get Amount Locked
	*/
	virtual int GetAmtLocked( void );

	/*
	* In lock range
	* @param Vector vecOrigin: Origin of entity that's in range
	* @param float &flDot: Dot product from test
	* @return BOOL: If it's in range
	*/
	virtual BOOL InLockRange( Vector vecOrigin, float &flDot );

	/*
	* Check Locks: Remove locks for locked-on dead players
	*/
	virtual void CheckLocks( void );

	/*
	* Can Lock
	* @param CBasePlayer *pLock: Entity we're locking onto
	* @return BOOL: If we can start or continue to lock onto entity
	*/
	virtual BOOL CanLock( CBasePlayer *pLock );

	/*
	* Is Locked
	* @param int iIndex: index of player we're testing
	* @param int &iAmount: Amount of locks this index has
	* @return BOOL: If the player is locked
	*/
	virtual BOOL IsLocked( int iIndex, int &iAmount );

	/*
	* Add Lock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void AddLock( int iIndex );

	/*
	* Remove Lock
	* @param bool blAll: If we should remove ALL of the locks for the specified index
	* @param int iIndex: Index of locked enemy
	*/
	virtual void RemoveLock( bool blAll, int iIndex );

	/*
	* Clear Locks
	*/
	virtual void ClearLocks( void );

	/*
	* Clear Client Locks
	*/
	virtual void ClearClientLocks( void );

	/*
	* Add Client Lock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void AddClientLock( int iIndex );
	
	/*
	* Add Client PreLock
	* @param int iIndex: Index of locked enemy
	*/
	virtual void AddClientPreLock( int iIndex );

	/*
	* Remove Client Lock
	* @param int iIndex: Index of locked enemy
	* @param bool blAll: Remove all locks for specified target
	*/
	virtual void RemoveClientLock( bool blAll, int iIndex );

	/*
	* Is attempting to lock onto an entity
	* @return BOOL: If we're attempting a lockon
	*/
	virtual BOOL IsAttemptingLockon( void );

	/*
	* Add attempt lock
	* @param CBasePlayer *pPlayer: Player we're attempting to lockon to
	*/
	virtual void AddAttemptLock( CBasePlayer *pPlayer );

	/*
	* Handle attempt lockon
	*/
	virtual BOOL HandleAttemptLockon( void );

	/*
	* Reset attempt lockon
	*/
	virtual void ResetAttemptLockon( void );

	int			m_iCurrentLocks;		// Players locked
	float		m_flAttemptLockTime;	// Time the attempted lock started
	lockdata_t	m_LockData[MAX_LOCKS];	// Lock information
	CBasePlayer *m_pAttemptLock;		// Attempting to lock this player ( index )
};

/*
* -------------------
* Custom Weapons
* -------------------
*/

/*
* ==============
* VF-1J Valkyrie
* ==============
*/

class CVf1j_Saber : public CBaseMelee
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_VF1JSABER; }
private:
	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_VF1J_SABER; }
};

/*
* VF1J: Rapid Fire
*/
class CVf1j_RapidFire : public CBaseRapidFireGun
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Emit Tracer FX
	* @param int flags: flags for event
	* @param Vector vecDir: Direction fire is going at
	*/
	void EmitPrimaryTracerFX( int flags, Vector vecDir );

	/*
	 * Rates of fire
	 * Custom rates of fire for VF-1J's Rapid Fire attack
	 * // grOOvy
	 */
	float PrimaryFireRate( void ) { return 0.1; }
	float SecondaryFireRate( void ) { return 0.1; }

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_VF1JRAPID; }

private:
	/*
	* Max Rounds
	*/
	int MaxRounds( void ) { return 50; }

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_VF1J_RAPID; }

	unsigned short m_event;
};

/*
* VF1J: Alphabomb
*/
class CVf1j_AlphaBomb : public CBaseChargeGun
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Fire Attack
	* @param float flCharged: Current amount charged
	*/
	void FireAttack( float flCharged );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_VF1JALPHA; }

	// added by jason
	void StartChargeFunc( void );
	void EndChargeFunc( void );
	CVf1j_AlphaBomb( void ) { m_pCharge = NULL; }
private:

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_VF1J_ALPHA; }

	// added by jason
	CSprite *m_pCharge;
};

/*
* VF1J: Homing Missles
*/
class CVf1j_HomingMissles : public CBaseLockOn
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );
	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_VF1JHOMING; }
private:
	/*
	* Power Cost
	* @return int: Power cost
	*/
	int PowerCost( void ) { return VF1J_MISSLE_COST; }

	/*
	* Delay between fires
	* @return float: Delay between blast
	*/
	float FireDelay( void ) { return 0; }

	/*
	* Called to fire attack
	* @param int iIndex: Entity we're firing at
	*/
	void FireAttack(int iIndex);

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_VF1J_HOMING; }
};


/*
* ==============
* AF-49 Silver Axe
* ==============
*/

// added by Pcjoe
class CAf49_TwinBlade : public CBaseMelee
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_AF49TWINBLADE; }

	/*
	* Start SaberFX
	*/
	void StartSaberFX( void ) {}

	/*
	* Stop SaberFX
	*/
	void StopSaberFX( void ) {}
private:
	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_AF49_TWINBLADE; }
};

// added by jason
class CAf49_RapidFire : public CBaseRapidFireGun
{
public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			CustomSpawn( void );
	/*******************
	* virtual	EmitPrimaryTracerFX
	* @param flags: tracer flags
	* @param vecDir: Direction we are firing
	* @return void: no return
	*******************/
	void	EmitPrimaryTracerFX( int flags, Vector vecDir );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_AF49RAPID; }

	/*
	 * Custom rates of fire for AF-49's rapid fire attack
	 * // grOOvy
	 */
	float PrimaryFireRate( void ) { return 0.15; }
	float SecondaryFireRate( void ) { return 0.15; }

private:
	/*******************
	* Max Rounds
	* @purpose: Gets the max ammo for weapon
	* @param void: no param
	* @return int: max ammo
	*******************/
	int				MaxRounds( void ) { return 75; }
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int				GetID( void ) { return WEAPON_AF49_RAPIDFIRE; }

	// Event 
	unsigned short m_event;
};

// same as joes vfj homing missles
class CAf49_HomingMissles : public CBaseLockOn
{
public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void	Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void	CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_AF49HOMING; }
private:
	/*******************
	* Max Locks
	* @purpose: Tells how many players we can lock onto
	* @param void: no param
	* @return int: max locks for weapon
	*******************/
	int		MaxLocks( void ) { return 5; }
	/*******************
	* virtual	PowerCost
	* @purpose: Tells how much power a missle costs
	* @param void: no param
	* @return int: amount of power missles costs
	*******************/
	int		PowerCost( void ) { return AF49_MISSLE_COST; }
	/*******************
	* virtual	FireDelay
	* @purpose: Tells how much delay between missles firing
	* @param void: no param
	* @return float: amount of delay between shots
	*******************/
	float	FireDelay( void ) { return 0; }
	/*******************
	* virtual	FireAttack
	* @purpose: Tells the weapon what to do
	* @param iIndex: Index to the entity we are firing at
	* @return void: No return
	*******************/
	void	FireAttack( int iIndex );
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int		GetID( void ) { return WEAPON_AF49_HOMING; }
};

class CAf49_WastedMissle : public CBaseLockOn
{
public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_AF49WASTED; }
private:
	/*******************
	* Max Locks
	* @purpose: Tells how many players we can lock onto
	* @param void: no param
	* @return int: max locks for weapon
	*******************/
	int		MaxLocks( void ) { return 1; }
	/*
	* Delay between lockons
	* @return float: Delay between lockons
	*/
	float LockDelay( void ) { return 3.0; }
	/*******************
	* virtual	PowerCost
	* @purpose: Tells how much power a missle costs
	* @param void: no param
	* @return int: amount of power missles costs
	*******************/
	int		PowerCost( void ) { return AF49_WMISSLE_COST; }
	/*******************
	* virtual	FireDelay
	* @purpose: Tells how much delay between missles firing
	* @param void: no param
	* @return float: amount of delay between shots
	*******************/
	float	FireDelay( void ) { return 0; }
	/*******************
	* virtual	FireAttack
	* @purpose: Tells the weapon what to do
	* @param iIndex: Index to the entity we are firing at
	* @return void: No return
	*******************/
	void	FireAttack( int iIndex );
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int				GetID( void ) { return WEAPON_AF49_WASTED; }
};

/*
* =======
* Freedom
* =======
*/
class CFreedom_Saber : public CBaseMelee
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_FREEDOMSABER; }
private:
	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_FREEDOM_SABER; }
};

/*
* Freedom: Buster Small
*/
class CFreedom_BusterSmall : public CBaseRapidFireGun
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Emit Tracer FX
	* @param int flags: flags for event
	* @param Vector vecDir: Direction fire is going at
	*/
	void EmitPrimaryTracerFX( int flags, Vector vecDir );

	/*
	* Custom rates of fire for this attack, grOOvy:
	*/
	virtual float PrimaryFireRate( void ) { return 0.3; }
	virtual float SecondaryFireRate( void ) { return 0.3; }
	
	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_FREEDOMRAPID; }

private:
	/*
	* Max Rounds
	*/
	int MaxRounds( void ) { return 20; }

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_FREEDOM_RAPID; }

	unsigned short m_event;
};

/*
* Freedom Buster Big Charged Cannon
* By grOOvy
*/
class CFreedom_BusterBig : public CBaseChargeGun
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Fire Attack
	* @param float flCharged: Current amount charged
	*/
	void FireAttack( float flCharged );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_FREEDOMBIG; }

	void StartChargeFunc( void );
	void EndChargeFunc( void );
	
	CFreedom_BusterBig( void ) { m_pCharge = NULL; }

	void AddEffects( CBaseEntity *pEnt );
// added 17nov04

	/*
	* Power Cost
	* @return int: Power cost
	*/
	int PowerCost( void ) { return FREEDOM_BUSTERBIG_COST; }


private:

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_FREEDOM_BIG; }

	CSprite *m_pCharge;
};



/*
* Pcjoe: Freedom 5 way attack
*/
class CFreedomCannons
{
public:
	/*
	* Constructor
	* @param CBasePlayer *pOwner: Owner of this cannon
	*/
	CFreedomCannons(CBasePlayer *pOwner);

	/*
	* Set Stats
	* @param std::string strSprite: Sprite the beam will use
	* @param int iSize: Size of beam
	* @param int iDamage: Damage beam will inflict
	* @param Vector m_vecOffset: Vector offset from player to beam cannon head
	*/
	void SetStats( std::string &strSprite, int iSize, int iDamage, Vector vecOffset );

	/*
	* Add special effects ( trails, head gfx )
	* @param CFreedomBlast *pBlast: Blast we're adding effects to
	*/
	void AddEffects( CFreedomBlast *pBlast );

	/*
	* Fire Attack
	* @param CBasePlayer *pPlayer: Player that's firing the attack
	*/
	void Fire( CBasePlayer *pPlayer );

private:
	CBasePlayer *m_pOwner;	// Owner
	std::string m_strSprite;// Sprite
	int m_iSize;		// Size
	int m_iDamage;		// Damage inflicted
	float m_flFadeSpeed;// Trail fade speed
	Vector m_vecOffset;	// Offset cannon is from main player
};

class CFreedom_5Way : public CBaseLockOn
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Deploy weapon
	* @return BOOL: If this weapon was deployed successfully
	*/
	BOOL Deploy( );

	/*
	* Can Holster
	*/
	BOOL CanHolster( void );

	/*
	* Holster
	*/
	void Holster( int skiplocal = 0  );

	/*
	* Primary Attack
	*/
	void PrimaryAttack();

	/*
	* Secondary Attack
	*/
	void SecondaryAttack();

	/*
	* Weapon Idle
	*/
	void WeaponIdle( void );

	/*
	* Item Slot
	*/
	int iItemSlot( void ) { return WP_SLOT_LOCKON; }

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_FREEDOM5WAY; }
private:
	/*
	* Power Cost
	* @return int: Power cost
	*/
	int PowerCost( void ) { return FREEDOM_5WAY_COST; }

	/*
	* Max amount of locks for this weapon
	* @return int: amout of locks
	*/
	virtual int MaxLocks( void ) { return 5; }

	/*
	* Delay between fires
	* @return float: Delay between blast
	*/
	float FireDelay( void ) { return 0; }

	/*
	* Fire Sound
	*/
	string Firesound( void ) { return string("weapons/5way_fire.wav"); }

	/*
	* Called to fire attack
	* @param int iIndex: Entity we're firing at
	*/
	void FireAttack(int iIndex);

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_FREEDOM_5WAY; }

	/*
	* In initial lock range
	* @param Vector vecOrigin: Origin of entity that's in range
	* @param float &flDot: Dot product from test
	* @return BOOL: If it's in range
	*/
	virtual BOOL InInitLockRange( Vector vecOrigin, float &flDot );

	/*
	* Delete prelock
	*/
	virtual void DeletePrelock( void );

	// Charged weapon code
	/*
	* Max Charge
	*/
	float MaxCharge( void ) { return 100; }

	/*
	* Min Charge To Fire
	*/
	float MinCharge( void ) { return 20; }

	/*
	* Charge Rate
	*/
	float ChargeRate( void ) { return 20; }

	/*
	* Max Charge
	* @return BOOL: If we're charged at the max
	*/
	BOOL ChargeMax( void ) { return (MaxCharge()==m_flCurCharge ? TRUE : FALSE); }
	
	/*
	* Get Percent Charge
	*/
	float GetPercentCharge( void ) { return m_flCurCharge/MaxCharge(); }

	/*
	* Can Fire
	*/
	BOOL CanFire( void );
	
	/*
	* Handle Charge
	* @return BOOL: If we're currently charging
	*/
	BOOL HandleCharge( void );

	/*
	* Update Client Data
	*/
	void UpdateClient( void );

	/*
	* Add Charge
	* @param float flAmount: Amount of charge we're adding
	*/
	void AddCharge( float flAmount );

	/*
	* Start Charge
	*/
	void StartCharge( void );

	/*
	* Handle Charge Fire: Calls FireAttack function if the player can fire
	*/
	void HandleChargeFire( void );
	
	/*
	* Reset Charge
	*/
	void ResetCharge( void );

	BOOL	m_fCharging;	// Currently charging
	float	m_flCurCharge;	// Current charge amount
	int		m_iFireNum;		// Blast fire number
	std::vector<CFreedomCannons>	m_FreedomBlast;	// Freedom blast(s)
};

/*
* ===========
* MK2 Monster
* ===========
*/

/*
* MK2: Rapid Fire
*/
class CMK2_RapidFire : public CBaseRapidFireGun
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Emit Tracer FX
	* @param int flags: flags for event
	* @param Vector vecDir: Direction fire is going at
	*/
	void EmitPrimaryTracerFX( int flags, Vector vecDir );

	/*
	 * Rates of fire
	 * Custom rates of fire for VF-1J's Rapid Fire attack
	 * // grOOvy
	 */
	float PrimaryFireRate( void ) { return 0.15; }
	float SecondaryFireRate( void ) { return 0.15; }

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_MK2RAPID; }

private:
	/*
	* Max Rounds
	*/
	int MaxRounds( void ) { return 75; }

	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_MK2_RAPIDFIRE; }

	unsigned short m_event;
};

/*
* MK2 Homing Missiles Attack
* by grOOvy
*/

class CMK2_HomingMissles : public CBaseLockOn
{
public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void	Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void	CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_MK2HOMING; }
private:
	/*******************
	* Max Locks
	* @purpose: Tells how many players we can lock onto
	* @param void: no param
	* @return int: max locks for weapon
	*******************/
	int		MaxLocks( void ) { return 5; }
	/*******************
	* virtual	PowerCost
	* @purpose: Tells how much power a missle costs
	* @param void: no param
	* @return int: amount of power missles costs
	*******************/
	int		PowerCost( void ) { return AF49_MISSLE_COST; } 
	// leaving this to same as AF-49's power cost
	/*******************
	* virtual	FireDelay
	* @purpose: Tells how much delay between missles firing
	* @param void: no param
	* @return float: amount of delay between shots
	*******************/
	float	FireDelay( void ) { return 0; }
	/*******************
	* virtual	FireAttack
	* @purpose: Tells the weapon what to do
	* @param iIndex: Index to the entity we are firing at
	* @return void: No return
	*******************/
	void	FireAttack( int iIndex );
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int		GetID( void ) { return WEAPON_MK2_HOMING; }
};


/*
* MK2 Rocket Barrels Attack
* by grOOvy
*/

class CMK2_RocketBarrels : public CBaseLockOn
{
public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void	Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void	CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_MK2BARRELS; }
private:
	/*******************
	* Max Locks
	* @purpose: Tells how many players we can lock onto
	* @param void: no param
	* @return int: max locks for weapon
	*******************/
	int		MaxLocks( void ) { return 1; }
	// We lockon only once, but fire 4 rockets.
	/*******************
	* virtual	PowerCost
	* @purpose: Tells how much power a missle costs
	* @param void: no param
	* @return int: amount of power missles costs
	*******************/
	int		PowerCost( void ) { return MK2_BARRELS_COST; } 
	/*******************
	* virtual	FireDelay
	* @purpose: Tells how much delay between missles firing
	* @param void: no param
	* @return float: amount of delay between shots
	*******************/
	float	FireDelay( void ) { return 0; }
	/*******************
	* virtual	FireAttack
	* @purpose: Tells the weapon what to do
	* @param iIndex: Index to the entity we are firing at
	* @return void: No return
	*******************/
	void	FireAttack( int iIndex );
	/*
	* Fire Sound
	*/
	virtual string Firesound( void ) { return string("weapons/barrels_fire.wav"); }
	
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int		GetID( void ) { return WEAPON_MK2_BARRELS; }
};

/*
* MK-2 Monster's Nuke Missile Attack
* By grOOvy
*/
class CMK2_NukeMissile : public CBaseChargeGun
{
public:
	/*
	* Precache
	*/
	void Precache( );

	/*
	* Custom Spawn
	*/
	void CustomSpawn( void );

	/*
	* Fire Attack
	* @param float flCharged: Current amount charged
	*/
	void FireAttack( float flCharged );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_MK2NUKE; }

	void StartChargeFunc( void );
	void EndChargeFunc( void );
	
	CMK2_NukeMissile( void ) { m_pCharge = NULL; }
private:

	/*
	* Max Charge
	*/
	virtual float MaxCharge( void ) { return 100; }

	/*
	* Min Charge To Fire
	*/
	// modified by jason to make it work
	virtual float MinCharge( void ) { return 99; }

	/*
	* Charge Rate
	*/
	virtual float ChargeRate( void ) { return 20; }

	/*
	* Use Power Rate
	*/
	virtual float UsePowerRate( void ) { return 110; }


	/*
	* Get ID
	*/
	int GetID( void ) { return WEAPON_MK2_NUKE; }

	CSprite *m_pCharge;
};




/*
* ===========
* Deathscythe
* ===========
*/

class CDS_Scythe : public CBaseMelee
{
public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void		Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			CustomSpawn( void );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_DSCYTHE; }
private:
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int				GetID( void ) { return WEAPON_DS_SCYTHE; }
};



/* 
  * Shield Gun Attack
  * by grOOvy
*/

class CDS_ShieldGun : public CBaseRapidFireGun
{

public:
	/*******************
	* Precache
	* @purpose: loads media files for weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			Precache( void );
	/*******************
	* Custom Spawn
	* @purpose: endables the weapon
	* @param void: no param
	* @return void: no return
	*******************/
	void			CustomSpawn( void );
	/*******************
	* virtual	EmitPrimaryTracerFX
	* @param flags: tracer flags
	* @param vecDir: Direction we are firing
	* @return void: no return
	*******************/
	virtual void	EmitPrimaryTracerFX( int flags, Vector vecDir );

	/*
	* Get weapon hud position
	*/
	int iPosition( void ) { return WP_POS_DSSHIELDGUN; }

	/*
	* Custom rates of fire for this shield gun attack:
	*/
	virtual float PrimaryFireRate( void ) { return 0.15; }
	virtual float SecondaryFireRate( void ) { return 0.15; }

private:
	/*******************
	* Max Rounds
	* @purpose: Gets the max ammo for weapon
	* @param void: no param
	* @return int: max ammo
	*******************/
	int				MaxRounds( void ) { return 20; }
	/*******************
	* Get ID
	* @purpose: Gets weapons id
	* @param void: no param
	* @return int: weapon id
	*******************/
	int				GetID( void ) { return WEAPON_DS_SHIELDGUN; }

	// Event 
	unsigned short m_event;
};
#endif