#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2DArray myTexture;

void main()
{
    vec4 Color = texture(myTexture, TexCoords);
    FragColor = vec4(Color.rgb, 1);
}
