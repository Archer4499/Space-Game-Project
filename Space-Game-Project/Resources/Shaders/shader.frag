#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D tex;
// uniform vec3 spriteColor;

void main() {
    // color = vec4(spriteColor, 1.0) * texture(tex, TexCoords);
    color = texture(tex, TexCoords);
}
