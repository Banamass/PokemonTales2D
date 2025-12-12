#include "OpenWorldGUI.h"

OpenWorldGUI::OpenWorldGUI(SharedContext* l_context)
	: context(l_context)
{
	context->openWorldGui = this;

	isBlocking = false;
	Font* font = context->fontManager->RequireGetResource("Arial");
	question.activated = false;

	question.panel = new Panel(context->shaderManager);
	question.background = (RectangleShape*)question.panel->AddElement(new RectangleShape(
		glm::vec2(0, 0), context->shaderManager->GetShader("SimpleShader")
	), -1);
	question.background->SetColor(glm::vec4(glm::vec3(1.0f), 0.5f));
	question.question = (Text*)question.panel->AddElement(new Text(
		font, "Question", context->shaderManager->GetShader("FontShader")
	), 1);
	question.question->SetCharacterSize(20.0f);
	question.question->SetColor(glm::vec3(0.0f));
}
OpenWorldGUI::~OpenWorldGUI(){
	context->fontManager->ReleaseResource("Arial");

	delete question.panel;
}

void OpenWorldGUI::Update(double dt){
	Window* win = context->win;
	if (question.activated) {
		for (auto& b : question.choices) {
			b->Update(win);
		}
	}
}
void OpenWorldGUI::Render(){
	Window* win = context->win;
	if (question.activated) {
		win->DrawStatic(question.panel);
	}
}

void OpenWorldGUI::AskQuestion(const std::string& quest, const std::vector<std::string> responses){
	Font* font = context->fontManager->GetResource("Arial");
	for (auto& br : question.choices) {
		question.panel->DeleteElement(br);
	}
	question.choices.clear();
	float xpos = -75.0f;
	for (auto& r : responses) {
		Button* newB = (Button*) question.panel->AddElement(new Button(font, context->shaderManager));
		newB->SetCharacterSize(15.0f);
		newB->SetFrameColor(glm::vec4(1.0f));
		newB->SetText(r);
		newB->SetTextColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		newB->SetSize(glm::vec2(100.0f, 30.0f));
		newB->SetOrigin(Location::MiddleTop);
		newB->SetPos(glm::vec2(xpos, 0.0f));

		newB->Subscribe("Click", &OpenWorldGUI::QuestionButtonNotifier, this);

		xpos += 150.0f;

		question.choices.push_back(newB);
	}
	question.background->SetSize(glm::vec2(250, 150));
	question.background->SetOrigin(Location::Middle);
	question.background->SetPos(glm::vec2(0.0f));

	question.question->SetText(quest);
	question.question->SetOrigin(Location::MiddleBottom);
	question.question->SetPos(glm::vec2(0.0f));

	question.panel->SetOrigin(Location::Middle);
	question.panel->SetPos(glm::vec2(Constants::WIN_WIDTH / 2.0f, Constants::WIN_HEIGHT / 2.0f));

	question.activated = true;	
	isBlocking = true;
}

void OpenWorldGUI::QuestionButtonNotifier() {
	isBlocking = false;
	question.activated = false;
	question.answer = "None";
	for (auto& b : question.choices) {
		if (b->GetClick()) {
			question.answer = b->GetText();
			break;
		}
	}

	NotifyAll("answer");
}

std::string OpenWorldGUI::GetAnswer() {
	return question.answer;
}

bool OpenWorldGUI::GetIsBlocking() {
	return isBlocking;
}