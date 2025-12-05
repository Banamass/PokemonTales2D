#pragma once

#include "Drawable.h"

class PlayerBody {
public:
	PlayerBody(SharedContext* l_context);
	virtual ~PlayerBody();

private:
	SharedContext* context;

};