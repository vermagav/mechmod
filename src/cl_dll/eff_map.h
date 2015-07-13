/**********************************************************
*			EFFECTS EXTENDED TO MAPPER
***********************************************************
*	Purpose: Point and brush based system for the mapper to 
*			use
*	Created On: 1/9/04
*	Last Edited: 1/9/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

class CEffBrush : public CParticleSystem 
{
public:
	/*****************
	*	Constructor
	* @purpose: intializes our vars setup a basic particle template
	* @param vec3_t vOrg: Origin
	* @param vec3_t vMaxVel: Max velocity
	* @param vec3_t vMinVel: Min velocity
	* @param vec3_t vPMaxVel: Max per frame velocity
	* @param vec3_t vPMinVel: Min per frame velocity
	* @param vec3_t vAng: Angle
	* @param colorRGBA clEdColor: end color for color blending
	* @param colorRGBA clStColor: start color for color vlending
	* @param float flBncScl: how much to bounce by
	* @param float flGrav: How much gravity
	* @param float flLife: How long it lives
	* @param float flFR: sprite framerate
	* @param float flStSize: start size for bleding
	* @param float flEdSize: end size for blending
	* @param float flSysLife: sytem life
	* @param float flDelay: system delay to start
	* @param float flPartDelay: delay till next particle
	* @param int iBounces: number of bounces
	* @param int iFlags: special flags for system
	* @param int iIndex: Entity attached to's index
	* @param int iPPS: Particles spawned per second
	* @param int iAngLock: Specifed angle locking
	* @param char *pTex: Name of the texture
	* @return bool: Tells us if its ok to render the system
	*****************/
	CEffBrush( vec3_t vOrg, vec3_t vMaxVel, vec3_t vMinVel, vec3_t vPMaxVel, vec3_t vPMinVel,
				vec3_t vAng, colorRGBA clEdCol, colorRGBA clStCol, float flBncScl, float flGrav,
				float flLife, float flFR, float flStSize, float flEdSize, float flSysLife, float flDelay,
				float flPartDelay, int iBounces, int iFlags, int iIndex, int iPPS, int iAngLock, char *pTex );		// constructor
	/*****************
	*	Test System
	* @purpose: Tests the system to see if its worth rendering
	* @param void: no param
	* @return bool: Tells us if its ok to render the system
	*****************/
	virtual	bool	TestSystem( void );
	/*****************
	*	Update System
	* @purpose: Updates all the crucial funcs of the system like adding new parts
	* @param void: no param
	* @return void: no param
	*****************/
	virtual	void	UpdateSystem( void );
	/*****************
	*	Test Part
	* @purpose: Test to see if particle is in working order
	* @param Particle_s *: Pointer to particle being tested
	* @return bool: If particle is worth being updated and drawn
	*****************/
	virtual bool	TestPart( Particle_s *pPart );
	/*****************
	*	Update Part
	* @purpose: Per Frame update of particle functions
	* @param Particle_s *: Pointer to particle being updated
	* @return void: no param
	*****************/
	virtual void	UpdatePart( Particle_s *pPart );
	/*****************
	*	Render Part
	* @purpose: Render the particle to make it visable
	* @param Particle_s *: Pointer to particle being renderd
	* @return void: no param
	*****************/
	virtual	void	RenderPart( Particle_s *pPart );
private:
	/*****************
	*	Fill Part
	* @purpose: fills in the data of the newly created particle
	* @param void: no param
	* @return void: no param
	*****************/
	void			FillPart( void );
	Particle_s		m_ptPart;			// particle
	cl_entity_t		*m_pEnt;			// entity			
};

class CEffPoint : public CParticleSystem 
{
public:
		/*****************
	*	Constructor
	* @purpose: intializes our vars setup a basic particle template
	* @param vec3_t vOrg: Origin
	* @param vec3_t vMaxVel: Max velocity
	* @param vec3_t vMinVel: Min velocity
	* @param vec3_t vPMaxVel: Max per frame velocity
	* @param vec3_t vPMinVel: Min per frame velocity
	* @param vec3_t vAng: Angle
	* @param colorRGBA clEdColor: end color for color blending
	* @param colorRGBA clStColor: start color for color vlending
	* @param float flBncScl: how much to bounce by
	* @param float flGrav: How much gravity
	* @param float flLife: How long it lives
	* @param float flFR: sprite framerate
	* @param float flStSize: start size for bleding
	* @param float flEdSize: end size for blending
	* @param float flSysLife: sytem life
	* @param float flDelay: system delay to start
	* @param float flPartDelay: delay till next particle
	* @param int iBounces: number of bounces
	* @param int iFlags: special flags for system
	* @param int iIndex: Entity attached to's index
	* @param int iPPS: Particles spawned per second
	* @param int iAngLock: Specifed angle locking
	* @param char *pTex: Name of the texture
	* @return bool: Tells us if its ok to render the system
	*****************/
	CEffPoint( vec3_t vOrg, vec3_t vMaxVel, vec3_t vMinVel, vec3_t vPMaxVel, vec3_t vPMinVel,
				vec3_t vAng, colorRGBA clEdCol, colorRGBA clStCol, float flBncScl, float flGrav,
				float flLife, float flFR, float flStSize, float flEdSize, float flSysLife, float flDelay,
				float flPartDelay, int iBounces, int iFlags, int iIndex, int iPPS, int iAngLock, char *pTex );		// constructor
	/*****************
	*	Test System
	* @purpose: Tests the system to see if its worth rendering
	* @param void: no param
	* @return bool: Tells us if its ok to render the system
	*****************/
	virtual	bool	TestSystem( void );
	/*****************
	*	Update System
	* @purpose: Updates all the crucial funcs of the system like adding new parts
	* @param void: no param
	* @return void: no param
	*****************/
	virtual	void	UpdateSystem( void );
	/*****************
	*	Test Part
	* @purpose: Test to see if particle is in working order
	* @param Particle_s *: Pointer to particle being tested
	* @return bool: If particle is worth being updated and drawn
	*****************/
	virtual bool	TestPart( Particle_s *pPart );
	/*****************
	*	Update Part
	* @purpose: Per Frame update of particle functions
	* @param Particle_s *: Pointer to particle being updated
	* @return void: no param
	*****************/
	virtual void	UpdatePart( Particle_s *pPart );
	/*****************
	*	Render Part
	* @purpose: Render the particle to make it visable
	* @param Particle_s *: Pointer to particle being renderd
	* @return void: no param
	*****************/
	virtual	void	RenderPart( Particle_s *pPart );
private:
	/*****************
	*	Fill Part
	* @purpose: fills in the data of the newly created particle
	* @param void: no param
	* @return void: no param
	*****************/
	void			FillPart( void );
	Particle_s		m_ptPart;			// particle
	cl_entity_t		*m_pEnt;			// entity
};