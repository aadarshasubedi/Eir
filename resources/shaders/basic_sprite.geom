#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 size_[];
in vec2 uv_offset_[];
in vec2 uv_size_[];
in vec4 color_[];

out vec2 texcoord;
out vec4 finalColor;

uniform vec2 atlasSize;

void main()
{
    // bottom left
    gl_Position = gl_in[0].gl_Position + vec4(-0.5 * size_[0].x,  -0.5 * size_[0].y, 0.0, 0.0);
    gl_Position *= vec4(0.08, 0.1066666667, 1.0, 1.0);
    gl_Position += vec4(0.5, 0, 0, 0);
    texcoord = (uv_offset_[0] + uv_size_[0] * vec2(0.0, 0.0)) / atlasSize;
    finalColor = color_[0];
    EmitVertex();

    //up left
    gl_Position = gl_in[0].gl_Position + vec4(-0.5 * size_[0].x, 0.5 * size_[0].y, 0.0, 0.0);
    gl_Position *= vec4(0.08, 0.1066666667, 1.0, 1.0);
    gl_Position += vec4(0.5, 0, 0, 0);
    texcoord = (uv_offset_[0] + uv_size_[0] * vec2(0.0, 1.0)) / atlasSize;
    finalColor = color_[0];
    EmitVertex();

    // bottom right
    gl_Position = gl_in[0].gl_Position + vec4(0.5 * size_[0].x, -0.5 * size_[0].y, 0.0, 0.0);
    gl_Position *= vec4(0.08, 0.1066666667, 1.0, 1.0);
    gl_Position += vec4(0.5, 0, 0, 0);
    texcoord = (uv_offset_[0] + uv_size_[0] * vec2(1.0, 0.0)) / atlasSize;
    finalColor = color_[0];
    EmitVertex();

    // up right
    gl_Position = gl_in[0].gl_Position + vec4(0.5 * size_[0].x, 0.5 * size_[0].y, 0.0, 0.0);
    gl_Position *= vec4(0.08, 0.1066666667, 1.0, 1.0);
    gl_Position += vec4(0.5, 0, 0, 0);
    texcoord = (uv_offset_[0] + uv_size_[0] * vec2(1.0, 1.0)) / atlasSize;
    finalColor = color_[0];
    EmitVertex();
    
    EndPrimitive();
}
