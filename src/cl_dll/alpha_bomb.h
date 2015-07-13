/**********************************************************
*			NEW ALPHABOMB EFFECT
***********************************************************
*	Purpose: New Alphabomb Effect, Since model takes lighting
*	Created On: 8/31/04
*	Last Edited: 8/31/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

#ifndef ALPHA_BOMB_H
#define ALPHA_BOMB_H

#define	MAX_ALPHABOMB	60
#define ALPHA_FRAME		1.0f/30.0f


class CAlphaBomb
{
public:
				CAlphaBomb( );
	void		Reset( void );
	void		LoopEffect( void );
	void		Initialize( void );
	void		Render( cl_entity_t *pEnt );
private:
	int			m_iFrame;
	float		m_flLastFrame;
	Texture_s	*m_pTex;
};

extern CAlphaBomb g_AlphaBomb;

#endif