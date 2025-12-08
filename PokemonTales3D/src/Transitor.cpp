#include"Transitor.h"

/*------------------Timer------------------*/

Timer::Timer()
	: timer(0.0), elapsed(0.0), mode(Mode::Loop)
{}
Timer::Timer(double time, Mode l_mode)
	: timer(time), elapsed(0.0), mode(l_mode)
{}
Timer::Timer(const Timer& c_timer)
	: timer(c_timer.timer), elapsed(0.0), mode(c_timer.mode)
{}
Timer::Timer(Timer&& c_timer) noexcept
	: timer(c_timer.timer), elapsed(0.0), mode(c_timer.mode)
{}
Timer::~Timer(){}

bool Timer::Update(double dt){
	if (mode == Simple) {
		elapsed += dt;
		return elapsed >= timer;
	}
	else {
		elapsed += dt;
		if (elapsed >= timer) {
			Reset();
			return true;
		}
		else {
			return false;
		}
	}
}
void Timer::Reset() { elapsed = 0.0; }
void Timer::SetTime(double time) { timer = time; Reset(); }