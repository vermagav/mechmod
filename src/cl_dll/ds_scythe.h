/**********************************************************
*			DEATHSCYTHE SCYTHE
***********************************************************
*	Purpose: Sprite for deathscythe's scythe
*	Created On: 4/15/04
*	Last Edited: 4/15/04
*	Created By: Jason "ssjason123" Matson
*	Created For: MechMod
**********************************************************/

class CScythe
{
public:
	CScythe( );
	~CScythe( );
	void		RenderScythe( void );
	void		Init( void );
private:
	void		DrawScythe( void );
	void		PreRender( void );
	void		PostRender( void );
	bool		IsDs( int iIndex );
	void		UpdateSprite( void );
	int			m_iFrame;
	Texture_s/*SprTex_s*/	*m_pTex;
	float		m_flFrameRate;
};

extern CScythe g_Scythe;