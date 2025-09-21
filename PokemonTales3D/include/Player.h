#pragma once

#include "APlayer.h"
#include "EventManager.h"
#include "StateManager.h"

class Player : public APlayer {
public:
	Player(SharedContext* l_context);
	virtual ~Player();

	virtual void Setup();
	virtual void Render();
	virtual void Update(double dt);

	void PlayTurn();
	void EndTurn();

private:
	Model* boxModel;

	void KeyCallback(CallbackData data);
	void MouseButtonCallback(CallbackData data);

	enum PType { None, Default, PokeSelected, PokeMove, PokeAttack };

	class State {
	public:
		State(Player* l_player) : player(l_player), type(PType::None),
			cursorDrawable(player->boxModel, player->context->shaderManager->GetShader("ModelShader")),
			cursor(glm::ivec2(1, 1)) {}
		virtual ~State() {};

		virtual void KeyCallback(Key_Data& data) {};
		virtual void MouseButtonCallback(MouseButton_Data& data) {};

		virtual void Render() {};
		virtual void Update(double dt){}

		virtual PType GetType() { return type; }

	protected:
		virtual void UpdateCursor();
		virtual void RenderCursor();

		PType type;
		Player* player;

		Drawable cursorDrawable;
		SquareArea cursor;
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