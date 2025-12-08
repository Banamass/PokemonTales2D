#pragma once

#include "glm/glm.hpp"
#include "SharedTypes.h"

/**/
template <typename T>
struct SmoothLinearTransition {
	SmoothLinearTransition(T l_current, T l_target, float l_C, float l_F)
		: C(l_C), F(l_F), current(l_current), target(l_target) { }

	//Update the transition, return if the target has been reached
	bool Update(float dt) {
		T diff = target - current;
		float normDiff = glm::dot(diff, diff);
		if (normDiff == 0.0f)
			return true;
		T dir = glm::normalize(diff);
		T delta = dir * (normDiff * F + C) * dt;
		if (normDiff <= glm::dot(delta, delta)) {
			current = target;
			return true;
		}
		current += delta;
		return false;
	}
	float F;
	float C;

	T current;
	T target;
};

class Timer {
public:
	enum Mode { Simple, Loop };

	Timer();
	Timer(double time, Mode mode = Simple);
	Timer(const Timer& c_timer);
	Timer(Timer&& c_timer) noexcept;
	~Timer();

	//Update the timer and return is the timer is over
	bool Update(double dt);
	//Reset the timer
	void Reset();
	//Set the timer value, and reset it
	void SetTime(double time);
	//Set the mode of the timer
	void SetMode(Mode l_mode);

private:
	Mode mode;
	double timer;
	double elapsed;
};