#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragWorldPos;

uniform mat4 transform;
uniform mat4 model;

void main()
{  
    gl_Position = transform * vec4(aPos, 1.0f);

    TexCoords = aTexCoords;  
    Normal = mat3(model) * aNormal;
    FragWorldPos = vec3(model * vec4(aPos, 1.0f));
}