#include "Pokemon.h"

Pokemon::Pokemon(const std::string& l_name, glm::ivec2 l_size, APlayer* l_trainer,
	ShaderManager* shaderMgr, glm::vec3 l_color)
	: name(l_name), size(l_size), trainer(l_trainer), model("Resources\\Box\\cube.obj"),
	sprite(&model, shaderMgr->GetShader("ModelShader")), color(l_color),
	OBB(&model, shaderMgr->GetShader("ModelShader"))
{
	health = 100.0f;
	moveRange = 5;
	height = 1.0f + 0.6f*2.0f;

	Drawable::Material mat;
	mat.ambient = 0.6f * color;
	mat.diffuse = 0.8f * color;
	mat.specular = color;
	mat.shininess = 32.0f;
	
	sprite.SetMaterial(mat);
	sprite.Scale(glm::vec3(0.6f));

	Drawable::Material matOBB;
	matOBB.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	OBB.SetMaterial(matOBB);
	OBB.Scale(glm::vec3(0.65f));
}
Pokemon::~Pokemon(){}

bool Pokemon::TestRayIntersection(
	const glm::vec3& ray_origin,
	const glm::vec3& ray_direction,
	float& interstion_distance) 
{
	return sprite.TestRayIntersection(ray_origin, ray_direction, interstion_distance);
}

void Pokemon::Render(Window* win, glm::ivec2 pos){
	glm::vec3 realPos(pos.x * Constants::BOX_SIZE, 1.0f, pos.y * Constants::BOX_SIZE);
	sprite.SetPosition(realPos);
	OBB.SetPosition(realPos);

	win->Draw(sprite);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	win->Draw(OBB);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

glm::ivec2 Pokemon::GetSize() { return size; }
float Pokemon::GetHeight() { return height; }
int Pokemon::GetMoveRange() { return moveRange; }
void Pokemon::SetMoveRange(int l_range) { moveRange = l_range; }
APlayer* Pokemon::GetTrainer() { return trainer; }
std::string Pokemon::GetName() { return name; }
bool Pokemon::IsKO() { return health <= 0; }

void Pokemon::TakeDamages(float l_damages) { health -= l_damages; }