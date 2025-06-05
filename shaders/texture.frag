#version 330 core

uniform sampler2D texture_diffuse1;
in vec2 frag_tex_coords;
out vec4 out_color;

void main() {
    out_color = texture(texture_diffuse1, frag_tex_coords);
}