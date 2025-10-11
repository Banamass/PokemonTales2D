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

void Transform::SetPosition(glm::vec3 pos) {
	translation = pos;
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

glm::vec3 Transform::GetScaling() {
	return scaling;
}

/*------------------------OBB------------------------*/

OBB::OBB(std::pair<glm::vec3, glm::vec3> l_AABBMinMax, Transform* l_modelTransform) {
	modelTransform = l_modelTransform;
	AABBMax = l_AABBMinMax.second;
	AABBMin = l_AABBMinMax.first;
}

bool OBB::TestRayOBBIntersection(
	glm::vec3 ray_origin,
	glm::vec3 ray_direction,
	float& interstion_distance)
{
	glm::vec3 scaling = modelTransform->GetScaling();

	glm::mat3 scale = glm::mat3(glm::scale(glm::mat4(1.0f), scaling));
	glm::vec3 aabb_min = scale * AABBMin;
	glm::vec3 aabb_max = scale * AABBMax;

	glm::mat4 ModelMatrix = glm::scale(modelTransform->GetTransform(), 1.0f / scaling);

	float tMin = 0.0f;
	float tMax = 10000000.0f;

	glm::vec3 OBBposition(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);
	glm::vec3 delta = OBBposition - ray_origin;

	//X-axis
	{
		glm::vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		if (std::abs(f) > 0.0001f) {
			float t1 = (e + aabb_min.x) / f;
			float t2 = (e + aabb_max.x) / f;
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w;
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;

			if (tMax < tMin)
				return false;
		}
		else {
			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
				return false;
		}
	}
	//Y-axis
	{
		glm::vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = glm::dot(yaxis, delta);
		float f = glm::dot(ray_direction, yaxis);

		if (std::abs(f) > 0.0001f) {
			float t1 = (e + aabb_min.y) / f;
			float t2 = (e + aabb_max.y) / f;
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w;
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;

			if (tMax < tMin)
				return false;
		}
		else {
			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
				return false;
		}
	}
	//Z-axis
	{
		glm::vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = glm::dot(zaxis, delta);
		float f = glm::dot(ray_direction, zaxis);

		if (std::abs(f) > 0.0001f) {
			float t1 = (e + aabb_min.z) / f;
			float t2 = (e + aabb_max.z) / f;
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w;
			}

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;

			if (tMax < tMin)
				return false;
		}
		else {
			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
				return false;
		}
	}

	interstion_distance = tMin;
	return true;
}

/*------------------------Drawable------------------------*/

Drawable::Drawable(Model * l_model, Shader * l_shader)
	: model(l_model), shader(l_shader), obb(model->GetAABBMinMax(), &transform),
	pos(0.0f), origin(0.0f), offset(0.0f)
{
	material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	material.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	material.specular = glm::vec3(0.0f, 0.0f, 0.0f);
	material.shininess = 1.0f;
}
Drawable::Drawable(Model* l_model, Shader* l_shader, Material l_material)
	: model(l_model), shader(l_shader), material(l_material), obb(model->GetAABBMinMax(), &transform),
	pos(0.0f), origin(0.0f), offset(0.0f) {

}
Drawable::~Drawable() {}

void Drawable::SetMaterial(const Material& l_materiel) {
	material = l_materiel;
}

bool Drawable::TestRayIntersection(
	const glm::vec3& ray_origin,
	const glm::vec3& ray_direction,
	float& interstion_distance)
{
	return obb.TestRayOBBIntersection(ray_origin,
		ray_direction,
		interstion_distance);
}

void Drawable::Move(glm::vec3 move) {
	pos += move;
	transform.Move(move);
}

void Drawable::Scale(glm::vec3 scale) {
	transform.Scale(scale);
	transform.SetPosition(GetRealPosition());
}

void Drawable::Rotate(glm::vec3 l_rotation) {
	transform.Rotate(l_rotation);
}

void Drawable::SetPosition(glm::vec3 l_pos) {
	pos = l_pos;
	transform.SetPosition(GetRealPosition());
}

void Drawable::SetOffset(glm::vec3 l_offset) { 
	offset = l_offset;
	transform.SetPosition(GetRealPosition());
}
void Drawable::SetOrigin(glm::vec3 l_origin) {
	origin = l_origin;
	transform.SetPosition(GetRealPosition());
}

glm::vec3 Drawable::GetPosition() { return pos; }
glm::vec3 Drawable::GetOffset() { return offset; }
glm::vec3 Drawable::GetOrigin() { return origin; }
glm::vec3 Drawable::GetRealPosition() { return pos + offset - origin*transform.GetScaling(); }

/*------------------------DrawableInstanced------------------------*/

DrawableInstanced::DrawableInstanced(Drawable* l_drawable, std::vector<Transform*>& l_instanceTransforms)
	:drawable(l_drawable) {
	std::vector<glm::mat4> instanceMatrix;
	for (Transform* trans : l_instanceTransforms) {
		instanceMatrix.push_back(trans->GetTransform());
		instanceTransforms.push_back(trans);
	}
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

/*------------------------DrawableStatic------------------------*/

void DrawableStatic::SetPos(glm::vec2 l_pos) {
	if (pos == l_pos)
		return;
	pos = l_pos;
	compute = true;
}
glm::vec2 DrawableStatic::GetPos() { return pos; }

void DrawableStatic::SetOffset(glm::vec2 l_offset) {
	if (offset == l_offset)
		return;
	offset = l_offset;
	compute = true;
}
glm::vec2 DrawableStatic::GetOffset() { return offset; }

void DrawableStatic::SetOrigin(glm::vec2 l_origin) {
	if (origin == l_origin)
		return;
	origin = l_origin;
	compute = true;
}
void DrawableStatic::SetOrigin(Location location) { 
	FloatRect rect = GetFloatRect();
	glm::vec2 globalOrigin = LocationToPosition(rect, location);
	SetOrigin(globalOrigin - GetRealPos());
};
glm::vec2 DrawableStatic::GetOrigin() { return origin; }

glm::vec2 DrawableStatic::GetRealPos() { return pos + offset - origin; }
