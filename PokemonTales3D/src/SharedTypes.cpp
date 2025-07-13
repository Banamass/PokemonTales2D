#include "SharedTypes.h"

glm::vec2 LocationToPosition(glm::vec2 size, Location loc) {
	 if (loc == Location::BottomLeft)
		 return glm::vec2(0, 0);
	 if (loc == Location::TopLeft)
		 return glm::vec2(0, size.y);
	 if (loc == Location::TopRight)
		 return glm::vec2(size.x, size.y);
	 if (loc == Location::BottomRight)
		 return glm::vec2(size.x, 0);
	 if (loc == Location::Middle)
		 return glm::vec2(size.x / 2, size.y / 2);
}

bool IntRect::Contains(glm::ivec2 v) const {
	return (v.x >= pos.x && v.x <= pos.x + size.x
		&& v.y >= pos.y && v.y <= pos.y + size.y);
}

bool IntRect::Intersects(IntRect rect) const {
	return !(pos.x > rect.pos.x + rect.size.x
		|| rect.pos.x > pos.x + size.x
		|| pos.y > rect.pos.y + rect.size.y
		|| rect.pos.y > pos.y + size.y);
}

bool FloatRect::Contains(glm::ivec2 v) const {
	return (v.x >= pos.x && v.x <= pos.x + size.x
		&& v.y >= pos.y && v.y <= pos.y + size.y);
}

bool FloatRect::Intersects(IntRect rect) const {
	return !(pos.x > rect.pos.x + rect.size.x
		|| rect.pos.x > pos.x + size.x
		|| pos.y > rect.pos.y + rect.size.y
		|| rect.pos.y > pos.y + size.y);
}