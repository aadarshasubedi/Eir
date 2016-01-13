#version 150

in vec3 default_position;
in vec4 default_color;

out vec4 default_color_;

uniform mat4 default_pmat;

void main()
{
   gl_Position = default_pmat * vec4(default_position, 1.0);
   default_color_ = default_color;
}
