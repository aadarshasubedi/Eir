#version 150

in vec3 default_position;
in vec4 default_color;

out vec4 default_color_;

void main()
{
	gl_Position = vec4(default_position, 1.0);
	default_color_ = default_color;
}
