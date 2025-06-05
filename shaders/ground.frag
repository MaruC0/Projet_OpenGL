#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D diffuse_map;
uniform vec3 viewPos;
uniform vec3 lightPos;

in vec2 frag_tex_coords;
in vec3 FragPos;
in vec3 Normal;

out vec4 out_color;

void main() {

    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = clamp(dot(norm, lightDir), 0.2, 1.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float shininess = 20;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    out_color = vec4((ambient + diffuse + specular) * texture(diffuse_map, frag_tex_coords).rgb, 1);
}