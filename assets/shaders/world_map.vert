#version 330 core
layout (location = 0) in vec2 vertex;

out vec2 TexCoords;

void main()
{
    gl_Position = vec4(vertex, 0.0, 1.0);
    TexCoords = (vertex + 1.) / 2.;
}
