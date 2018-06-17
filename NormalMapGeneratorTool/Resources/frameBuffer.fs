#version 140
in vec2 textureUV;
in vec3 worldPos;
out vec4 color;
uniform sampler2D textureOne;

void main()
{
    color = 1.0 - texture(textureOne,textureUV);
}