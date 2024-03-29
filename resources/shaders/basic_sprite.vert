#version 150

in vec2 position;
in vec2 size;
in vec2 uv_offset;
in vec2 uv_size;
in vec4 color;
in vec2 atlasSize;

out vec2 size_;
out vec2 uv_offset_;
out vec2 uv_size_;
out vec4 color_;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    size_ = size;
    uv_offset_ = uv_offset;
    uv_size_ = uv_size;
    color_ = color;
}
