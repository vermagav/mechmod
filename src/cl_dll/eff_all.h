/**********************************************************
*				EFFECTS ALL
***********************************************************
*	Purpose: Usefull stuff for every effect
*	Created On: 1/6/04
*	Last Edited: 1/6/03
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

//taken from tri.cpp
extern vec3_t g_CameraAngles;

#define DLLEXPORT __declspec( dllexport )

extern "C"
{
	// Added by Pcjoe
	int DLLEXPORT CL_IsThirdPerson( void );
};
// end stolen code hehe

// Just a mess of effects we dont want to define for each class
// since they are used alot
class CPartEffects 
{
public:
	/*****************
	*	Bounce
	* @purpose: Bounces the particle
	* @param Particle_s *pPart: particle we want to bounce
	* @return void: no param
	*****************/
	void		Bounce( Particle_s *pPart );
	/*****************
	*	Animate
	* @purpose: Animates the particle
	* @param Particle_s *pPart: particle we want to anim
	* @return void: no param
	*****************/
	void		Anim( Particle_s *pPart );
	/*****************
	*	Water Sur
	* @purpose: Finds surface of water from particle intersecting
	* @param Particle_s *pPart: particle thats intersetcting
	* @return vec3_t: Point where the surface is
	*****************/
	vec3_t		WaterSur( Particle_s *pPart );
	/*****************
	*	Anim On Collsion
	* @purpose: Sets the particle to animate on collision 
	*			kills after max framerate
	* @param Particle_s *pPart: particle we want to anim
	* @return void: no param
	*****************/
	void		AnimOnCol( Particle_s *pPart );		// good for splashing rain
	/*****************
	*	Render Quad
	* @purpose: Renders a quad
	* @param Particle_s *pPart: particle we want to render
	* @return void: no param
	*****************/	
	void		RenderQuad( Particle_s *pPart );
	/*****************
	*	Collision
	* @purpose: Tests point collision
	* @param Particle_s *pPart: particle we want to test collision on
	* @return bool: if we collided or not
	*****************/
	bool		Collision( Particle_s *pPart );
	/*****************
	*	CalcDeltas
	* @purpose: Calculate scaling vars
	* @param Particle_s *pPart: particle we want to calc deltas for
	* @param float flDistTime: time between frames
	* @return void: no param
	*****************/
	void		CalcDeltas( Particle_s *pPart, float flDistTime );
	/*****************
	*	RandomRoundVec
	* @purpose: Calculates a random round vector
	* @param vec3_t vMin: Min vector
	* @param vec3_t vMax: Max vector
	* @return vec3_t: Computed round vector
	*****************/
	vec3_t		RandomRoundVec( vec3_t vMin, vec3_t vMax );
private:
};

extern CPartEffects g_PartEffects;