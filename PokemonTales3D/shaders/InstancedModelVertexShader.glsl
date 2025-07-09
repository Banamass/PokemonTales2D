#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragWorldPos;

uniform mat4 cameraMatrix;

void main()
{  
    gl_Position = cameraMatrix * instanceMatrix * vec4(aPos, 1.0f);

    TexCoords = aTexCoords;  
    Normal = mat3(instanceMatrix) * aNormal;
    FragWorldPos = vec3(instanceMatrix * vec4(aPos, 1.0f));
}