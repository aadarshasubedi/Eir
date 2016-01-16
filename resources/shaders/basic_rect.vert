#version 150

in vec2 rect_position;
in vec2 rect_size;
in vec4 rect_color;

out vec2 rect_size_;
out vec4 rect_color_;

void main()
{
    gl_Position = vec4(rect_position, 0.0, 1.0);
    rect_size_ = rect_size;
    rect_color_ = rect_color;
}
