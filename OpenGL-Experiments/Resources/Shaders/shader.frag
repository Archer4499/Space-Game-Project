#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

// texture samplers
uniform sampler2D tex0;  // ambientTex            map_Ka
uniform sampler2D tex1;  // diffuseTex            map_Kd
uniform sampler2D tex2;  // specularTex           map_Ks
uniform sampler2D tex3;  // specularHighlightTex  map_Ns
uniform sampler2D tex4;  // bumpTex               map_bump, map_Bump, bump
uniform sampler2D tex5;  // displacementTex       disp
uniform sampler2D tex6;  // alphaTex              map_d
uniform sampler2D tex7;  // reflectionTex         refl

uniform vec3 lightColor;

void main() {
    // ambient
    // float ambientStrength = 0.1;
    vec3 ambient = lightColor * vec3(texture(tex0, TexCoord));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * vec3(texture(tex1, TexCoord));

    // specular
    // float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor * vec3(texture(tex2, TexCoord));

    FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
