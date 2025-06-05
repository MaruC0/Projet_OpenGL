#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 u_v_coords;

out vec3 FragPos;
out vec3 Normal;
out vec2 frag_tex_coords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    FragPos = vec3(model * vec4(position, 1.0));
    frag_tex_coords = u_v_coords;
    Normal = mat3(transpose(inverse(model))) * aNormal;
}