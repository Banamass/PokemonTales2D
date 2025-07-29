#include "Shader.h"

/*---------------------Shader---------------------*/

Shader::Shader(const char* vertexPath, const char* fragmentPath){
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexPath, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Impossible to open %s\n", vertexPath);
		getchar();
		ID = 0;
		return;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentPath, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		printf("Impossible to open %s\n", fragmentPath);
		getchar();
		ID = 0;
		return;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexPath);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentPath);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	ID = ProgramID;
}
Shader::~Shader(){
	glDeleteProgram(ID);
}

void Shader::use(){
	glUseProgram(ID);
}

void Shader::unuse() {
	glUseProgram(0);
}

void Shader::SetUniform(const std::string& name, bool value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::SetUniform(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetUniform(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::SetUniform(const std::string& name, glm::vec3 value) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::SetUniform(const std::string& name, glm::vec4 value) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}
void Shader::SetUniform(const std::string& name, const GLfloat* matrix) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matrix);
}

unsigned int Shader::GetID() { return ID; }

/*---------------------ShaderManager---------------------*/

ShaderManager::ShaderManager(SharedContext* context) 
	: ResourceManager("shaders\\shaders.cfg"),
	lightPos(0.0f, 0.0f, 0.0f), viewPos(0.0f, 0.0f, 0.0f)
{
	context->shaderManager = this;
}
ShaderManager::~ShaderManager() {

}

Shader* ShaderManager::Load(const std::vector<std::string>* l_args) {
	if (l_args->size() < 2)
		return nullptr;
	return LoadShader(l_args->at(0).data(), l_args->at(1).data());
}

Shader* ShaderManager::LoadShader(const char* vertexPath, const char* fragmentPath) {
	Shader* shader = new Shader(vertexPath, fragmentPath);

	shader->use();
	shader->SetUniform("viewPos", viewPos);
	shader->SetUniform("lightPos", lightPos);
	shader->unuse();

	return shader;
}
Shader* ShaderManager::GetShader(const std::string& shaderName) {
	if (!RequireResource(shaderName))
		return nullptr;
	return GetResource(shaderName);
}

void ShaderManager::SetLightPos(glm::vec3 pos) {
	for (auto& res : m_resources) {
		auto& shad = res.second.first;
		shad->use();
		shad->SetUniform("lightPos", pos);
		shad->unuse();
	}
	lightPos = pos;
}

void ShaderManager::SetViewPos(glm::vec3 pos) {
	for (auto& res : m_resources) {
		auto& shad = res.second.first;
		shad->use();
		shad->SetUniform("viewPos", pos);
		shad->unuse();
	}
	viewPos = pos;
}