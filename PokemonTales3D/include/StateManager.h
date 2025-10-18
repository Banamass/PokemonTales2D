#pragma once

#include "SharedTypes.h"
#include <vector>

/*Enum for describing a state*/
enum StateType { Menu, Battle, Options, NoneState };

/*Class describing a state in the game*/
class State {
public:
	State(SharedContext* l_context);
	virtual ~State() {};

	//Update method for the state
	virtual void Update(double dt) = 0;
	//Render method for the state
	virtual void Render() = 0;

	//Activate the state
	virtual void Activate() { isActivated = true; }
	//Desactivate the state
	virtual void Desactivate() { isActivated = false; }

	//Getters and setters

	void SetIsTransparent(bool b);
	void SetIsTranscendent(bool b);

	bool GetIsTransparent();
	bool GetIsTranscendent();
	bool GetIsActivated();

	StateType GetType();

protected:
	SharedContext* context;

	StateType type;

	bool isTransparent;
	bool isTranscendent;
	bool isActivated;
};

//This class manage different state of the game, it can store one state per type
class StateManager {
public:
	StateManager(SharedContext* l_context);
	virtual ~StateManager();

	/* Udpate states : starting with the state on the top of the stack, while the current state
	is transcendent the current state is the next state on the stack*/
	void Update(double dt);
	/* Render states : starting with the state on the top of the stack, while the current state
	is transparent the current state is the next state on the stack*/
	void Render();

	/*Add a new state, if a such state type already exists return false either return true*/
	bool AddState(StateType type, State* state);
	/*Remove a state, if no such state type exists then return false, either return true*/
	bool RemoveState(StateType type);
	/*Activate a state, if no such state type exists then return false, either return true
	Put the activated state on the top of the stack*/
	void ActivateState(StateType type);

	/*Get a reference to the speciefed state*/
	State* GetState(StateType type);

private:
	void UpdateStateToActivate();

	std::vector<State*>::iterator GetStateItr(StateType type);

	SharedContext* context;

	std::vector<State*> states;
	StateType stateToActivate;
};