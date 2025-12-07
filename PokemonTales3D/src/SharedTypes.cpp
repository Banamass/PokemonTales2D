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
	 if (loc == Location::MiddleTop)
		 return glm::vec2(size.x / 2, size.y);
	 if (loc == Location::MiddleLeft)
		 return glm::vec2(0.0f, size.y / 2);
	 if (loc == Location::MiddleBottom)
		 return glm::vec2(size.x / 2, 0.0f);
	 if (loc == Location::MiddleRight)
		 return glm::vec2(size.x, size.y / 2);
	 if (loc == Location::Middle)
		 return glm::vec2(size.x / 2, size.y / 2);
}

glm::vec2 LocationToPosition(FloatRect rect, Location loc) {
	return rect.pos + LocationToPosition(rect.size, loc);
}

bool IntRect::Contains(glm::ivec2 v) const {
	return (v.x >= pos.x && v.x < pos.x + size.x
		&& v.y >= pos.y && v.y < pos.y + size.y);
}

bool IntRect::Intersects(IntRect rect) const {
	return !(pos.x >= rect.pos.x + rect.size.x
		|| rect.pos.x >= pos.x + size.x
		|| pos.y >= rect.pos.y + rect.size.y
		|| rect.pos.y >= pos.y + size.y);
}

bool FloatRect::Contains(glm::vec2 v) const {
	return (v.x >= pos.x && v.x <= pos.x + size.x
		&& v.y >= pos.y && v.y <= pos.y + size.y);
}

bool FloatRect::Intersects(IntRect rect) const {
	return !(pos.x > rect.pos.x + rect.size.x
		|| rect.pos.x > pos.x + size.x
		|| pos.y > rect.pos.y + rect.size.y
		|| rect.pos.y > pos.y + size.y);
}

glm::vec3 ColorFromRGB(uint8_t r, uint8_t g, uint8_t b) {
	return glm::vec3(((float)r) / 255.0f, ((float)g) / 255.0f, ((float)b) / 255.0f);
}

std::ostream& operator<<(std::ostream& s, glm::vec3 vec) {
	s << "x : " << vec.x << ", y : " << vec.y << ", z : " << vec.z;
	return s;
}
std::ostream& operator<<(std::ostream& s, glm::ivec3 vec) {
	s << "x : " << vec.x << ", y : " << vec.y << ", z : " << vec.z;
	return s;
}
std::ostream& operator<<(std::ostream& s, glm::vec2 vec) {
	s << "x : " << vec.x << ", y : " << vec.y;
	return s;
}
std::ostream& operator<<(std::ostream& s, glm::ivec2 vec) {
	s << "x : " << vec.x << ", y : " << vec.y;
	return s;
}