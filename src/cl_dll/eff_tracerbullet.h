/**********************************************************
*			EXPLOSION EFFECTS
***********************************************************
*	Purpose: Particle Tracer
*	Created On: 1/15/04
*	Last Edited: 1/15/03
*	Created By: Joseph 'Pcjoe' Florencio
*	Created For: MechMod
**********************************************************/

class CEffTracerBullet : public CParticleSystem
{
public:
	/*****************
	*	Constructor
	* @purpose: intializes our vars
	*****************/
	CEffTracerBullet();		// constructor
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

	/*****************
	*	Fill Part
	* @purpose: fills in the data of the newly created particle
	* //added by jason
	* @param iType: type of tracer
	* @param void: no param
	* @return void: no param
	*****************/
	void			FillPart( vec3_t vecOrigin, vec3_t vecVelocity, int iType );

private:
	/*****************
	*	Render Tracer
	* @purpose: Renders the tracer particle
	* @param Particle_s *pPart: Particle we're rendering
	* @return void: no param
	*****************/
	void RenderTracer( Particle_s *pPart );

	Texture_s *m_pTex1;
	Texture_s *m_pTex2;
};
