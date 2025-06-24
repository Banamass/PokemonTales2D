#pragma once

#include "APlayer.h"

class Player : public APlayer {
public:
	Player(SharedContext* l_context);
	virtual ~Player();

	void Setup();
	void PlayTurn();
	void Render();
	void EndTurn();

private:
	void SetBindings();
	void SetGlobalCallbacks();
	void UnsetGlobalCallbacks();

	enum StateType {None, Default, PokeSelected, PokeMove, PokeAttack};

	class State {
	public:
		State(Player* l_player) : player(l_player), type(StateType::None) {}
		virtual ~State() {};

		virtual void SetCallbacks() = 0;
		virtual void UnsetCallbacks() = 0;
		virtual void Render() {};

		virtual StateType GetType() { return type; }

	protected:
		StateType type;
		Player* player;
	};

	void SwitchState(State* newState);
	State* state;

	class DefaultState : public State {
	public:
		DefaultState(Player* l_player);

		void SetCallbacks();
		void UnsetCallbacks();

		void Select();
	};

	class PokeSelectedState : public State {
	public:
		PokeSelectedState(Player* l_player, Pokemon* l_selectedPokemon);

		void SetCallbacks();
		void UnsetCallbacks();

		void UpdateCursor();
		void Unselect();
		void Move();
		void Attack();

	private:
		Pokemon* selectedPokemon;
		SquareArea cursor;
	};

	class PokeMoveState : public State{
	public:
		PokeMoveState(Player* l_player, Pokemon* l_selectedPokemon);

		void SetCallbacks();
		void UnsetCallbacks();
		void Render();

		void Unmove();
		void Move();

	private:
		Pokemon* selectedPokemon;
		sf::RectangleShape pokeFrame;
		MoveArea moveArea;
	};

	class PokeAttackState : public State {
	public:
		PokeAttackState(Player* l_player, Pokemon* l_selectedPokemon);
		void SetCallbacks();
		void UnsetCallbacks();

		void UpdateCursor();
		void Attack();

	private:
		Pokemon* selectedPokemon;
		SquareArea cursor;
	};
};