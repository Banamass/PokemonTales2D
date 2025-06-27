#version 330 core

in vec2 texCoord;

uniform sampler2D tex1;
uniform sampler2D tex2;

out vec4 FragColor;

void main() {
	FragColor = vec4(texCoord, 1.0f, 1.0f);
}