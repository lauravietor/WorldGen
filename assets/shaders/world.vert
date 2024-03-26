#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 offset;
layout (location = 2) in float textureIndex;

out vec3 TexCoords;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vertex + offset, 0.0, 1.0);
    TexCoords = vec3(vertex, textureIndex);
}
