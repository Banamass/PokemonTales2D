#include "StateManager.h"

/*-----------------State-----------------*/

State::State(SharedContext* l_context)
	: type(StateType::NoneState), context(l_context), isTranscendent(false), isTransparent(false)
{}

void State::SetIsTransparent(bool b) { isTransparent = b; }
void State::SetIsTranscendent(bool b) { isTranscendent = b; }

bool State::GetIsTransparent() { return isTransparent; }
bool State::GetIsTranscendent() { return isTranscendent; }

StateType State::GetType() { return type; }

/*-----------------StateManager-----------------*/

StateManager::StateManager(SharedContext* l_context)
	: context(l_context)
{
	context->stateManager = this;
}
StateManager::~StateManager(){
	for (auto state : states)
		delete state;
}

void StateManager::Update(double dt) {
	states[states.size() - 1]->Update(dt);
}
void StateManager::Render() {
	states[states.size() - 1]->Render();
}

bool StateManager::AddState(StateType type, State* state) {
	State* curState = GetState(type);
	if (curState != nullptr)
		return false;
	states.push_back(state);
	return true;
}
bool StateManager::RemoveState(StateType type) {
	auto itr = GetStateItr(type);
	if (itr == states.end())
		return false;
	delete* itr;
	states.erase(itr);
	return true;
}
bool StateManager::ActivateState(StateType type) {
	auto itr = GetStateItr(type);
	if (itr == states.end())
		return false;
	State* s = *itr;
	states.erase(itr);
	states.push_back(s);
	return true;
}

State* StateManager::GetState(StateType type) {
	for (auto state : states) {
		if (state->GetType() == type)
			return state;
	}
	return nullptr;
}

std::vector<State*>::iterator StateManager::GetStateItr(StateType type) {
	std::vector<State*>::iterator itr = states.begin();
	for (; itr != states.end(); itr++) {
		if ((*itr)->GetType() == type) {
			break;
		}
	}
	return itr;
}