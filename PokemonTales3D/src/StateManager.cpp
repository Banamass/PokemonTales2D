#include "StateManager.h"

/*-----------------State-----------------*/

State::State(SharedContext* l_context)
	: type(StateType::NoneState), isActivated(false), context(l_context), isTranscendent(false), isTransparent(false)
{}

void State::SetIsTransparent(bool b) { isTransparent = b; }
void State::SetIsTranscendent(bool b) { isTranscendent = b; }

bool State::GetIsTransparent() { return isTransparent; }
bool State::GetIsTranscendent() { return isTranscendent; }
bool State::GetIsActivated() { return isActivated; }

StateType State::GetType() { return type; }

/*-----------------StateManager-----------------*/

StateManager::StateManager(SharedContext* l_context)
	: context(l_context)
{
	stateToActivate = StateType::NoneState;
	context->stateManager = this;
}
StateManager::~StateManager(){
	for (auto state : states)
		delete state;
}

void StateManager::Update(double dt) {
	if (states.size() == 0)
		return;
	auto itr = states.end();
	do {
		itr--;
		(*itr)->Update(dt);
	} while (itr != states.begin() && (*itr)->GetIsTranscendent());
	UpdateStateToActivate();
}
void StateManager::Render() {
	if (states.size() == 0)
		return;
	auto itr = states.end();
	do {
		itr--;
		(*itr)->Render();
	} while (itr != states.begin() && (*itr)->GetIsTransparent());
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
void StateManager::ActivateState(StateType type) {
	stateToActivate = type;
}
void StateManager::UpdateStateToActivate() {
	if (stateToActivate == StateType::NoneState)
		return ;
	auto itr = GetStateItr(stateToActivate);
	if (itr == states.end())
		return ;
	if (!states.empty())
		states[states.size() - 1]->Desactivate();
	State* s = *itr;
	s->Activate();
	states.erase(itr);
	states.push_back(s);
	stateToActivate = StateType::NoneState;
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