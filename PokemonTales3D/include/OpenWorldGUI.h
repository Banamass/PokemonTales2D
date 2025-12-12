#pragma once

#include "GameGUI.h"

/*This class is the gui of the open world state
Implement Notifier, notification tags:
- answer : when a question is answered */
class OpenWorldGUI : public Notifier {
public:
	OpenWorldGUI(SharedContext* l_context);
	virtual ~OpenWorldGUI();

	void Update(double dt);
	void Render();

	void AskQuestion(const std::string& question, const std::vector<std::string> responses);
	std::string GetAnswer();

	bool GetIsBlocking();

private:
	void QuestionButtonNotifier();

	struct QuestionPanel {
		bool activated;
		Panel* panel;

		RectangleShape* background;
		Text* question;
		std::vector<Button*> choices;
		std::string answer;
	};
	bool isBlocking;
	SharedContext* context;

	QuestionPanel question;
};