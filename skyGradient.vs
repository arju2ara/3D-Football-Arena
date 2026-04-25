#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out float vSkyT;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.999, 1.0);
    TexCoord = aTexCoord;
    vSkyT = aTexCoord.y;
}