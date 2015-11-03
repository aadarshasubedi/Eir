#version 150

in vec2 position;
in vec2 size;
in vec2 uv_offset;
in vec2 uv_size;

out vec2 size_;
out vec2 uv_offset_;
out vec2 uv_size_;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    size_ = size;
    uv_offset_ = uv_offset;
    uv_size_ = uv_size;
}

