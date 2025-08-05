#pragma once

#include "SharedTypes.h"
#include <vector>

enum StateType { Menu, Battle, Pause, NoneState };

class State {
public:
	State(SharedContext* l_context);
	virtual ~State() {};

	virtual void Update(double dt) = 0;
	virtual void Render() = 0;

	void SetIsTransparent(bool b);
	void SetIsTranscendent(bool b);

	bool GetIsTransparent();
	bool GetIsTranscendent();

	StateType GetType();

protected:
	SharedContext* context;

	StateType type;

	bool isTransparent;
	bool isTranscendent;
};

class StateManager {
public:
	StateManager(SharedContext* l_context);
	virtual ~StateManager();

	void Update(double dt);
	void Render();

	/*Add a new state, if a such state type already exists return false either return true*/
	bool AddState(StateType type, State* state);
	/*Remove a state, if no such state type exists then return false, either return true*/
	bool RemoveState(StateType type);
	/*Activate a state, if no such state type exists then return false, either return true*/
	bool ActivateState(StateType type);

	State* GetState(StateType type);

private:
	std::vector<State*>::iterator GetStateItr(StateType type);

	SharedContext* context;

	std::vector<State*> states;
};