#version 330 core

in vec4 finalColor;
in vec2 texCoord;

out vec4 vertColor;

uniform vec4 uf_color;
uniform sampler2D uf_texture_0;
uniform sampler2D uf_texture_1;

void main() {
	//vertColor = finalColor;
	//vec4 vertColor_0 = texture(uf_texture_0, texCoord);

	vec4 vertColor_1 = texture(uf_texture_1, texCoord);
	//vertColor = mix(vertColor_0, vertColor_1, 0.3);
	vertColor = vertColor_1;
};