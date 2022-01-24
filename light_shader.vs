#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform float x_c;
uniform float y_c;

void main()
{
    mat4 v = mat4(1.0,0.0, 0.0, 0.0,  0.0,1.0,0.0,0.0,  0.0,0.0,1.0,0.0,  x_c*cos(time),y_c*sin(time+x_c),0.0,1.0);
    mat4 translate = model * v;
    FragPos = vec3(translate * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * translate * vec4(aPos, 1.0);
}
