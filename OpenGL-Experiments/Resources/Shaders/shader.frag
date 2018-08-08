#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D tex0;  // map_Ka
uniform sampler2D tex1;  // map_Kd
uniform sampler2D tex2;  // map_Ks
uniform sampler2D tex3;  // map_Ns
uniform sampler2D tex4;  // map_bump, map_Bump, bump
uniform sampler2D tex5;  // disp
uniform sampler2D tex6;  // map_d
uniform sampler2D tex7;  // refl
// uniform sampler2D texture1;
// uniform sampler2D texture2;

void main() {
    // TODO(Derek): Use other textures
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    // FragColor = mix(texture(atexture, TexCoord), texture(texture2, TexCoord), 0.2);
    FragColor = texture(tex1, TexCoord);
}
