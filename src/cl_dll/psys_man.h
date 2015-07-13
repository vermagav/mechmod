/**********************************************************
*			PARTICLE SYSTEM MANAGER
***********************************************************
*	Purpose: Particle system manager keeps track of all the 
*			running systems
*	Created On: 1/5/04
*	Last Edited: 1/5/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

class CPartSysManager
{
public:
	CPartSysManager( void );				//constructor
	~CPartSysManager( void );				//destructor

	/***************************
	*	Add System
	* @purpose: Adds a Particle System to the list 
	* @param CParticleSystem *pSys: pointer to the system were adding
	* @return void: no param
	***************************/
	void				AddSystem( CParticleSystem *pSys );
	/***************************
	*	Update Systems
	* @purpose: Updates all of these particle systems
	* @param void: no param
	* @return void: no param
	***************************/
	void				UpdateSystems( void );
	/***************************
	*	Remove
	* @purpose: Removes a single system from the list
	* @param CParticleSystem *pSys: system we want to remove
	* @return CParticleSystem *: returns the next system in the list
	***************************/
	CParticleSystem		*Remove( CParticleSystem *pSys );
	/***************************
	*	Remove All
	* @purpose: Removes all systems
	* @param void: no param
	* @return void: no param
	***************************/
	void				RemoveAll( void );
	/***************************
	*	Get Manager
	* @purpose: Gets the partsysmanager so we can use it
	* @param void: no param
	* @return CPartSysManager *: The current particle system manager
	***************************/
	static CPartSysManager		*GetManager( void );
	/***************************
	*	Find
	* @purpose: Finds a System
	* @param int iSystem: system we are looking for
	* @param int iIndex: with the index we are looking for
	* @return CParticleSystem *: particle system we were looking for
	***************************/
	CParticleSystem				*Find( int iSystem, int iIndex );
private:
	CParticleSystem		*m_pHeadSystem;		//for linked list of our particle systems
};

extern CPartSysManager *GetManager( void );