/**********************************************************
*			PARTICLE SYSTEM FRAMEWORK
***********************************************************
*	Purpose: Base Particle Info for inherited classes
*	Created On: 1/4/04
*	Last Edited: 1/6/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

class CParticleSystem
{
public:
	CParticleSystem( void );			// constructor
	~CParticleSystem( void );			// destructor
	/*****************
	*	Render System
	* @purpose: Renders the whole current particle system
	* @param void: no param
	* @return bool: Lets us know if the system was renderd or not
	*****************/
	bool			RenderSystem( void );
	/*****************
	*	Pre Render
	* @purpose: Do somethign once before we reneder
	* @param void: no param
	* @return void: no param
	*****************/
	virtual void	PreRender( void );
	/*****************
	*	Post Render
	* @purpose: Do somethign once after we reneder
	* @param void: no param
	* @return void: no param
	*****************/
	virtual void	PostRender( void );
	/*****************
	*	Test System
	* @purpose: Tests the system to see if its worth rendering
	* @param void: no param
	* @return bool: Tells us if its ok to render the system
	*****************/
	virtual	bool	TestSystem( void ) = 0;
	/*****************
	*	Update System
	* @purpose: Updates all the crucial funcs of the system like adding new parts
	* @param void: no param
	* @return void: no param
	*****************/
	virtual	void	UpdateSystem( void ) = 0;
	/*****************
	*	Test Part
	* @purpose: Test to see if particle is in working order
	* @param Particle_s *: Pointer to particle being tested
	* @return bool: If particle is worth being updated and drawn
	*****************/
	virtual bool	TestPart( Particle_s * ) = 0;
	/*****************
	*	Update Part
	* @purpose: Per Frame update of particle functions
	* @param Particle_s *: Pointer to particle being updated
	* @return void: no param
	*****************/
	virtual void	UpdatePart( Particle_s * ) = 0;
	/*****************
	*	Render Part
	* @purpose: Render the particle to make it visable
	* @param Particle_s *: Pointer to particle being renderd
	* @return void: no param
	*****************/
	virtual	void	RenderPart( Particle_s * ) = 0;
	/*****************
	*	Add Part
	* @purpose: Adds a particle to the particle system
	* @param void: no param
	* @return Particle_s: Returns the particle we jsut added
	*****************/
	Particle_s 		*AddPart( void );
	/*****************
	*	Test Visability
	* @purpose: Checks weather the particle is visable
	* @param Particle_s *pPart: Particle we are testing visability of
	* @return bool: Tells us if the particle is visable or not
	*****************/
	bool			TestVisability( Particle_s *pPart );
	/*****************
	*	Return All Parts
	* @purpose: Returns all particles to the pool
	* @param void: no param
	* @return void: no param
	*****************/
	void			ReturnAllParts( void );
	/*****************
	*	Get Head
	* @purpose: returns the head particle in liked list
	* @param void: no param
	* @return Particle_s *: Head Particle in linked list
	*****************/
	Particle_s		*GetHead( void ) { return m_pHeadPart; };
	
	CParticleSystem	*m_pNext;			// next particle system in linked list
	CParticleSystem	*m_pPrev;			// previous particle system in linked list


	int				m_iIndex;			// index of the ent system is attached to
	int				m_iSystem;			// which sysstem is this
	int				m_iPartsPerSec;		// Spawn n parts per second
	int				m_iAngLock;			// angle locking
	float			m_flLife;			// systems life
	float			m_flStart;			// time system was started
	float			m_flDelay;			// delay till system starts
	float			m_flPartDelay;		// delay till next particle spawned
	float			m_flLastPart;		// time last particle was spawned
	float			m_flPrevTime;		// previous frame time
	float			m_flCurTime;		// current frame time
	float			m_flDistTime;		// distance between the frames
	bool			m_blSpawn;			// tells us if we should spawn a particle
	bool			m_blAlive;			// is the system alive

	vec3_t m_vAng, m_vRight, m_vUp;		// usefull vectors for once per frame operations
	// removed by jason changing to new texture system
	//SprTex_s		*m_pTex;			// texutre for quick loading
	Texture_s		*m_pTex;
private:
	/*****************
	*	Return To Pool
	* @purpose: Moves the dead particle back to the free particle pool
	* @param Particle_s *pPart: Particle we dont need
	* @return void: no param
	*****************/
	void			ReturnToPool( Particle_s *pPart );

	Particle_s		*m_pHeadPart;		// head particle in linked list
};