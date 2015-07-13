/**********************************************************
*				SMOKE EFFECT
***********************************************************
*	Purpose: Smoke effect for missles
*	Created On: 1/18/04
*	Last Edited: 1/18/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

class CEffSmoke : public CParticleSystem
{
public:
	/*****************
	*	Constructor
	* @purpose: intializes our vars
	* @param int iIndex: entity index
	*****************/
	CEffSmoke( int iIndex );		// constructor
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
	cl_entity_t		*m_pEnt;			// entity attached to
};