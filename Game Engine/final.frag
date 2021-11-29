//Fragment shader code

#version 430

in vec4 ex_Color;
in vec2 ex_TextCoord;
uniform int mode;
uniform sampler2D texture_map;
out vec4 out_Color;

void main() {
	if (mode == 0)
		out_Color = ex_Color;
	else{
		out_Color = texture2D(texture_map, ex_TextCoord);
	}
}