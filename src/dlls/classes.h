/*
* Class system
* @version: January 7th, 2004
* @Author: Joseph Florencio
*/

#ifndef CLASSES_H
#define CLASSES_H

// Defines for characters
#define PC_NONE			0
#define PC_VF1J			1
#define PC_AF49			2
#define PC_DEATHSCYTHE	3
#define PC_MK2			4
#define PC_FREEDOM		5
#define PC_RANDOM		6

// Boost colors
#define	BOOST_BLUE		1
#define	BOOST_RED		2
#define	BOOST_PURPLE	3

/*
* Base class system
*/
class CBasePlayerClass
{
public:
	/*
	* Set Owner
	* @param CBasePlayer *pOwner: Owner we're setting
	*/
	virtual void SetOwner( CBasePlayer *pOwner ) { m_pOwner = pOwner; }

	/*
	* Max health
	* @Return int: Max Health
	*/
	virtual int MaxHealth() { return 100; }
	
	/*
	* Max Armor
	*/
	virtual int MaxArmor() { return 100; }

	/*
	* Max Speed
	* @Return int: Max Speed
	*/
	virtual int MaxSpeed() { return 400; }

	/*
	* Fly Multiplier Speeds
	* @return float: Fly Speeds
	*/
	virtual float FlyNormal() { return 1.2; }
	virtual float FlyFast() { return 1.75; }
	virtual float FlyBoost() { return 2.25; }

	/*
	* Fuel Efficiency
	* @return float: Fuel Efficiency
	*/
	virtual float FuelEfficiency() { return 1; }

	/*
	* Class ID
	* @Return int: Class ID
	*/
	virtual int ClassID() { return 0; }
	
	/*
	* Give weapons
	*/
	virtual void GiveWeapons( void ) {}

	/*
	* Add saber styles
	*/
	virtual void AddSaberStyles( void ) {}

	/*
	* Model Name
	* return char*: model name
	*/
	virtual char *GetModelName() { return "vf1j"; }
	
	/*
	* Set Playermodel
	*/
	virtual void SetModel( void );

	/*
	* Set Health
	*/
	virtual void SetHealth( void );

	/*
	* Set Armor //Rick
	*/
	virtual void SetArmor( void );

	/*
	* Send Max Health/Armor
	*/
	virtual void SendMaxHealth( void );

	/*
	* Boost Color
	*/
	virtual int BoostColor( void ) { return 0; }

protected:
	CBasePlayer *m_pOwner;
};

/*
* VF-1J Valkyrie
*/
class CVf1j : public CBasePlayerClass
{
public:
	/*
	* Max health
	* @Return int: Max Health
	*/
	virtual int MaxHealth() { return 100; }

	/*
	* Max Armor
	*/
	virtual int MaxArmor() { return 100; }

	/*
	* Max Speed
	* @Return int: Max Speed
	*/
	virtual int MaxSpeed() { return 250; }

	/*
	* Fuel Efficiency
	* @return float: Fuel Efficiency
	*/
	virtual float FuelEfficiency() { return 0.75; }

	/*
	* Class ID
	* @Return int: Class ID
	*/
	virtual int ClassID() { return PC_VF1J; }

	/*
	* Give weapons
	*/
	virtual void GiveWeapons( void );

	/*
	* Add saber styles
	*/
	virtual void AddSaberStyles( void );

	/*
	* Model Name
	* return char*: model name
	*/
	virtual char *GetModelName() { return "vf1j"; }

	/*
	* Boost Color
	*/
	virtual int BoostColor( void ) { return BOOST_PURPLE; }
};

/*
* AF-49 Silver Axe
*/
class CAf49 : public CBasePlayerClass
{
public:
	/*
	* Max health
	* @Return int: Max Health
	*/
	virtual int MaxHealth() { return 100; }

	/*
	* Max Armor
	*/
	virtual int MaxArmor() { return 250; }

	/*
	* Max Speed
	* @Return int: Max Speed
	*/
	virtual int MaxSpeed() { return 150; }

	/*
	* Class ID
	* @Return int: Class ID
	*/
	virtual int ClassID() { return PC_AF49; }

	/*
	* Give weapons
	*/
	virtual void GiveWeapons( void );
	/*
	* Add saber styles
	*/
	virtual void AddSaberStyles( void );

	/*
	* Model Name
	* return char*: model name
	*/
	virtual char *GetModelName() { return "silveraxe"; }

	/*
	* Boost Color
	*/
	virtual int BoostColor( void ) { return BOOST_RED; }
};

/*
* Freedom
*/
class CFreedom : public CBasePlayerClass
{
public:
	/*
	* Max health
	* @Return int: Max Health
	*/
	virtual int MaxHealth() { return 100; }

	/*
	* Max Armor
	*/
	virtual int MaxArmor() { return 200; }

	/*
	* Max Speed
	* @Return int: Max Speed
	*/
	virtual int MaxSpeed() { return 220; }

	/*
	* Class ID
	* @Return int: Class ID
	*/
	virtual int ClassID() { return PC_FREEDOM; }

	/*
	* Give weapons
	*/
	virtual void GiveWeapons( void );

	/*
	* Add saber styles
	*/
	virtual void AddSaberStyles( void );

	/*
	* Model Name
	* return char*: model name
	*/
	virtual char *GetModelName() { return "freedom"; }

	/*
	* Boost Color
	*/
	virtual int BoostColor( void ) { return BOOST_BLUE; }
};

/*
* MK2 Monster
*/
class CMK2 : public CBasePlayerClass
{
public:
	/*
	* Max health
	* @Return int: Max Health
	*/
	virtual int MaxHealth() { return 100; }

	/*
	* Max Armor
	*/
	virtual int MaxArmor() { return 400; }

	/*
	* Max Speed
	* @Return int: Max Speed
	*/
	virtual int MaxSpeed() { return 130; }

	/*
	* Class ID
	* @Return int: Class ID
	*/
	virtual int ClassID() { return PC_MK2; }

	/*
	* Give weapons
	*/
	virtual void GiveWeapons( void );

	/*
	* Add saber styles
	*/
	virtual void AddSaberStyles( void );

	/*
	* Model Name
	* return char*: model name
	*/
	virtual char *GetModelName() { return "monster"; }

	/*
	* Boost Color
	*/
	virtual int BoostColor( void ) { return BOOST_BLUE; }

	/*
	* Fuel Efficiency
	* @return float: Fuel Efficiency
	*/
	virtual float FuelEfficiency() { return 1.6; }
};

/*
* Deathscythe
*/
class CDeathscythe : public CBasePlayerClass
{
public:
	/*
	* Max health
	* @Return int: Max Health
	*/
	virtual int MaxHealth() { return 100; }

	/*
	* Max Armor
	*/
	virtual int MaxArmor() { return 200; }

	/*
	* Max Speed
	* @Return int: Max Speed
	*/
	virtual int MaxSpeed() { return 170; }

	/*
	* Class ID
	* @Return int: Class ID
	*/
	virtual int ClassID() { return PC_DEATHSCYTHE; }

	/*
	* Give weapons
	*/
	virtual void GiveWeapons( void );

	/*
	* Add saber styles
	*/
	virtual void AddSaberStyles( void );

	/*
	* Model Name
	* return char*: model name
	*/
	virtual char *GetModelName() { return "deathscythe"; }

	/*
	* Boost Color
	*/
	virtual int BoostColor( void ) { return BOOST_BLUE; }
};
#endif