/*
* Extended Player Animation System
* @version 12/20/03
* @author: Joseph 'Pcjoe' Florencio
*/

#ifndef PLAYERANIMATION_H
#define PLAYERANIMATION_H

class CPlayerAnimation : public CEntAnimation
{
public:
	/*
	* Remove Weapon Anims
	*/
	void RemoveWepAnims( void );

	/*
	* Add weapon animation
	* @param PLAY_ANIM eAnim: Weapon anim type
	*/
	std::list<entanim_s>::iterator AddWeaponAnimation( PLAY_ANIM eAnim );
};

#endif