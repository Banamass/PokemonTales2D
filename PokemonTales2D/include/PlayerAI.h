#pragma once

#include "APlayer.h"

class PlayerAI : public APlayer {
public:
	PlayerAI(SharedContext* l_context) : APlayer(l_context){}
	virtual ~PlayerAI(){}

	void PlayTurn();
	void Setup();

protected:

};