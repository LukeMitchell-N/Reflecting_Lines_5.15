#version 430
layout(location = 0) in vec3 pos;   // the position variable has attribute position 0
layout(location = 1) in vec3 color;
layout(location = 2) in float age;

out vec3 finalColor;

void main()
{
    gl_Position = vec4(pos, 1.0);
    finalColor = color;
}   