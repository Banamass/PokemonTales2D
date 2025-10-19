#pragma once

#include "APlayer.h"
#include "EventManager.h"
#include "StateManager.h"

class Player : public APlayer {
public:
	Player(SharedContext* l_context);
	virtual ~Player();

	void Setup();
	void Render();
	void Update(double dt);

	void PlayTurn();
	void EndTurn();

private:
	Model* boxModel;

	void KeyCallback(CallbackData data);
	void MouseButtonCallback(CallbackData data);

	//Enum with all state taking by a player
	enum PType { None, Default, PokeSelected, PokeMove, PokeAttack };

	/*This class is the upper class state, which does nothing but other
	states inherit from it
	*/
	class State {
	public:
		State(Player* l_player);
		virtual ~State() {};

		virtual void KeyCallback(Key_Data& data) {};
		virtual void MouseButtonCallback(MouseButton_Data& data) {};

		virtual void Render();
		virtual void Update(double dt) {}

		virtual PType GetType() { return type; }

	protected:
		virtual void UpdateCursor();
		virtual void RenderCursor();
		// Set the selected pokemon area to poke, if poke is nullptr then remove the selected pokemon area
		void UpdateSelectedPokeArea(Pokemon* poke);
		void RenderSelectedPokeArea();

		PType type;
		Player* player;

		Drawable cursorDrawable;
		Drawable selectedPokeAreaDrawable;
		SquareArea cursor;
		SquareArea selectedPokeArea;
	};

	void SwitchState(State* newState);
	State* state;

	class DefaultState : public State {
	public:
		DefaultState(Player* l_player);

		virtual void KeyCallback(Key_Data& data);
		virtual void MouseButtonCallback(MouseButton_Data& data);

		virtual void Render();
		virtual void Update(double dt);

		void Select();

	protected:
		
	};

	class PokeSelectedState : public State {
	public:
		PokeSelectedState(Player* l_player, Pokemon* l_selectedPokemon);

		virtual void Update(double dt);

		virtual void KeyCallback(Key_Data& data);
		virtual void MouseButtonCallback(MouseButton_Data& data);

		void Unselect();
		void Move();
		void Attack(int moveId);

	private:
		Pokemon* selectedPokemon;
	};

	class PokeMoveState : public State {
	public:
		PokeMoveState(Player* l_player, Pokemon* l_selectedPokemon);

		virtual void KeyCallback(Key_Data& data);
		virtual void MouseButtonCallback(MouseButton_Data& data);

		virtual void Render();
		virtual void Update(double dt);

		void Unmove();
		void Move();

	private:
		Pokemon* selectedPokemon;
		//sf::RectangleShape pokeFrame;
		Drawable moveBox;
		MoveArea moveArea;
	};

	class PokeAttackState : public State {
	public:
		PokeAttackState(Player* l_player, Pokemon* l_selectedPokemon, int moveId);

		virtual void KeyCallback(Key_Data& data);
		virtual void MouseButtonCallback(MouseButton_Data& data);

		virtual void Render();
		virtual void Update(double dt);

		void Unattack();
		void Attack();

	private:
		void SetMove(int id);

		Pokemon* selectedPokemon;
		PokemonMove* move;
	};
};