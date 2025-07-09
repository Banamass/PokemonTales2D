#include "Drawable.h"

/*------------------------Transform------------------------*/

Transform::Transform() {
	ResetTransformations();
}

void Transform::ResetTransformations() {
	rotation = glm::vec3(0.0f);
	translation = glm::vec3(0.0f);
	scaling = glm::vec3(1.0f);
	transform = glm::mat4(1.0f);
}

void Transform::Move(glm::vec3 move) {
	translation += move;
	ComputeTransform();
}

void Transform::Scale(glm::vec3 scale) {
	scaling.x *= scale.x;
	scaling.y *= scale.y;
	scaling.z *= scale.z;
	ComputeTransform();
}

void Transform::Rotate(glm::vec3 l_rotation) {
	rotation += l_rotation;
	ComputeTransform();
}

void Transform::ComputeTransform() {
	transform = glm::mat4(1.0f);
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::translate(transform, translation);
	transform = glm::scale(transform, scaling);
}

glm::mat4 Transform::GetTransform() { 
	return transform;
}

/*------------------------Drawable------------------------*/

Drawable::Drawable(Model * l_model, Shader * l_shader)
	: model(l_model), shader(l_shader) {
	material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
	material.shininess = 1.0f;
}
Drawable::Drawable(Model* l_model, Shader* l_shader, Material l_material)
	: model(l_model), shader(l_shader), material(l_material) {

}
Drawable::~Drawable() {}

void Drawable::SetMaterial(const Material& l_materiel) {
	material = l_materiel;
}

void Drawable::Move(glm::vec3 move) {
	transform.Move(move);
}

void Drawable::Scale(glm::vec3 scale) {
	transform.Scale(scale);
}

void Drawable::Rotate(glm::vec3 l_rotation) {
	transform.Rotate(l_rotation);
}

/*------------------------DrawableInstanced------------------------*/

DrawableInstanced::DrawableInstanced(Drawable* l_drawable, std::vector<Transform*>& l_instanceTransforms)
	:drawable(l_drawable) {
	std::vector<glm::mat4> instanceMatrix;
	for (Transform* trans : l_instanceTransforms) {
		instanceMatrix.push_back(trans->GetTransform());
		instanceTransforms.push_back(trans);
	}
	std::cout << instanceMatrix.size() << std::endl;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, instanceMatrix.size() * sizeof(glm::mat4),
		&instanceMatrix[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	isVAOSetup = false;
}

unsigned int DrawableInstanced::GetNbInstance() {
	return instanceTransforms.size();
}

unsigned int DrawableInstanced::GetInstanceVBO() {
	return instanceVBO;
}

bool DrawableInstanced::IsVAOSetup() {
	return isVAOSetup;
}

void DrawableInstanced::VAOSetup() {
	isVAOSetup = true;
}
