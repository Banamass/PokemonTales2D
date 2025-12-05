#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SharedTypes.h"
#include "Model.h"

class Window;

/* This structure represent all transformations apply to an 3d object.
It contains the translation, the scaling and the rotation.
Useful for model matrix computation */
struct Transform {
public:
	Transform();
	
	//Move the transform
	void Move(glm::vec3 move);
	//Scale the transform
	void Scale(glm::vec3 scale);
	//Rotate the transform
	void Rotate(glm::vec3 rotation);

	//Set the position of the transform
	void SetPosition(glm::vec3 pos);

	//Get the matrix associated to the transformations applied to this object
	glm::mat4 GetTransform();
	//Get the scaling of this object
	glm::vec3 GetScaling();

private:
	void ResetTransformations();
	void ComputeTransform();

	glm::vec3 translation;
	glm::vec3 scaling;
	glm::vec3 rotation; //pitch(around x) yaw(around y) roll(around z)
	glm::mat4 transform;

	friend class Window;
};

/* Oriented bounding box, useful for ray tracing of object 
AABBMin and AABBMax correspond to min and max coords value in the tree dimensions of the OBB */
class OBB {
public:
	OBB();
	OBB(std::pair<glm::vec3, glm::vec3> l_AABBMinMax, Transform* l_modelTransform);
	~OBB() {}

	/* Test if a ray go through the obb, transformed by the matrix ModelMatrix
	Params:
	- ray_origin : the origin of the considered ray
	- ray_direction : the oriented direction of the ray
	- intersection_distance& : this param is used to store the value of the distance between the ray and the OBB
	- scaling : the scaling apply to this model matrix
	*/
	bool TestRayOBBIntersection(
		glm::vec3 ray_origin,
		glm::vec3 ray_direction,
		float& interstion_distance);

	void Setup(std::pair<glm::vec3, glm::vec3> l_AABBMinMax, Transform* l_modelTransform);

private:
	Transform* modelTransform;

	glm::vec3 AABBMin;
	glm::vec3 AABBMax;
};

/* Class representing a 3D objet that can be drawn on the window */
class Drawable {
public:
	/* Structure storing the behavior of an 3D object in the face of light 
	The default material is a white object with no light effect and shininess at 32.0*/
	struct Material {
		Material()
			: ambient(1.0f), diffuse(1.0f), specular(1.0f), shininess(32.0f), alpha(1.0f)
		{}

		void SetPlainColor(glm::vec3 color);
		void SetLightningColor(glm::vec3 color, float ambientFact = 0.0f, float specularFact = 0.0f);

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float shininess;
		float alpha;
	};

	Drawable();
	Drawable(Model* model, Shader* shader);
	Drawable(Model* model, Shader* shader, Material l_material);
	~Drawable();

	/* Test if a ray go through the 3D object
	Params:
	- ray_origin : the origin of the considered ray
	- ray_direction : the oriented direction of the ray
	- intersection_distance& : this param is used to store the value of the distance between the ray and the object
	*/
	bool TestRayIntersection(
		const glm::vec3& ray_origin,
		const glm::vec3& ray_direction,
		float& interstion_distance);

	void Move(glm::vec3 move);
	void Scale(float uniScale);
	void Scale(glm::vec3 scale);
	void Rotate(glm::vec3 rotation);

	void SetPosition(glm::vec3 l_pos);
	void SetOffset(glm::vec3 l_offset);
	void SetOrigin(glm::vec3 l_origin);

	void SetModel(Model* l_model);
	void SetShader(Shader* l_shader);
	void SetMaterial(const Material& l_material);

	glm::vec3 GetPosition();
	glm::vec3 GetOffset();
	glm::vec3 GetOrigin();
	glm::vec3 GetRealPosition();
	//Get the transform representing all the transformations applied to the original model
	Transform* GetTransform() { return &transform; }

private:
	void Setup(Model* l_model, Shader* l_shader, Material& l_material);

	Material material;

	Model* model;
	Shader* shader;
	Transform transform;
	OBB obb;

	glm::vec3 pos;
	glm::vec3 origin;
	glm::vec3 offset;

	friend class Window;
};

/* Class representing a set of 3D object with the same aspect representing with an unique Drawable.
Each of these objects have their own Transform. Thus, the Transform of the unique Drawable isn't used*/
class DrawableInstanced {
public:
	DrawableInstanced(Drawable* drawable, std::vector<Transform*>& instanceMatrix);
	virtual ~DrawableInstanced(){}

	//Get the number of instance drawn
	unsigned int GetNbInstance();
	//Getting the VBO that stores all different model matrix
	unsigned int GetInstanceVBO();
	//Get if the VAO of the model as to be setup
	bool IsVAOSetup();
	//Indicate that the vao of the model is setup
	void VAOSetup();

	//Delete transforms object so erase also all instance drawable
	void DeleteTransforms();

private:
	//Reference drawable for multiple drawing
	Drawable* drawable;
	std::vector<Transform*> instanceTransforms;

	unsigned int instanceVBO;
	bool isVAOSetup;

	friend class Window;
};

/* Class representing a 2D object, that can be drawn on the window, with screen coords */
class DrawableStatic {
public:
	DrawableStatic() 
		: pos(0.0f), offset(0.0f), origin(0.0f), compute(true) {}
	DrawableStatic(glm::vec2 l_pos) 
		: pos(l_pos), offset(0.0f), origin(0.0f), compute(true) {}
	virtual ~DrawableStatic() {}

	//Draw the object according with the camera matrix provided
	virtual void Draw(glm::mat4& cameraMatrix) = 0;

	//Set the position of the object
	void SetPos(glm::vec2 l_pos);
	//Get the position of the object
	glm::vec2 GetPos();

	//Set the offset of the object, representing the origin of the pos coords
	virtual void SetOffset(glm::vec2 l_offset);
	//Get the offset of the object, representing the origin of the pos coords
	virtual glm::vec2 GetOffset();

	//Set the origin of the object, in its local coords
	virtual void SetOrigin(glm::vec2 l_origin);
	/*Set the offset of the object, with a giving location in the hitbox
	Warning : this origin doesn't adapt if the size of the object is changed, another call to this method is needed*/
	virtual void SetOrigin(Location location);
	//Get the origin of the object
	glm::vec2 GetOrigin();

	//Get the position according to the global origin of the screen(bottom right corner)
	glm::vec2 GetRealPos();

	//Get the rectangle box of the drawable static, warning : depending on the object, can have a high complexity
	virtual FloatRect GetFloatRect() = 0;

protected:
	glm::vec2 pos;
	glm::vec2 offset;
	glm::vec2 origin;

	bool compute; //This attribut store if the object has been modified this frame
};